# Serial Wire Debug (SWD)

## Overview

During the process of manufacturing and writing firmware for embedded devices, hardware engineers always have the need for some sort of On-Chip Debugging (OCD) interface. Such an interface allows the engineer to program and analyze the microcontroller unit (MCU) by flashing and debugging firmware. In real life Printed Circuit Boards (PCB), you will encounter through-holes that expose an OCD interface, most commonly SWD or JTAG.

SWD stands for Serial Wire Debug, and has become a standard in the industry. It was created as a modern alternative to JTAG, requiring fewer connection signals. SWD needs only two signal lines to function :

- SWDIO : The bidirectional data line that can be used by both parties.
- SWDCLK : The clock signal maintained by the host.

## Exploit

With control over an SWD interface, you basically gain unlimited access to the MCU's memory. This is a gold mine for any attacker, because it would allow them to : 

- Dump the chip's firmware : Export all the code that is running on the chip, and extract potentially sensitive secrets or logic.
- Rewrite firmware : Develop a malicious version of the firmware and flash it on the chip, for example with a backdoor to grant some access to the attacker.
- Control instruction execution : Step through MCU instructions and bypass potential security or password checks in order to obtain unauthorized behavior from the device.

## Mitigations

Getting control over an SWD interface is not always as easy as plugging in a debugger and extracting all the chip's secrets. There are mitigation techniques that can be performed to prevent an external attacker to tamper with the SWD interfaces. The most common is enabling Readout Protection (RDP), which comes in different security levels :

- RDP level 0 : This level offers no protection at all, external access is fully open on debug interfaces.
- RDP level 1 : This level prevents reading or flashing the chip's memory via a debug interface. It is however possible to step down from level 1 to level 0, but this forces the memory to be mass-erased before effectively disabling the readout protection.
- RDP level 2 : This level build up on level 1 by adding additional protection, by irreversibly disabling debugging via SWD/JTAG and the bootloader. It is nt possible to step down to level 1 or 0, thus it is very likely to brick a chip by experimenting with level 2.

> It is always possible to step up from a low RDP level to a higher level.

While RDP is a well known mitigation, it is practically impossible to make the chip invulnerable to physical attacks, such as fault injection (forcing the MCU to skip the instruction that checks for RDP) or attaching a debugger before RDP is effectively locking down debugging. Other mitigation techniques include :

- Hiding SWD/JTAG interfaces on production boards.
- Debug Authentication (DA) to prevent unauthorized access to debugging.
- Secure Boot to prevent execution of unauthorized code.
- Flash encryption to mitigate the impact of someone reading the code.

## Practice

You can now gain hands-on experience with SWD by performing the [lab](./instructions.md) for this section.
