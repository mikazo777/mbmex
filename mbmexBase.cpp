#include <cstring>
#include "mbmexBase.hpp"

int checkEventDevice(const struct dirent *pDir) {
    int retValue = 0;
    if (0 == (strncmp("event", pDir->d_name, 5))) {
        retValue = 1;
    }
    return retValue;
}

