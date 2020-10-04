# umbrella bot motor controller

At the moment the application is build for and tested on stm32f4-discovery board
just because I have got one.


## Setup Development Environment

Install the `probe-run`. On Ubuntu 20.04, you will have to install `libusb-1.0.0-dev` package.

```sh
sudo apt install libusb-1.0.0-dev
cargo install probe-run --git https://github.com/knurling-rs/probe-run.git --branch main -f --features defmt
```

Add the target to the toolchain.

```sh
rustup target add thumbv7em-none-eabihf
```

Then create the `.cargo/config` file with the following content:

```toml
[target.'cfg(all(target_arch = "arm", target_os = "none"))']
runner = "probe-run --chip STM32F407VGTx"

[build][target.'cfg(all(target_arch = "arm", target_os = "none"))']
       runner = "probe-run --chip STM32F407VGTx --defmt"
       rustflags = [
           "-C", "link-arg=-Tlink.x",
           "-C", "link-arg=-Tdefmt.x",
       ]
       
       [build]
       target = "thumbv7em-none-eabihf"

target = "thumbv7em-none-eabihf"
```

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

