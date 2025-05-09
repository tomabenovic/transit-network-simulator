#ifndef NETWORK_H
#define NETWORK_H

#include "Stop.h"

#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <tuple>

class Network {
public:

	Network(const Network&) = delete;
	Network& operator=(const Network&) = delete;
	~Network();

	static Network& getInstance();

	void loadGraph(const string& filename);
	void loadLines(const string& filename);

	void printStationInformations(int id) const;
	void printLineInformations(const string& name) const;

	vector<pair<int, string>> findMinTransfersPath(int start, int target) const;
	void minTransfersPath(int start, int target);

	void throughAllImportantsPath();

	void printPath(const vector<pair<int, string>>& path, ofstream& output) const;

private:

	Network();
	map<int, Stop*> graph_;
	map<string, vector<int>> bus_lines_;


};

#endif 