#include <cstring>
#include <pthread.h>
#include <unistd.h>
#include <thread>
#include "mbmexBase.hpp"
#include "changeEvent.hpp"
#include "debug.hpp"


int main(void) {
	int retValue = -1;
    int devNum = 0;
    int lpCnt = 0;
	int chgEvtCnt = 0;
    struct dirent **ppNameList;
	
	while(true) {
		devNum = scandir(devInputDir, &ppNameList, checkEventDevice, versionsort);
		if (0 < devNum) {
			changeEvent *pChgEvt[devNum];
			for (lpCnt = 0; lpCnt < devNum; lpCnt++) {
				char filename[sizeof(devInputDir) + sizeof((ppNameList[lpCnt])->d_name)];
				memset(filename, 0x00, sizeof(filename));
				snprintf(filename, 
						 sizeof(filename),
						 "%s/%s", 
						 devInputDir,
						 (ppNameList[lpCnt])->d_name);

				debugParamCharPrint("filename", filename);
				pChgEvt[chgEvtCnt] = new changeEvent(filename);
				if (nullptr != pChgEvt[chgEvtCnt]) {
					if (false == pChgEvt[chgEvtCnt]->changeEventSts) {
						delete pChgEvt[chgEvtCnt];
					} else {
						chgEvtCnt ++;
					}
				}
			}
			if (0 < chgEvtCnt) {
				thread changeEventThreads[chgEvtCnt];
				for (lpCnt = 0; lpCnt < chgEvtCnt; lpCnt++ ) {
					changeEventThreads[lpCnt] = thread(changeEvent::planchThread,
													   pChgEvt[lpCnt]);
				}
				for (lpCnt = 0; lpCnt < chgEvtCnt; lpCnt++ ) {
					changeEventThreads[lpCnt].join();
				}
			} else {
				debugPrint("Device not found");
				sleep(5);
				continue;
			}
		}
	}
	return retValue;
}

