# I²C lab instructions

## Overview

Here are the top-level instructions required to perform the I²C lab. They are meant to give you the necessary information to get you started by yourself. If you ever feel stuck or need any help, you can read the lab's complete [walkthrough](./walkthrough.md) that will provide step-by-step instructions.

## Requirements

Below is a check list of all the tools and skills that you need in order to perform the lab. Everything is mandatory unless stated otherwise, except the skill set items. It is absolutely not a problem if you do not have some of the listed skills, but fundamentals of these topics is always nice to have. Anyway, you will learn more about those in the context of the lab. Feel free to do your own research whenever you lack understanding of any of the topics.

### Materials and equipment

- Computer
- DVH board with latest firmware (if not, you will need to [flash](../../tools/flash.md) it)
- UART bridge ([debugprobe](../../tools/debugprobe.md) for example)
- Logic analyzer (can be set up on a [Pico](../../tools/logic_analyzer.md))
- MicroPython board (can be set up on a [Pico](../../tools/micropython.md))
- Micro-USB cable and 5-12V power supply
- Micro-USB to USB/USB-C cable (depending on available ports)
- Dupont cables (recommended) or regular wires

> Note that completing this lab requires a UART bridge, a logic analyzer, and a MicroPython board, which can all be set up on a Raspberry Pi Pico, but not at the same time (with official firmware). This will require switching Pico firmware back and forth, but the lab is entirely doable with a single Pico. Acquiring standalone tools, or using multiple Pico boards can be an option for comfort.

### Software

- Serial terminal emulator ([Minicom](https://man7.org/linux/man-pages/man1/minicom.1.html) is the alternative used in the [walkthrough](./walkthrough.md))
- Logic analyzer software ([PulseView](https://sigrok.org/wiki/PulseView) recommended, setup is included in the logic analyzer [setup](../../tools/logic_analyzer.md))
- Linux Physical or Virtual Machine (recommended)

### Skill set

- Principles of electronics
- Basic familiarity with command line interfaces (CLIs)

## Objective

The goal of this lab is to exploit the I²C bus of the DVH board. Plain text communication with an EEPROM is performed to retrieve sensitive configuration variables such as passwords. Your task is to interact with I²C to gain root access to the UART shell to find the flags.

This lab contains 3 flags, formatted like so : `DVH{th1s_1s_4_f4k3_fl4g_385c951916}`.

## Instructions

### Reset the lab

> This step is off-limits in the scope of the lab.

Since the EEPROM is independent from the STM32, it needs to go through an initialization process to make sure the environment is properly setup, and the EEPROM is in a valid state. In order to initialize or reset the lab, press the reset button while powering on the board. The LED will blink, and if everything goes smoothly, it will turn off. If it stays on, an error has occured, meaning the reset has not been completed. Please retry until you end up with an LED turned off.

### Initial access

Gain access to the UART shell as practiced in the [corresponding lab](../01_uart/README.md).

### User escalation

Now that you are an anonymous user inside the system, find a way to authenticate as a real system user.

> Hint : Where does the password live ? How is it transmitted ?

### Root escalation

With standard user privileges, find a way to gain root access to obtain the third flag.

> Hint : Exploit eveything in the environment, the EEPROM might respond to other masters than the STM32. Completing this step requires two specific clues.
