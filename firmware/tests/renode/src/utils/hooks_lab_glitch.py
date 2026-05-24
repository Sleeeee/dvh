import sys
from Antmicro.Renode.Logging import Logger, LogLevel
from Antmicro.Renode.Peripherals.CPU import ICPU

def hook_screen_write(cpu, address):
    str_ptr = cpu.GetRegister(0)
    raw = self.Machine.SystemBus.ReadBytes(str_ptr, 128)
    text = "".join([chr(b) for b in raw]).split('\0')[0]
    Logger.Log(LogLevel.Info, "Utils_Screen: %s" % (text))

try:
    sysbus = self.Machine.SystemBus
    cpu = list(self.Machine.GetPeripheralsOfType[ICPU]())[0]
    cpu.AddHook(sysbus.GetSymbolAddress("Utils_Screen_Write"), hook_screen_write)
    cpu.AddHook(sysbus.GetSymbolAddress("Utils_Screen_Fill_Write"), hook_screen_write)
except Exception as e:
    Logger.Log(LogLevel.Error, "Error during setup: " + str(e))
