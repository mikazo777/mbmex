#include <error.h>
#include <assert.h>
#include <iostream>
#include <string.h>
#include <linux/input.h>
#include <pthread.h>
#include "searchEvent.hpp"
using namespace std;

searchEvent::searchEvent(const char *pInputPath) {
	tracePrint("searchEvent::searchEvent start");
	int retValue = -1;
	memset(deviceName, 0x00, sizeof(deviceName));
	if (nullptr != pInputPath) {
		inputFd = open(pInputPath, O_RDWR);
		if (0 <= inputFd) {
			retValue = ioctl(inputFd, EVIOCGNAME(sizeof(deviceName)), deviceName);
			if (0 <= retValue) { 
				outputFd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
				searchEventSts = true;
			} else {
				retValue = close(inputFd);
				debugPrint("ioctl error");
			}
		}
	}
	if (false == searchEventSts) {
#ifdef MBMEX_DEBUG_ON
		cout << "pInputPath = " << pInputPath << endl;
		cout << "inputFd =" << inputFd << endl;
		cout << "outputFd =" << outputFd << endl;
#endif //MBMEX_DEBUG_ON
	}
	tracePrint("searchEvent::searchEvent end");
}
searchEvent::~searchEvent() {
	ioctl(inputFd, EVIOCGRAB, 0);
	close(inputFd);
	close(outputFd);
}

int searchEvent::eventTask(void) {
    int readResult = -1;
    struct input_event event;
    cout << "searchEvent start" << endl;
    while(true) {
		readResult = read(inputFd, &event, sizeof(event));
        if (-1 == readResult) {
            debugPrint("read error do stop");
            break;
        } else if (readResult != sizeof(event)) {
            printf("size error readResult = %d", readResult);
            break;
        } else if ((EV_KEY == event.type) &&
                (event.value == 1)) {
        	cout << "deviceName" << deviceName << endl;
        	cout << "inputEvent.type" << event.type << endl;
        	cout << "InputEvent.code" << event.code << endl;
        	cout << "inputEvent.value" << event.value << endl;
        	fflush(stdout);
		} else {
			;
		}
    }
    close(inputFd);
}

int main(void) {
    int retValue = -1;
    int devNum = 0;
    int lpCnt = 0;
    int searchEvtCnt = 0;
    struct dirent **ppNameList;

    devNum = scandir(DEV_INPUT_DIR, &ppNameList, checkEventDevice, versionsort);

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

            printf("filename = %s\n", filename);
            pSearchEvt[searchEvtCnt] = new searchEvent(filename);
            if (nullptr != pSearchEvt[searchEvtCnt]) {
                if (false == pSearchEvt[searchEvtCnt]->searchEventSts) {
                    delete pSearchEvt[searchEvtCnt];
                } else {
                    searchEvtCnt ++;
                }
            }
        }
        pthread_t handle;  // Thread handle
        for (lpCnt = 0; lpCnt < searchEvtCnt; lpCnt++ ) {
            retValue = pthread_create(&handle,
                                      nullptr,
                                      searchEvent::planchThread,
                                      pSearchEvt[lpCnt]);
        }
        pthread_join(handle, nullptr);
    }
    return retValue;
}


