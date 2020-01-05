#ifndef SEARCH_EVENT_HPP
#define SEARCH_EVENT_HPP
#include <errno.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include "../mbmexBase.hpp"
#include "../debug.hpp"
using namespace std;

class searchEvent {
public:
    bool searchEventSts;
    searchEvent(const char *pInputPath);
    ~searchEvent();
    int eventTask(void);
	static void *planchThread(void *pParam) {
	    reinterpret_cast<searchEvent*>(pParam)->eventTask();
    	pthread_exit(NULL);
	};
protected:
    int inputFd;        // input fd
	char deviceName[UINPUT_MAX_NAME_SIZE];

};
#endif //SEARCH_EVENT_HPP
