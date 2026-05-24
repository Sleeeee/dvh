# Voltage Glitching lab instructions

## Overview

Here are the top-level instructions required to perform the Voltage Glitching lab. They are meant to give you the necessary information to get you started by yourself. If you ever feel stuck or need any help, you can read the lab's complete [walkthrough](./walkthrough.md) that will provide step-by-step instructions.

## Requirements

Below is a check list of all the tools and skills that you need in order to perform the lab. Everything is mandatory unless stated otherwise, except the skill set items. It is absolutely not a problem if you do not have some of the listed skills, but fundamentals of these topics is always nice to have. Anyway, you will learn more about those in the context of the lab. Feel free to do your own research whenever you lack understanding of any of the topics.

### Materials and equipment

- Computer
- DVH board with latest firmware (if not, you will need to [flash](../../tools/flash.md) it)
- Glitcher, we will use a Pico with [MicroPython](../../tools/micropython.md) for this lab
- Oscilloscope (optional)
- Micro-USB cable and 5-12V power supply
- Micro-USB to USB/USB-C cable (depending on available ports)
- Dupont cables (recommended) or regular wires
- 5.1K ohm resistor (tested) or any other value ranging from 1K to 10K

### Software

- Linux Physical or Virtual Machine (recommended)
- [MicroPython](../../tools/micropython.md) development environment

### Skill set

- Principles of electronics
- Some knowledge of MicroPython and the Raspberry Pi Pico
- Basic familiarity with oscilloscopes

## Objective

The goal of this lab is to exploit the ATTiny of the DVH board. A security check is performed on the ATTiny to verify the system's integrity, but unfortunately, it never passes. Your task is to perform a voltage glitching attack on the ATTiny to bypass the security check and obtain the flags.

This lab contains 2 flags, formatted like so : `DVH{th1s_1s_4_f4k3_fl4g_385c951916}`.

## Instructions

### Reset the lab

> This step is off-limits in the scope of the lab.

Since the ATTiny is independent from the STM32, it needs to go through an initialization process to make sure the environment is properly setup, and the ATTiny is in a valid state. In order to initialize or reset the lab, press the reset button while powering on the board. The LED will blink, and if everything goes smoothly, it will turn off. If it stays on, an error has occured, meaning the reset has not been completed. Please retry until you end up with an LED turned off.

### Dump the firmware

> Because the difficulty of this lab is already higher than the others, the [firmware](./assets/attiny.bin) is provided. If you have time, it would be a good exercise to attempt the dump yourself.

Dump a copy of the firmware running on the ATTiny to understand where the security check lives and how to detect it.

> Hint : Is there a specific trigger signal that is transmitted right before the security check ?

### Set up

Find the right spots on the board to perform the glitch. You will need the trigger from last step, and some sort of method to pull the voltage down.

> Hint : Is there a specific component near the ATTiny's power supply that can help drop the voltage easily ?

### Perform glitch

Find suitable parameters to bypass the security check by glitching the ATTiny. You can use this [MicroPython script](./assets/glitcher.py) to configure the parameters and drop the voltage.

> Hint : Try to interpret the outputs. What happens when you drop the voltage too long ? What if you do not drop it long enough ? Also, the `autosweep` and `autosweep2d` functions may help with scanning ranges automatically.
