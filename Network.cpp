#include "Network.h"

#include "Exceptions.h"

#include <sstream>

Network::Network(){}

Network::~Network() {
	for (auto& it : graph_) {
		delete(it.second);
	}
}

Network& Network::getInstance(){
	static Network network;
	return network;
}

void Network::loadGraph(const string& filename){
	ifstream input(filename);
	if (!input.is_open()) {
		throw new FileNotFound();
	}

	string row;
	while (getline(input, row)) {
		istringstream iss(row);
		int id;
		string name;
		bool important = false;
		iss >> id;
		iss.ignore();
		getline(iss, name, '[');
		if (iss.peek() == '!') important = true;
		if (important) {
			name = name.substr(0, name.size() - 1);
		}
		graph_[id] = new Stop(name, id, important);
	}
	input.close();
	cout << endl << "Informacije o stajalistima su uspesno procitane!" << endl << endl;
}

void Network::loadLines(const string& filename){
	ifstream input(filename);
	if (!input.is_open()) {
		throw new FileNotFound();
	}

	string row;
	while (getline(input, row)) {
		istringstream iss(row);
		
		string name;
		vector<int> stops;
		int id;

		iss >> name;
		iss.ignore();

		while (iss >> id) {
			stops.push_back(id);
			graph_[id]->lines_.push_back(name);
			graph_[id]->sortLines();
		}
		
		for (int i = 0; i < stops.size() - 1; i++) {
			graph_[stops[i]]->addNeighbor(graph_[stops[i+1]], name);
			graph_[stops[i+1]]->addNeighbor(graph_[stops[i]], name);
		}
		
		bus_lines_[name] = stops;

		stops.clear();
	}
	input.close();
	cout << endl << "Informacije o linijama su uspesno procitane!" << endl << endl;
}

void Network::printStationInformations(int id) const{
	auto it = graph_.find(id);
	if (it == graph_.end()) {
		throw new StopNotFound();
	}

	string filename = "stajaliste_" + to_string(id) + ".txt";
	ofstream output(filename);

	Stop* s = it->second;

	output << s->id_ << " " << s->name_ << " [";
	if (s->lines_.empty()) output << "]";
	for (string& line : s->lines_) {
		if (line == s->lines_.back()) {
			output << line << "]";
			break; 
		}
		output << line << " ";
	}
	output << "{! ";
	set<int> importants;
	for (auto& neighbour : s->neighbors_) {
		if (neighbour.first->isImportant()) importants.insert(neighbour.first->id_);
	}
	for (int i : importants) output << i << " ";
	importants.empty() ? output << " !}" << endl : output << "!}" << endl;
	importants.clear();
	output.close();
	cout << "Izlazna datoteka '" << filename << "' je uspesno formirana!" << endl << endl;
}

void Network::printLineInformations(const string& name) const{
	auto line = bus_lines_.find(name);
	if (line == bus_lines_.end()) {
		throw new LineNotFound();
	}

	string filename = "linija_" + name + ".txt";
	ofstream output(filename);

	int start, end;
	start = line->second.front();
	end = line->second.back();

	output << name << " " << graph_.find(start)->second->name_ << " -> " <<graph_.find(end)->second->name_ << endl;
	for (int i : line->second) {
		Stop* s = graph_.find(i)->second;
		
		output << s->id_ << " " << s->name_;
		if (s->isImportant()) output << " [!]";
		output << endl;
	}
	output.close();
	cout << "Izlazna datoteka '" << filename << "' je uspesno formirana!" << endl << endl;
}

vector<pair<int, string>> Network::findMinTransfersPath(int start, int target) const{
	if (graph_.find(start) == graph_.end() || graph_.find(target) == graph_.end()) {
		throw new StopNotFound;
	}
	queue<tuple<int, string, int, vector<pair<int, string>>>> q;
	map<int, int> visited;
	vector<pair<int, string>> bestPath;
	int minTransfers = INT32_MAX;

	q.emplace(start, "", 0, vector<pair<int, string>>{make_pair(start, "")});
	visited[start] = 0;

	int currentStation;
	string currentLine;
	int transfers;
	vector<pair<int, string>> path;

	while (!q.empty()){
		currentStation = get<0>(q.front());
		currentLine = get<1>(q.front());
		transfers = get<2>(q.front());
		path = get<3>(q.front());
		q.pop();

		if (currentStation == target && transfers < minTransfers) {
			bestPath = path;
			minTransfers = transfers;
			if (transfers == 1) {
				break;
			}
		}

		for (auto& neighbour : graph_.find(currentStation)->second->neighbors_) {
			int neighbourID = neighbour.first->id_;
			for (string& line : neighbour.second) {
				if (visited.find(neighbourID) == visited.end() || transfers < visited.find(neighbourID)->second || (transfers == visited.find(neighbourID)->second) and currentLine == line and path[path.size() - 2].first != neighbourID) {
					int nextTransfers = currentLine == line ? transfers : transfers + 1;
					visited[neighbourID] = nextTransfers;
					auto newPath = path;
					newPath.push_back(make_pair(neighbourID, line));
					q.emplace(neighbourID, line, nextTransfers, move(newPath));
				}
			}
		}
	}
	return bestPath;
}

void Network::minTransfersPath(int start, int target)
{
	vector<pair<int, string>> bestPath = findMinTransfersPath(start, target);
	string filename = "putanja_" + to_string(start) + "_" + to_string(target) + ".txt";
	ofstream output(filename);
	printPath(bestPath, output);
	cout << "Datoteka '" << filename << "' je uspesno formirana!" << endl << endl;
}

void Network::throughAllImportantsPath(){
	vector<int> importants;
	set<int> visited;

	for (auto& it : graph_) {
		if (it.second->isImportant()) {
			importants.push_back(it.first);
		}
	}
	if (importants.size() == 0) {
		cout << "Ne postoje bitne stanice u sistemu!" << endl << endl;
		return;
	}
	if (importants.size() == 1) {
		cout << "Samo jedna bitna stanica: " << importants[0] << endl << endl;
		return;
	}
	vector<pair<int, string>> path;
	ofstream output("putanja_bitne_stanice.txt");
	for (int i = 0; i < importants.size() - 1; i++) {
		if (visited.find(importants[i]) == visited.end()) {
			path = findMinTransfersPath(importants[i], importants[i + 1]);
			printPath(path, output);
			for (auto& it : path) {
				if (it.first == path.back().first) break;
				if (graph_.find(it.first)->second->isImportant()) visited.insert(it.first);
			}
		}
	}
	cout << "Datoteka 'putanja_bitne_stanice.txt' je formirana!" << endl << endl;
	output.close();
}



void Network::printPath(const vector<pair<int, string>>& path, ofstream& output) const{
	//ofstream output(filename);
	output << "->" << path[1].second << endl << path[0].first << " ";
	for (int i = 1; i < path.size() - 1; i++) {
		if (path[i].second != path[i + 1].second) {
			output << path[i].first << endl;
			output << path[i].second << " -> " << path[i + 1].second << endl;
		}
		output << path[i].first << " "; 
	}
	if (path[path.size() - 1].second == path[path.size() - 1].second) output << path[path.size() - 1].first << endl;
}

