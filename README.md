All named products and descriptions are trademarks and/or registered trademarks of the respective manufacturers and are accepted as protected
# mbmex
This repository is program of many button mouse extended.

# process

# build

```
$ mkdir build
$ cmake ..
$ make
$ sudo make install
```

mbmex will be installed to /usr/local/bin.
mbmex.service will be installed to /etc/systemd/system.

# start program
```
$ ./start_mbmex.sh
```

# searchEvent
SearchEvent is purpose to find devicename and button number.
You can add the devicename and button number to logitechMouseProduct.hpp.
