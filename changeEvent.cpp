#include <error.h>
#include <iostream>
#include <string.h>
#include "changeEvent.hpp"
#include <linux/input.h>
#include "main.hpp"
//#include <sys/epoll.h>
#include <pthread.h>
#include "logitechMouseProduct.hpp"
using namespace std;

TARGET_TBL_T targetDevNameTable[] = {
	{ "Logitech USB Receiver Consumer Control", MX_REVOLUTION_1, 0x00D9, BTN_TASK },
	{ "Logitech USB Receiver Consumer Control", MX_REVOLUTION_2, 0x00D9, BTN_TASK },
	{ "Logitech USB Receiver Consumer Control", MX_REVOLUTION_3, 0x00D9, BTN_TASK },
	{ "Logitech USB Receiver Consumer Control", MX_REVOLUTION_4, 0x00D9, BTN_TASK },
	{ "Logitech USB Receiver Consumer Control", MX_REVOLUTION_5, 0x00D9, BTN_TASK },
	{ "Logitech USB Receiver Consumer Control", MX_5500, 0x00D9, BTN_TASK },
	{ "MX Master 2S Keyboard", MX_MASTER2S_KEYBOARD, 0x001D, BTN_TASK },
	{ "MX Master 2S Keyboard", MX_MASTER2S_KEYBOARD, 0x000F, 0x0000 },
	{ "MX Master 2S Keyboard", MX_MASTER2S_KEYBOARD, 0x0038, 0x0000 },
	{ nullptr, 0x0000, 0x0000 }
};

changeEvent::changeEvent(const char *pInputPath) {
	tracePrint("changeEvent::changeEvent start");
	int retValue = -1;
	memset(deviceName, 0x00, sizeof(deviceName));
	if (nullptr != pInputPath) {
		inputFd = open(pInputPath, O_RDWR);
		if (0 <= inputFd) {
			retValue = ioctl(inputFd, EVIOCGNAME(sizeof(deviceName)), deviceName);
			if (0 <= retValue) { 
				retValue = ioctl(inputFd, EVIOCGID, &inputId);
				if (0 <= retValue) { 
					retValue = searchDeviceList(deviceName, inputId);
					if (0 == retValue) {
						outputFd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
						if (-1 != outputFd) {
							//outEvent = changeEventNum;
							createOutputDvRet = createOutputDevice(outputFd, 
																   deviceName);
							ioctl(inputFd, EVIOCGRAB, 1);
							if (createOutputDvRet != -1) {
								changeEventSts = true;
							} else {
								close(outputFd);
							}
						}
					} else {
						tracePrint("Target DeviceName was not found");
					}
				}
			}
			if (0 > retValue) {
				retValue = close(inputFd);
				debugPrint("searchDeviceList between createOutputDevice error");
			}
		}
	}
	if (false == changeEventSts) {
#ifdef MBMEX_DEBUG_ON
		cout << "pInputPath = " << pInputPath << endl;
		cout << "inputFd =" << inputFd << endl;
		cout << "outputFd =" << outputFd << endl;
		cout << "createOutputDvRet =" << createOutputDvRet << endl;
#endif //MBMEX_DEBUG_ON
	}
	tracePrint("changeEvent::changeEvent end");
}
changeEvent::~changeEvent() {
	ioctl(inputFd, EVIOCGRAB, 0);
	close(inputFd);
	close(outputFd);
}
int changeEvent::changeEventTask(void) {
	int readResult = -1;
    struct input_event inputEvent;

	cout << "DeviceName = " << deviceName << endl;
	while(true) {
		readResult = read(inputFd, &inputEvent, sizeof(inputEvent));
		if (-1 == readResult) {
			debugPrint("read error do stop");
			break;
		} else if (readResult != sizeof(inputEvent)) {
			printf("size error readResult = %d", readResult);
			break;
		} else if ((EV_KEY == inputEvent.type) && 
				(targetEvent == inputEvent.code) && 
				(inputEvent.value == 1)) {
			readResult = changeOperation(inputEvent.type);
			if (readResult == -1) {
				break;
			}
		} else {
#ifdef MBMEX_DEBUG_ON
			cout << "skip.Because" << endl;
			cout << "EV_KEY = " << EV_KEY << endl;
			cout << "targetEvent = " << targetEvent << endl;
			cout << "inputEvent.code = " << inputEvent.code << endl;
			cout << "inputEvent.value = " << inputEvent.value << endl;
#endif // MBMEX_DEBUG_ON
		}
	}
	changeEventSts = false;
	return readResult;
}

int changeEvent::writeChangeEvent(int setEventType, 
								  int eventCode, 
								  int setValue) {
	struct input_event writeEvent;
	writeEvent.type = setEventType;
	writeEvent.code = eventCode;
	writeEvent.value = setValue;
	writeEvent.time.tv_sec = 0;
	return write(outputFd, &writeEvent, sizeof(writeEvent));
}
int changeEvent::changeOperation(__u16 inputEventType) {
    int retValue = -1;
    // Press 0x00D9 -> Press mouse button 10
    retValue = writeChangeEvent(inputEventType, outEvent, 1);

    if (-1 == retValue) {
		debugPrint("changeEvent::changeOperation checkpoint 1 error");
	} else {
        retValue = writeChangeEvent(EV_SYN, SYN_REPORT, 0);
    	// Release the key To prevent key repeat
    	if (-1 == retValue) {
			debugPrint("changeEvent::changeOperation checkpoint 2 error");
		} else {
    	    retValue = writeChangeEvent(inputEventType, outEvent, 0);
    		if (-1 == retValue) {
				debugPrint("changeEvent::changeOperation checkpoint 3 error");
			} else {
    		    retValue = writeChangeEvent(EV_SYN, SYN_REPORT, 0);
    		}
    	}
		
    }
    return retValue;

}


int changeEvent::createOutputDevice(int createFd, const char *pInputDevName) {
	struct uinput_user_dev newDevice = {0} ;
	int retValue = -1;
	errno = 0;
	tracePrint("changeEvent::createOutputDevice start");
	if (nullptr != pInputDevName) {
		ioctl(createFd, UI_SET_EVBIT, EV_KEY);
		ioctl(createFd, UI_SET_KEYBIT, outEvent);

		size_t inputDevLen = strnlen(pInputDevName, UINPUT_MAX_NAME_SIZE);
		strncpy(newDevice.name, pInputDevName, inputDevLen);
		newDevice.id.bustype = BUS_USB;
		newDevice.id.vendor = 0x7777;
		newDevice.id.product = 0x7777;
		newDevice.id.version = 1;

		retValue = write(createFd, &newDevice, sizeof(newDevice));
		if (retValue >= 0) {
			retValue = ioctl(createFd, UI_DEV_CREATE);
			if (retValue < 0) {
				debugPrint("changeEvent::createOutputDevice ioctl error");
			}
		} else {
			debugPrint("changeEvent::createOutputDevice write error");
		}
	} else {
		tracePrint("changeEvent::nullpointer");
	}
	tracePrint("changeEvent::createOutputDevice end");
	return retValue;
}

int changeEvent::searchDeviceList(char *pDeviceName, struct input_id &pInputId) {
    int loopCnt = 0;
    bool retValue = -1;
	size_t checkLen = 0;
    for (loopCnt = 0; targetDevNameTable[loopCnt].pTargetDeviceName != nullptr; loopCnt++ ) {
		checkLen = strnlen(targetDevNameTable[loopCnt].pTargetDeviceName, UINPUT_MAX_NAME_SIZE);
        retValue = strncmp(pDeviceName, 
						   targetDevNameTable[loopCnt].pTargetDeviceName,
						   checkLen);
		if ((0 == retValue) && (pInputId.product == targetDevNameTable[loopCnt].detailDeviceType)){
//		if (0 == retValue) {
			cout <<"match device name = "<< pDeviceName << endl;
			cout <<" device.product = " << hex << pInputId.product << endl;
			targetEvent = targetDevNameTable[loopCnt].targetOpeNum;
			outEvent = targetDevNameTable[loopCnt].changeEventNum;
            break;
        }
    }
    return retValue;
}
int checkEventDevice(const struct dirent *pDir) {
    int retValue = 0;
    if (0 == (strncmp("event", pDir->d_name, 5))) {
        retValue = 1;
    }
    return retValue;
}
