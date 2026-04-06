# Setting up MicroPython on the Raspberry Pi Pico

[MicroPython](https://micropython.org/) is a Python environment that can be used for high-level embedded programming. It provides an interpreter to execute scripts interactively, which is very convenient for quickly writing scripts and prototyping. The Raspberry Pi Pico, being a development board, is a great candidate for MicroPython.

## Required materials and equipment

- Computer
- Raspberry Pi Pico
- Micro-USB to USB/USB-C cable (depending on available ports)

## Download and flash firmware

First step is to download the latest MicroPython firmware for the [Pico](https://micropython.org/download/RPI_PICO/) or [Pico 2](https://micropython.org/download/RPI_PICO2/).

You can then flash it on the Pico by holding its `BOOTSEL` button when powering it on, and copying the UF2 file to the Pico peripheral. This step is explained in greater details in the [debugprobe](./debugprobe.md) setup.

## Write code

This step is not be covered in detail in this document. The official [documentation](https://docs.micropython.org/en/latest/) will be your best friend. The `machine`[library](https://docs.micropython.org/en/latest/library/machine.html) is a good starting point for controlling the hardware.

## Run scripts

The simplest way is to use the [Thonny](https://thonny.org/) IDE, which can be set up for Pico development by reading [this guide](https://projects.raspberrypi.org/en/projects/getting-started-with-the-pico/3).

If you prefer using the command line however, this is also possible with utilities like [rshell](https://github.com/dhylands/rshell) :

```bash
$ rshell -p /dev/ttyACM0
Using buffer-size of 32
Connecting to /dev/ttyACM0 (buffer-size 32)...
Trying to connect to REPL  connected
...
> cp script.py /pyboard/
Copying 'script.py' to '/pyboard/script.py' ...
> repl
Entering REPL. Use Control-X to exit.
MicroPython v1.27.0 on 2025-12-09; Raspberry Pi Pico with RP2040
Type "help()" for more information.
>>> import script
```
