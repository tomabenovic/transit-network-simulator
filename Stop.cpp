#include "Stop.h"

void Stop::addBus(const string& bus_name){
	lines_.push_back(bus_name);
}

void Stop::addNeighbor(Stop* neighbor, const string& line) {
	for (auto& it : neighbors_) {
		if (neighbor == it.first) {
			it.second.push_back(line);
			return;
		}
	}
	neighbors_.push_back(make_pair(neighbor, vector<string>{line}));
}
vector<pair<Stop*, vector<string>>> Stop::getNeighbors() const{
	return neighbors_;
}

int Stop::getId() const{
	return id_;
}

string Stop::getName() const{
	return name_;
}

bool Stop::isImportant() const{
	return important_;
}

void Stop::sortLines(){
	for (int i = 0; i < lines_.size() - 1; i++) {
		for (int j = i + 1; j < lines_.size(); j++) {
			if (lines_[j] < lines_[i]) {
				string t = lines_[i];
				lines_[i] = lines_[j];
				lines_[j] = t;
			}
		}
	}
}
