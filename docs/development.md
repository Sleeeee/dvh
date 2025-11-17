# Setting up the development environment

## VS Code extension

If you are using VS Code, there is an [extension](https://marketplace.visualstudio.com/items?itemName=raspberry-pi.raspberry-pi-pico) that automates a lot of the work.

## Manual installation

If like me, you are using another IDE like `neovim`, you might want to set up the environment manually. You can follow this procedure to complete the installation process which has slight variations the official [documentation](https://pip-assets.raspberrypi.com/categories/610-raspberry-pi-pico/documents/RP-008276-DS-1-getting-started-with-pico.pdf?disposition=inline). This walkthrough uses `apt` as a package manager, though you may need to adapt those commands to your OS and general environment.

### Configuring the toolchain

We start by creating a dedicated directory, then cloning the Pico SDK from GitHub and updating the dependencies : 

```
$ mkdir ~/pico && cd ~/pico
$ git clone https://github.com/raspberrypi/pico-sdk.git --branch master && cd pico-sdk
$ git submodule update --init
```

We then install the toolchain, including CMake, allowing us to manage the build process as well as a cross-compiler to translate C code to Pico-compatible machine code.

```
$ sudo apt update
$ sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential g++ libstdc++-arm-none-eabi-newlib
```

If you ever need to update your existing SDK installation, just re-run these commands :

```
$ cd ~/pico/pico-sdk
$ git pull
$ git submodule update
```

For the build process to work, we also need to set up this environment variable (and reload the source for it to take effect immediately). Keep in mind you may need to adapt the code to your shell :

```
echo "export PICO_SDK_PATH=~/pico/pico-sdk" >> ~/.bashrc
source ~/.bashrc
```

We are now ready to start building. You can now clone this repository (if you haven't done it yet), configure CMake and build the firmware for the Pico : 

```
$ git clone https://github.com/Sleeeee/dvh && cd dvh
$ mkdir build && cd build
$ cmake ..
$ make
```

The UF2 files can be found in the `/build/src/` directory.

### Flashing firmware

 You can flash any `.uf2` file onto the Pico by holding the BOOTSEL button while wiring it to your machine. It will then be mounted on your drive, and you can easily copy the UF2 on the Pico in your file manager, or from the terminal : 

```
$ cp ./path_to_uf2 /run/media/$USER/RPI-RP2/
```
