#include <iostream>
using namespace std;

void debugPrint(string str) {
#ifdef MMCI_DEBUG_ON
	cout << "error no = " << errno << endl;
	
	perror(str.c_str());
	return;
#endif // MMCI_DEBUG_ON
}

void tracePrint(string str) {
#ifdef MMCI_DEBUG_ON
	cout << str << endl;
#endif // MMCI_DEBUG_ON
}
