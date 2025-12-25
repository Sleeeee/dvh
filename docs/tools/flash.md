# Flashing the board with the debugprobe Pico and OpenOCD

In order to move code to any circuit, you need to go through a process called flashing. By connecting your debugger on a specific interface, you will be able to talk to the chip and program it so it executes the instructions written in your firmware. This document will help you set up the environment for flashing code to the STM32 chip and get the board running.

## Required materials and equipment

- Computer
- DVH board
- Debugger ([debugprobe Pico](debugprobe.md), ST-Link or something else)
- Micro-USB cable and 5-12V power supply
- Micro-USB to USB/USB-C cable (depending on your computer's ports)
- 3 Dupont cables (recommended) or regular wires

## Install and configure OpenOCD

There are several required steps to have a working flashing environment. First is to install [OpenOCD](https://openocd.org/pages/getting-openocd.html), which is the software that will allow us to communicate with the boards :

```
sudo apt update
sudo apt install openocd
```

Once this is done, we need to create a configuration file for our setup. Please make sure that `/usr/share/openocd/scripts/interface/cmsis-dap.cfg` and `/usr/share/openocd/scripts/target/stm32f1x.cfg` do exist on your file system. If you are on Windows, you will need to modify those paths. Here is the configuration that we can write to a file named `openocd.cfg` :

```
# Source debugprobe interface config file
source /usr/share/openocd/scripts/interface/cmsis-dap.cfg
transport select swd

# Source target (STM32F103C8T6) config file
source /usr/share/openocd/scripts/target/stm32f1x.cfg

# Flashing at 5MHz
adapter speed 5000
```

> It is recommended to create a dedicated directory for this process, where you will place all the files required by this document.

## Download firmware

Depending on the lab you want to practice on, you will be prompted to download a specific piece of firmware from this repository. All of them can be found under the [Releases](https://github.com/Sleeeee/dvh/releases) section.

## Wire boards

You will now need to connect the debugger to the DVH board using its SWD (Serial Wire Debug) interface. You only need three pins to be connected, SWDCLK, SWDIO, and a common ground. Here is a valid pinout for the connection using the Pico debugprobe :

| DVH    | Pico |
|--------|------|
| SWDCLK | GP2  |
| SWDIO  | GP3  |
| GND    | GND  |

You may need to take a look at the pinout for both the DVH board and the [Raspberry Pi Pico](https://www.raspberrypi.com/documentation/microcontrollers/pico-series.html).

Once this is done, you can power on both boards, while connecting the debugger to your computer. Both boards should have an LED turned on to display that they are correctly receiving current.

## Flash firmware

The DVH board is now ready to be flashed. From your dedicated directory, execute this command, changing filenames if needed :

```
openocd -f openocd.cfg -c "program firmware.bin 0x08000000 verify reset exit"
```

> To make the process simpler for next time, do not hesitate to create a shell script containing this command.

You have now successfully flashed firmware on the DVH board !
