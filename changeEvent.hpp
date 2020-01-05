#ifndef CHANGE_EVENT_HPP
#define CHANGE_EVENT_HPP
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
#include "debug.hpp"


#define UINPUTPATH "/dev/uinput"
using namespace std;
typedef struct {
    const char *pTargetDeviceName;
	const int detailDeviceType;
    int targetOpeNum;
    int changeEventNum;
} TARGET_TBL_T;


class changeEvent {
public:
    bool changeEventSts;
    changeEvent(const char *pInputPath);
    ~changeEvent();
    int changeEventTask(void);
	int searchDeviceList(char *pDeviceName, struct input_id &pInputId);
	static void *planchThread(void *pParam) {
	    reinterpret_cast<changeEvent*>(pParam)->changeEventTask();
    	pthread_exit(NULL);
	};
private:
    int targetEvent;    // target change event num
    int inputFd;        // input fd
    int outputFd;       // output fd
    int outEvent;       // change event num
    int createOutputDvRet;  // 
	char deviceName[UINPUT_MAX_NAME_SIZE];
	struct input_id inputId;

    int createOutputDevice(int createFd, const char *pInputDevName);
    int changeOperation(__u16 inputEventType);
    int writeChangeEvent(int setEventType, int eventCode, int setValue);
};
//int checkEventDevice(const struct dirent *pDir);
#endif //CHANGE_EVENT_HPP
