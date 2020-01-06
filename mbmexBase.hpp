#ifndef MBMEX_BASE_HPP
#define MBMEX_BASE_HPP
#include <dirent.h>
const char devInputDir[] = "/dev/input";
int checkEventDevice(const struct dirent *pDir);
#endif	// MBMEX_MAIN_HPP
