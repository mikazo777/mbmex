#ifndef DEBUG_HPP
#define DEBUG_HPP
#include <iostream>
#include <cstdio>
#include <stdio.h>

#define MBMEX_DEBUG_ON
using namespace std;
//void debugPrint(string str);
void debugPrint(const char *pPrintMsg,
				const char *pFile = __builtin_FILE(),
				int lineNum = __builtin_LINE(),
				const char *pFunc = __builtin_FUNCTION());
void tracePrint(const char *pPrintMsg,
				const char *pFile = __builtin_FILE(),
				int lineNum = __builtin_LINE(),
				const char *pFunc = __builtin_FUNCTION());
void debugParamCharPrint(const char *pPrintMsg_1,
						 const char *pParamChar,
						 const char *pFile = __builtin_FILE(),
						 int lineNum = __builtin_LINE(),
						 const char *pFunc = __builtin_FUNCTION());
void debugParamIntPrint(const char *pPrintMsg_1,
						const int paramInt,
						const char *pFile = __builtin_FILE(),
						int lineNum = __builtin_LINE(),
						const char *pFunc = __builtin_FUNCTION());
#endif // DEBUG_HPP
