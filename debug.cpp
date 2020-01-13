#include "debug.hpp"
#include <cstdio>
using namespace std;

void debugPrint(const char *pPrintMsg,
			    const char *pFile,
			    const int pLine,
			    const char *pFunction) {
	cout << pFile << ":" << dec << pLine << ":" << pFunction << ":" << pPrintMsg <<  endl;
	return;
}
void tracePrint(const char *pPrintMsg,
			    const char *pFile,
			    const int pLine,
			    const char *pFunction) {
#ifdef MBMEX_DEBUG_ON
	cout << pFile << ":" << dec << pLine << ":" << pFunction << ":" << pPrintMsg <<  endl;
#endif
	return;
}
void debugParamCharPrint(const char *pPrintMsg_1,
						 const char *pParamChar,
						 const char *pFile,
						 const int pLine,
						 const char *pFunction) {
#ifdef MBMEX_DEBUG_ON
	cout << pFile << ":" << dec << pLine << ":" << pFunction << ":" << pPrintMsg_1 << "="<< pParamChar <<  endl;
#endif
	return;
}
void debugParamIntPrint(const char *pPrintMsg_1,
						const int paramInt,
						const char *pFile,
						const int pLine,
						const char *pFunction) {
#ifdef MBMEX_DEBUG_ON
	cout << pFile << ":" << dec << pLine << ":" << pFunction << ":" << pPrintMsg_1 << "="<< paramInt <<  endl;
#endif
	return;
}
