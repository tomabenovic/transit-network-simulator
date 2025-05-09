#ifndef TERMINAL_H
#define TERMINAL_H

#include "Network.h"
#include "Exceptions.h"
#include <iostream>

using namespace std;

class Terminal {
public:
	Terminal(const Terminal&) = delete;
	Terminal& operator=(const Terminal&) = delete;

	static Terminal& getInstance();
	void openTerminal();


private:
	Terminal();
};

#endif