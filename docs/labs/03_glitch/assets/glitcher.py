import machine
import rp2
import time
import sys

machine.freq(250_000_000)

# Hardware state machine (perform the actual glitch)
@rp2.asm_pio(set_init=rp2.PIO.OUT_LOW)
def glitcher():
    pull() # Pull DELAY
    mov(y, osr)
    pull() # Pull WIDTH
    mov(x, osr)

    # Wait for TRIGGER to go HIGH
    wait(0, pin, 0)
    wait(1, pin, 0)

    # Wait DELAY cycles
    label("delay_loop")
    jmp(y_dec, "delay_loop") 

    # Pull ATTiny VCC to GND for WIDTH cycles
    set(pins, 1)
    label("width_loop")
    jmp(x_dec, "width_loop") 

    set(pins, 0) # Release MOSFET

mosfet_pin = machine.Pin(13, machine.Pin.OUT)
trigger_pin = machine.Pin(17, machine.Pin.IN, machine.Pin.PULL_DOWN)
sm = rp2.StateMachine(0, glitcher, freq=250_000_000, in_base=trigger_pin, set_base=mosfet_pin)

delay_cycles = 0
width_cycles = 0

print("Welcome to DVH glitcher")
print("Commands: 'fire', 'test trigger', 'set delay <val>', 'set width <val>'")

while True:
    cmd = sys.stdin.readline().strip().lower().split()
    if not cmd:
        continue

    if cmd[0] == "set" and len(cmd) == 3:
        if cmd[1] == "delay":
            delay_cycles = int(cmd[2])
            print(f"[+] Delay set to {delay_cycles} cycles")
        elif cmd[1] == "width":
            width_cycles = int(cmd[2])
            print(f"[+] Width set to {width_cycles} cycles")

    elif cmd[0] == "test" and len(cmd) == 2 and cmd[1] == "trigger":
        print("[+] Listening on TRIGGER pin...")

        trigger_caught = [False] 
        def trigger_callback(pin):
            trigger_caught[0] = True

        trigger_pin.irq(trigger=machine.Pin.IRQ_RISING, handler=trigger_callback)
        while not trigger_caught[0]:
            pass

        trigger_pin.irq(handler=None)
        print("[!] Successfully captured TRIGGER signal")

    elif cmd[0] == "fire":
        print(f"[+] Launching glitch (delay: {delay_cycles}, width: {width_cycles})")
        # Start state machine
        sm.active(0) 
        sm.put(delay_cycles)
        sm.put(width_cycles)
        sm.active(1)

    elif cmd[0] == "autosweep" and len(cmd) == 3:
        start_delay = int(cmd[1])
        step = int(cmd[2])
        current_delay = start_delay

        print(f"[+] Starting auto-sweep (initial delay: {current_delay}, step: {step}, width: {width_cycles}. Press CTRL+C to stop.")

        try:
            while True:
                sm.active(0) 
                sm.put(current_delay)
                sm.put(width_cycles)
                sm.active(1) 

                if current_delay % 100 == 0:
                    print(f"[+] Scanning delay: {current_delay}")

                timeout_start = time.ticks_ms()
                triggered = False

                while time.ticks_diff(time.ticks_ms(), timeout_start) < 8000:
                    if trigger_pin.value() == 1:
                        triggered = True
                        break

                if not triggered:
                    print("[!] Timed out (TRIGGER is not rising)")
                    while trigger_pin.value() == 0:
                        pass 
                while trigger_pin.value() == 1:
                    pass

                current_delay += step

        except KeyboardInterrupt:
            print("\n[-] auto-sweep stopped.")

    elif cmd[0] == "autosweep2d" and len(cmd) == 5:
        # autosweep2d <start_delay> <step> <min_width> <max_width>
        start_delay = int(cmd[1])
        step = int(cmd[2])
        min_width = int(cmd[3])
        max_width = int(cmd[4])
        current_delay = start_delay

        print(f"[+] Starting auto-sweep 2D (delay: {start_delay}, step: {step}, width Range: {min_width} to {max_width}). Press CTRL+C to stop.")

        try:
            while True:
                # Sweep the widths for this delay
                for current_width in range(min_width, max_width + 1):
                    sm.active(0) 
                    sm.put(current_delay)
                    sm.put(current_width)
                    sm.active(1) 

                    timeout_start = time.ticks_ms()
                    triggered = False
                    while time.ticks_diff(time.ticks_ms(), timeout_start) < 8000:
                        if trigger_pin.value() == 1:
                            triggered = True
                            break

                    if not triggered:
                        print("[!] Timed out (TRIGGER is not rising)")
                        while trigger_pin.value() == 0:
                            pass 
                    while trigger_pin.value() == 1:
                        pass

                if current_delay % 50 == 0:
                    print(f"[+] Scanned delay: {current_delay} (widths {min_width}-{max_width})")
                current_delay += step

        except KeyboardInterrupt:
            print("\n[*] 2D AutoSweep stopped by user.")

    else:
        print("[!] Unknown command.")
