# Inter-Integrated Circuit (I²C)

## Overview

I²C (pronounced "I-squared-C" or "I-two-C") is a serial communication bus that is used to send data between integrated circuits in a master-slave architecture. This type of interface is implemented on most microcontrollers that can act as either a master or a slave, and a lot of chips such as sensors, ROMs, transmitters, and so many more, can act as an I²C slave and receive commands from a master using their custom protocol lying on top of the I²C communication bus.

One advantage of I²C is that it only needs two signal lines :

- SCL : Serial Clock line
- SDA : Serial Data line

Multiple slaves can be wired on the same I²C bus. Because there is no dedicated Chip Select (CS) signal, the I²C master starts the communication by sending a message containing the slave address (7 bits) and the requested operation type (Read/Write - 1 bit). All slaves listen, but only respond if the slave address transmitted corresponds to their own.

> All slaves should have different identifiers to avoid collisions.

### M24C02

In the case of this lab, we will be looking at a specific I²C implementation : the M24C02 EEPROM. An EEPROM, or Electrically Erasable Programmable Read-Only Memory, is a simple data storage device. The M24 family are tiny chips (designed by ST Microelectronics, the same company behind STM32 microcontrollers) that can be placed in IOT systems to store hardcoded data (a password for example).

> To get a deeper understanding of the M24C02, please read the [datasheet](https://www.st.com/en/memories/m24c02-w.html).

## Exploit

Because I²C enables communication between circuits by exposing an unencrypted bus, the attack surface is quite large, and can be exploited by attackers in multiple ways, for example :

- Sniff communications to reverse engineer the system's behavior.
- Impersonate the master and control the slave.
- Impersonate the slave and give wrong data to the master.
- Replay arbitrary communications.

Depending on the application, exploits can become very specific. For example, with an EEPROM, we could read the stored data to discover sensitive strings or values.

## Mitigations

Although the I²C bus itself is inherently insecure, there are some techniques that can help reduce the attack surface :

- Encrypt the payloads (you cannot encrypt the I²C transport itself, but you can encrypt the data it transports).
- Authenticate messages (MAC) to validate integrity and sender validity.
- Include a nonce against replay attacks.
- Avoid exposing easily accessed sensitive I²C lines.

In IOT, targets implement their own commmunication protocol over I²C by defining how they should process the payloads transported by I²C. This means that the security of the communications will highly depend on the target model.

## Practice

You can now gain hands-on experience with I²C and the EEPROM by completing the [lab](./instructions.md) for this section.
