#include <iostream>
#include "debug.hpp"
using namespace std;

void debugPrint(string str) {
#ifdef MBMEX_DEBUG_ON
	cout << "error no = " << errno << endl;
	
	perror(str.c_str());
	return;
#endif // MBMEX_DEBUG_ON
}

void tracePrint(string str) {
#ifdef MBMEX_DEBUG_ON
	cout << str << endl;
#endif // MBMEX_DEBUG_ON
}
