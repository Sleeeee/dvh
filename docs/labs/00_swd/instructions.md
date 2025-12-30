# SWD lab instructions

## Overview

Here are the top-level instructions required to perform the SWD lab. They are meant to give you the necessary information to get you started by yourself. If you ever feel stuck or need any help, you can read the lab's complete [walkthrough](./walkthrough.md) that will provide step-by-step instructions.

## Requirements

Below is a check list of all the tools and skills that you need in order to perform the lab. Everything is mandatory unless stated otherwise, except the skill set items. It is absolutely not a problem if you do not have some of the listed skills, but fundamentals of these topics is always nice to have. Anyway, you will learn more about those in the context of the lab. Feel free to do your own research whenever you lack understanding of any of the topics.

### Materials and equipment

- Computer
- DVH board with latest firmware (if not, you will need to [flash](../../tools/flash.md) it).
- Debugger ([debugprobe](../../tools/debugprobe.md), ST-Link or something else)
- Micro-USB cable and 5-12V power supply
- Micro-USB to USB/USB-C cable (depending on your computer's ports)
- 3 Dupont cables (recommended) or regular wires

### Software

- OpenOCD (dedicated section in [flashing guide](../../tools/flash.md)).
- Disassembler/decompiler ([Ghidra](https://ghidralite.com/), Binary Ninja, IDA Pro or any other). Ghidra is strongly recommended if you have zero to little experience with that kind of software, because it is the one used in the [walkthrough](./walkthrough.md).
- Linux Physical or Virtual Machine (recommended)

### Skill set

- Principles of electronics
- Basic understanding of assembly and disassembly, compiling and decompiling
- C fundamentals

## Objective

The goal of this lab is to exploit the SWD interface of the DVH board. The manufacturer has forgotten to enable Readout Protection, which makes the board vulnerable to external debugging. Your task is to access the firmware that is currently running on the MCU, and perform a bit of reverse engineering to find the flags.

This lab contains 3 flags, formatted like so : `DVH{th1s_1s_4_f4k3_fl4g_385c951916}`.

## Instructions

### Initial access

Plug in a circuit debugger to the DVH board. Using a correct OpenOCD configuration, dump the firmware to your local machine.

> As a reminder, the main chip is a STM32F103C8T6, its base address is `0x08000000` and its flash size is `0x10000` (64kB).

### File analysis

Inspect the firmware `.bin` with standard Linux commands and see what you can find.

> Hint : What command gives you generic information about the file ? How can you read the ASCII-printable text contained in that binary ? Can you retrieve a flag already ?

### Static analysis

Open the binary in your favorite decompiler (Ghidra or other). Don't forget to select the correct architecture type and base address (`0x08000000`). Navigate through the disassembly and retrieve the remaining flags. Note that this step is harder than the rest of this lab, so you may want to have a look at the [walkthrough](./walkthrough.md) to explain the techniques that can be used here.

> Hint : Do not scroll through all the program. By now, you should have an anchor point that you can navigate to. All three flags are contained in the same function.
