# Universal Asynchronous Receiver/Transmitter (UART)

## Overview

The UART protocol, or Universal Asynchronous Receiver/Transmitter, is one of the most common standards in the embedded world, enabling serial communication between devices. It transports any kind of data and highly varies depending on the context. Often times, UART interfaces are used to transport text and provide shell interfaces for humans to communicate with the hardware and perform general purpose actions such as testing and debugging. UART interfaces will regularly expose Linux shells, as it is the predominant OS in the IOT world.

A UART interface uses two signal lines to enable two-way communications :

- RX : Receiver line
- TX : Transmitter line

It is important to note that the TX line of device A will need to be wired to the RX line of device B.

## Exploit

System vulnerabilities often rise not from the UART protocol itself, but rather its implementation inside the firmware. On IOT devices, you are very likely to discover that UART interfaces are left unguarded and that they are very verbose. If an intruder gains access to a UART interface, they might be able to :

- Gain an unauthorized shell access (sometimes even root).
- View sensitive system logs.
- Execute unauthorized hardware instructions.

## Mitigations

As stated before, the exploits performed on UART interfaces are often caused by misconfigurations and the lack of proper access controls. Mitigations may seem pretty obvious, but here are a couple of actions that can reduce the attack surface of a UART interface :

- Securing physical access to the device.
- Enforcing proper authentication and access controls.
- Encrypting UART traffic.

## Practice

You can now gain hands-on experience with UART by completing the [lab](./instructions.md) for this section.
