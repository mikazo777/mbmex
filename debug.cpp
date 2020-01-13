#include "debug.hpp"
#include <cstdio>
using namespace std;

void msgPrint(const char *pPrintMsg,
			  const char *pFile,
			  const int pLine,
			  const char *pFunction) {
#ifdef MBMEX_DEBUG_ON
	cout << pFile << ":" << dec << pLine << ":" << pFunction << ":" << pPrintMsg <<  endl;
#endif
	return;
}
void debugPrint(string str) {
#ifdef MBMEX_DEBUG_ON
//	cout << "error no = " << errno << endl;
	//mbmexDebug::msgPrint(__builtin_FILE(), __builtin_LINE());
	
	perror(str.c_str());
	return;
#endif // MBMEX_DEBUG_ON
}

void tracePrint(string str) {
#ifdef MBMEX_DEBUG_ON
	cout << str << endl;
#endif // MBMEX_DEBUG_ON
}
