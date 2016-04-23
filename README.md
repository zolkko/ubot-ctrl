# umbrella bot motor controller

At the moment the application is build for and tested on stm32f4-discovery board
just because I have got one.

## Build
``` bash
git submodule init
./bootstrap.sh
```
These will produce __ubot-ctrl.elf__ binary into __build__ subdirectory.

## Additional make command
* `make ubot-ctrl.bin` - output binary file
* `make dump` - output ubot-ctrl.S assembler file, which I find usefull if I want to check generated code
* `make size` - output to display size of sections
* `make gdbserver` - starts openocd GDBServer on port 3333
* `make gdbserver-stlink` - starts st-util GDBServer, port 3333
* `make gdbserver-jlink` - starts JLinkGDBServer, swd, default port
* `make cgdb` - opens cgdb session, gdbserver must be started by the time this command is executed
* `make flash` - uploads __ubot-ctrl.bin__ file into the controller
* `make flash-read` - download binary data from the controller

