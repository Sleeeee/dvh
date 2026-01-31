# UART lab instructions

## Overview

Here are the top-level instructions required to perform the UART lab. They are meant to give you the necessary information to get you started by yourself. If you ever feel stuck or need any help, you can read the lab's complete [walkthrough](./walkthrough.md) that will provide step-by-step instructions.

## Requirements

Below is a check list of all the tools and skills that you need in order to perform the lab. Everything is mandatory unless stated otherwise, except the skill set items. It is absolutely not a problem if you do not have some of the listed skills, but fundamentals of these topics is always nice to have. Anyway, you will learn more about those in the context of the lab. Feel free to do your own research whenever you lack understanding of any of the topics.

### Materials and equipment

- Computer
- DVH board with latest firmware (if not, you will need to [flash](../../tools/flash.md) it).
- UART bridge ([debugprobe](../../tools/debugprobe.md) for example)
- Micro-USB cable and 5-12V power supply
- Micro-USB to USB/USB-C cable (depending on your computer's ports)
- 3 Dupont cables (recommended) or regular wires

### Software

- Serial terminal emulator ([Minicom](https://man7.org/linux/man-pages/man1/minicom.1.html) is the alternative used in the [walkthrough](./walkthrough.md))
- Password cracking tool ([John The Ripper](https://www.openwall.com/john/) or [Hashcat](https://hashcat.net/hashcat/) for example)
- Linux Physical or Virtual Machine (recommended)

### Skill set

- Principles of electronics
- Basic familiarity with command line interfaces (CLIs)
- Some knowledge of fundamental cryptographic concepts

## Objective

The goal of this lab is to exploit the UART interface of the DVH board. The shell's implementation in the firmware is still very verbose, and failed to properly ensure safe access controls to its features. Your task is to gain access to the UART shell and escalate as the root user to find the flags.

This lab contains 3 flags, formatted like so : `DVH{th1s_1s_4_f4k3_fl4g_385c951916}`.

## Instructions

### Initial access

Wire a UART bridge to the DVH board, and power it with lab 1 selected. Use a terminal emulator to gain access to the shell.

> The baud rate is a standard 115200 bauds.

### User escalation

Now that you are an anonymous user inside the system, find a way to authenticate as a real system user.

> Hint : What commands are available ? Also, some interesting debug logs may be printed over the UART interface, but when ?

### Root escalation

With standard user privileges, find a way to gain root access to obtain the third flag.

> Hint : What data can you gather on root ? Is there a way to make use of it ?
