#ifndef STOP_H
#define STOP_H

#include <string>
#include <utility>
#include <vector>

using namespace std;

class Stop {
public:
	friend class Network;
	Stop(const Stop&) = delete;
	Stop& operator=(const Stop&) = delete;

	void addBus(const string& bus_name);
	void addNeighbor(Stop* neighbor, const string& line);

	vector<pair<Stop*, vector<string>>> getNeighbors() const;
	int getId() const;
	string getName() const; 
	bool isImportant() const;
	void sortLines();

private:
	Stop(const string& name, const int& id, const bool& important) : name_(name), id_(id), important_(important), lines_(), neighbors_() {};
	string name_;
	int id_;
	bool important_;
	
	vector <pair<Stop*, vector<string>>> neighbors_; 
	vector<string> lines_;

};

#endif 
