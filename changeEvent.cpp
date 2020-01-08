#include <error.h>
#include <iostream>
#include <string.h>
#include <linux/input.h>
#include <pthread.h>
#include <sched.h>
#include "logitechMouseProduct.hpp"
#include "changeEvent.hpp"
using namespace std;

TARGET_TBL_T targetDevNameTable[] = {
	{ "Logitech USB Receiver Consumer Control", MX_REVOLUTION_1, 0x00D9, BTN_TASK },
	{ "Logitech USB Receiver Consumer Control", MX_REVOLUTION_2, 0x00D9, BTN_TASK },
	{ "Logitech USB Receiver Consumer Control", MX_REVOLUTION_3, 0x00D9, BTN_TASK },
	{ "Logitech USB Receiver Consumer Control", MX_REVOLUTION_4, 0x00D9, BTN_TASK },
	{ "Logitech USB Receiver Consumer Control", MX_REVOLUTION_5, 0x00D9, BTN_TASK },
	{ "Logitech MX Revolution Mouse Consumer Control", MX_REVOLUTION_5, 0x00D9, BTN_TASK },
	{ "Logitech USB Receiver Consumer Control", VX_REVOLUTION, 0x00D9, BTN_TASK },
	{ "MX Master 2S Keyboard", MX_MASTER2S_KEYBOARD, 0x001D, BTN_TASK },
	{ "MX Master 2S Keyboard", MX_MASTER2S_KEYBOARD, 0x000F, 0x0000 },
	{ "MX Master 2S Keyboard", MX_MASTER2S_KEYBOARD, 0x0038, 0x0000 },
	{ nullptr, UNKOWN_MOUSE, 0x0000, 0x0000 }
};

changeEvent::changeEvent(const char *pInputPath) {
	tracePrint("changeEvent::changeEvent start");
	int retValue = -1;
	outputFd = -1;
	memset(deviceName, 0x00, sizeof(deviceName));
	if (nullptr != pInputPath) {
		inputFd = open(pInputPath, O_RDWR);
		if (0 <= inputFd) {
			retValue = ioctl(inputFd, EVIOCGNAME(sizeof(deviceName)), deviceName);
		} else {
			debugPrint("changeEvent::changeEvent checkpoint 1 error");
		}
		if (0 <= retValue) { 
			retValue = ioctl(inputFd, EVIOCGID, &inputId);
		} else {
			debugPrint("changeEvent::changeEvent checkpoint 2 error");
		}
		if (0 <= retValue) { 
			retValue = searchDeviceList(deviceName, inputId);
		} else {
			debugPrint("changeEvent::changeEvent checkpoint 3 error");
		}
		if (0 == retValue) {
			outputFd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
		} else {
			tracePrint("Target DeviceName was not found");
		}
		if (-1 != outputFd) {
			createOutputDvRet = createOutputDevice(outputFd, 
												   deviceName);
			ioctl(inputFd, EVIOCGRAB, 1);
			if (createOutputDvRet != -1) {
				changeEventSts = true;
			} else {
				close(outputFd);
			}
		}
		if (0 > retValue) {
			retValue = close(inputFd);
			debugPrint("searchDeviceList between createOutputDevice error");
		}
	} else {
		debugPrint("pInputPath = nullptr");
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
	__u16 inputEventCode;

	cout << "DeviceName = " << deviceName << endl;
	while(true) {
		cout << "debug checkpoint 1" << endl;
		readResult = read(inputFd, &inputEvent, sizeof(inputEvent));
		if (-1 == readResult) {
			debugPrint("read error do stop");
			//break;
		} else if (readResult != sizeof(inputEvent)) {
			printf("size error readResult = %d", readResult);
			break;
		} else if ((EV_KEY == inputEvent.type) ||
				   (EV_REL == inputEvent.type) ||
				   (EV_MSC == inputEvent.type) ||
				   (EV_SYN == inputEvent.type)) {
			if ((EV_KEY == inputEvent.type) && (targetEvent == inputEvent.code)) {
				inputEventCode = outEvent;				
			} else {
				inputEventCode = inputEvent.code;
			}
			readResult = writeChangeEvent(inputEvent.type, inputEventCode, inputEvent.value);
			cout << "debug checkpoint 2" << endl;
			if (-1 == readResult) {
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
		if (-1 == sched_yield()) {
			break;
		}
		cout << "debug checkpoint 3" << endl;
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
	writeEvent.time.tv_usec = 0;
	return write(outputFd, &writeEvent, sizeof(writeEvent));
}
int changeEvent::changeOperation(__u16 inputEventType, __u16 inputEventCode, __u16 inputEventValue) {
    int retValue = -1;
    // Press 0x00D9 -> Press mouse button 12
    retValue = writeChangeEvent(inputEventType, inputEventCode, inputEventValue);
	cout << "inputEvent.type = " << inputEventType << endl;
	cout << "inputEvent.code = " << inputEventCode << endl;
	cout << "inputEvent.value = " << inputEventValue << endl;

    if (-1 == retValue) {
		debugPrint("changeEvent::changeOperation checkpoint 1 error");
	} else {
//		retValue = writeChangeEvent(EV_SYN, SYN_REPORT, 0);
	}
	if (-1 == retValue) {
		debugPrint("changeEvent::changeOperation checkpoint 2 error");
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
		} else {
			debugPrint("changeEvent::createOutputDevice write error");
		}
		if (retValue < 0) {
			debugPrint("changeEvent::createOutputDevice ioctl error");
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
		if ((0 == retValue) &&
			(pInputId.product == targetDevNameTable[loopCnt].detailDeviceType)){
			cout <<"match device name = "<< pDeviceName << endl;
			cout <<" device.product = " << hex << pInputId.product << endl;
			targetEvent = targetDevNameTable[loopCnt].targetOpeNum;
			outEvent = targetDevNameTable[loopCnt].changeEventNum;
            break;
        }
    }
    return retValue;
}
