import sys
import System
from Antmicro.Renode.Logging import Logger, LogLevel
from Antmicro.Renode.Peripherals.CPU import ICPU

_captured_ptr = None
_return_hook_attached = False

def read_string(bus, address, size=64):
    byte_array = bus.ReadBytes(address, size)
    return "".join([chr(b) for b in byte_array if 32 <= b <= 126])

def get_reg_value(cpu, offset_reg):
    reg_val = cpu.GetRegisterUnsafe(offset_reg)
    try:
        return System.Convert.ToUInt64(reg_val)
    except:
        val_str = str(reg_val).strip()
        return int(val_str, 16) if "0x" in val_str.lower() else int(val_str)

def check_flag(cpu, symbol, flag, ptr):
    actual_data = read_string(cpu.Bus, ptr)

    start_idx = actual_data.find("DVH{")
    if start_idx == -1:
        Logger.Log(LogLevel.Error, "Failed: No 'DVH{' found in memory, expected '%s'" % flag)
        return
    end_idx = actual_data.find("}", start_idx) + 1
    discovered_content = actual_data[start_idx:end_idx]

    match = False
    if discovered_content == flag:
        match = True

    if match:
        Logger.Log(LogLevel.Error, "Success: %s found! ('%s')" % (symbol, discovered_content))
    else:
        Logger.Log(LogLevel.Error, "Failed: %s content mismatch" % flag)
        Logger.Log(LogLevel.Error, "    Expected: '%s'" % flag)
        Logger.Log(LogLevel.Error, "    Found: '%s'" % discovered_content)

def hook_flag_one(cpu, address):
    ptr = get_reg_value(cpu, 2)
    check_flag(cpu, "LAB_SWD_FLAG_ONE", "DVH{pl41n_text_d3t3ct1v3_fb242a8175}", ptr)

def hook_return_from_three(cpu, address):
    global _captured_ptr
    if _captured_ptr is not None:
        check_flag(cpu, "LAB_SWD_FLAG_THREE", "DVH{y0ur_b1n_1s_l34k1ng_c8ffc55f5e}", _captured_ptr)

def hook_flag_two(cpu, address):
    global _captured_ptr, _return_hook_attached
    ptr = get_reg_value(cpu, 2)
    check_flag(cpu, "LAB_SWD_FLAG_TWO", "DVH{h1d1ng_b3h1nd_x0r_4e2f398b6b}", ptr)
    _captured_ptr = ptr

    lr_addr = get_reg_value(cpu, 14) & 0xfffffffe
    if not _return_hook_attached:
        cpu.AddHook(lr_addr, hook_return_from_three)
        _return_hook_attached = True

try:
    sysbus = self.Machine.SystemBus
    cpu = list(self.Machine.GetPeripheralsOfType[ICPU]())[0]
    cpu.AddHook(sysbus.GetSymbolAddress("Lab_SWD_Solve_Flag_Two"), hook_flag_one)
    cpu.AddHook(sysbus.GetSymbolAddress("Lab_SWD_Solve_Flag_Three"), hook_flag_two)
except Exception as e:
    Logger.Log(LogLevel.Error, "Error during setup: " + str(e))
