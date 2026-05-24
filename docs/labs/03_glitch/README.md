# Voltage glitching

## Overview

Fault injection is a hardware hacking practice that aims to destabilize a device to bypass its security controls. Voltage glitching, or power glitching, is a common form of fault injection, where an attacker controls the power supply to trigger unstable behaviors.

When dropping the power supply to 0, the chip will obviously power off. However, when exposed to extremely fast (fraction of a millisecond) voltage drops at very precise timings (down to the CPU instruction), it is possible to perform a glitch that causes the CPU to skip its current instruction and continue its execution.

Skipping a single instruction can be particularly useful when encountering contidional branches (`if ... then ... else`). For example, if we have some sort of password check :

```c
if (user_input == expected_password) {
    welcome()
} else {
    get_out()
}
```

If we manage to skip the password verification using a voltage glitch, we would be able to call the `welcome()` function without knowing the password.

## Exploit

Voltage glitching is extremely powerful (and very hard to master). Because it is targeted at the silicon itself, any chip is virtually vulnerable to such attacks. Compromising the integrity of a system with voltage glitching may result to :

- Bypass password or security checks.
- Bypass potential Readout Protection measures.
- Obtain unexpected chip behavior, and potentially infinite possibilities.

The difficulty of power glitching resides in the strictness of finding a relevant width (to prevent the chip from passing out) and delay (to reliably attack the right instruction). Finding those typically requires a lot of trial and error, performing side-channel analysis (footprinting the chip to reliably detect when to trigger to glitch), and a bit of luck.

Some powerful tools specialized in glitching exist on the market, such as [ChipWhisperer](https://chipwhisperer.readthedocs.io/en/latest/getting-started.html) and the [PicoGlitcher](https://fault-injection-library.readthedocs.io/en/latest/overview/).

## Mitigations

There are common methods to protect a system against power glitching :

- Decoupling capacitors stabilize the power supply, making the chip sustain even with voltage drops. It is however possible for an attacker to desolder the chip.
- Brown Out Detectors (BOD) monitor the microcontroller's power and place the core in a safe state when the voltage falls below a certain level.
- Compare the values twice to harden the check itself.
- Add random time delays before sensitive operations for unpredictability.

Voltage glitching also suffers from the fact that it is a destructive practice that may damage, or permanently fry the chip. Depending on the target, it may also be frustrating because of its volatile nature, CPU clocks may drift, and an exploit's success rate may vary depending on the setup.

## Practice

You can now gain hands-on experience with Voltage Glitching by completing the [lab](./instructions.md) for this section.
