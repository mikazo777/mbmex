#ifndef DEBUG_HPP
#define DEBUG_HPP
#include <iostream>
#include <cstdio>
#include <stdio.h>

//#define MBMEX_DEBUG_ON
using namespace std;
void debugPrint(string str);
void tracePrint(string str);
void msgPrint(const char *pPrintMsg,
			  const char *pFile = __builtin_FILE(),
			  int lineNum = __builtin_LINE(),
			  const char *pFunc = __builtin_FUNCTION());
#endif // DEBUG_HPP
