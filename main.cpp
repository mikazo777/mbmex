#include <dirent.h>
#include <cstring>
#include <pthread.h>
#include "main.hpp"
#include "mbmexBase.hpp"
#include "changeEvent.hpp"
#include "debug.hpp"


int main(void) {
	int retValue = -1;
    int devNum = 0;
    int lpCnt = 0;
	int chgEvtCnt = 0;
    struct dirent **ppNameList;
	
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

            printf("filename = %s\n", filename);
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
			pthread_t handle;  // Thread handle
			for (lpCnt = 0; lpCnt < chgEvtCnt; lpCnt++ ) {
				retValue = pthread_create(&handle,
										  nullptr,
										  changeEvent::planchThread,
										  pChgEvt[lpCnt]);
			}
			pthread_join(handle, nullptr);
		} else {
			debugPrint("Device not found");
		}
	}
	return retValue;
}

