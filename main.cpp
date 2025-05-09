#include "Network.h"
#include "Terminal.h"

int main() {
	Terminal::getInstance();
	Terminal::getInstance().openTerminal();
	return 0;
}