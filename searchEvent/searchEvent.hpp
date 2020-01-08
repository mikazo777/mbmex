#ifndef SEARCH_EVENT_HPP
#define SEARCH_EVENT_HPP
#include <errno.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
	struct input_id inputId;
	string inputPath;
	char deviceName[UINPUT_MAX_NAME_SIZE];

};
#endif //SEARCH_EVENT_HPP
