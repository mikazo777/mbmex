#include <error.h>
#include <assert.h>
#include <iostream>
#include <string.h>
#include <linux/input.h>
#include <thread>
#include "searchEvent.hpp"
#include "../mbmexBase.hpp"
using namespace std;

searchEvent::searchEvent(const char *pInputPath) {
	tracePrint("start");
	int retValue = -1;
	memset(deviceName, 0x00, sizeof(deviceName));
	if (nullptr != pInputPath) {
		inputFd = open(pInputPath, O_RDWR);
		if (0 <= inputFd) {
			retValue = ioctl(inputFd,
							 EVIOCGNAME(sizeof(deviceName)),
							 deviceName);
		} else {
			debugPrint("open error");
		}
		if (0 <= retValue) { 
			retValue = ioctl(inputFd, EVIOCGID, &inputId);
		} else {
			debugPrint("ioctl error EVIOCGNAME");
		}
		if (0 <= retValue){ 
			inputPath = pInputPath;
			searchEventSts = true;
		} else {
			retValue = close(inputFd);
			debugPrint("ioctl error EVIOCGID");
		}
	}
	if (false == searchEventSts) {
		debugParamCharPrint("pInputPath", pInputPath);
		debugParamIntPrint("inputFd", inputFd);
	}
	tracePrint("end");
}
searchEvent::~searchEvent() {
	ioctl(inputFd, EVIOCGRAB, 0);
	close(inputFd);
}

int searchEvent::eventTask(void) {
    int readResult = -1;
    struct input_event event;
    tracePrint("start");
    while(true) {
		readResult = read(inputFd, &event, sizeof(event));
        if (-1 == readResult) {
            debugPrint("read error do stop");
            break;
        } else if (readResult != sizeof(event)) {
			debugParamIntPrint("size error readResult", readResult);
            break;
		} else if (EV_KEY == event.type) {
			cout << "----------------------" << endl;
			cout << "inputPath -> " << inputPath << endl;
        	cout << "deviceName-> " << endl;
			cout << deviceName << endl;
			cout << "deviceVendor->" << endl;
			cout << "0x" << hex << inputId.vendor << endl;
			cout << "deviceType->" << endl;
			cout << "0x" << hex << inputId.product << endl;
        	cout << "inputEvent.type->" << endl;
			cout << "0x" << hex << event.type << endl;
        	cout << "InputEvent.code->" << endl;
			cout << "0x" << hex << event.code << endl;
			cout << "InputEvent.value->" << endl;
			cout << "0x" << hex << event.value << endl;
			cout << "----------------------" << endl;
        	fflush(stdout);
		} else {
			;
		}
    }
    close(inputFd);
	return readResult;
}

int main(void) {
    int retValue = -1;
    int devNum = 0;
    int lpCnt = 0;
    int searchEvtCnt = 0;
    struct dirent **ppNameList;

    devNum = scandir(devInputDir, &ppNameList, checkEventDevice, versionsort);

    if (0 < devNum) {
        searchEvent *pSearchEvt[devNum];
        for (lpCnt = 0; lpCnt < devNum; lpCnt++) {
            char filename[sizeof(devInputDir) + sizeof((ppNameList[lpCnt])->d_name)];
            memset(filename, 0x00, sizeof(filename));
            snprintf(filename,
                     sizeof(filename),
                     "%s/%s",
                     devInputDir,
                     (ppNameList[lpCnt])->d_name);

			debugParamCharPrint("filename", filename);
            pSearchEvt[searchEvtCnt] = new searchEvent(filename);
            if (nullptr != pSearchEvt[searchEvtCnt]) {
                if (false == pSearchEvt[searchEvtCnt]->searchEventSts) {
                    delete pSearchEvt[searchEvtCnt];
                } else {
                    searchEvtCnt ++;
                }
            }
        }
		thread searchEvtThreads[searchEvtCnt];
        for (lpCnt = 0; lpCnt < searchEvtCnt; lpCnt++ ) {
			searchEvtThreads[lpCnt] = thread(searchEvent::planchThread,
											 pSearchEvt[lpCnt]);
        }
        for (lpCnt = 0; lpCnt < searchEvtCnt; lpCnt++ ) {
			searchEvtThreads[lpCnt].join();
		}
    }
    return retValue;
}


