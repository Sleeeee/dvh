_saved_buffer_address = None

def check_memory_content(address, length, symbol):
    """
    Reads the given memory address and assert it contains the same value as the given symbol name
    """
    data = self.Machine.SystemBus.ReadString(address, length)

    symbol_address = sysbus.GetSymbolAddress(symbol)
    symbol_value = sysbus.ReadString(symbol_address, length)

    if symbol_value not in data:
        raise Exception("Error: expected '%s' in memory. Found '%s'" % (expected_value, data))

def check_register_buffer(register_index, buffer_length, symbol):
    """
    Reads the given register and assert it contains the same value as the given symbol name
    """
    cpu = self.Machine.SystemBus.GetCPU()
    buffer = cpu.GetResiterUnsafe(register_index).Value

    check_memory_content(buffer, buffer_length, symbol)

def capture_buffer_address(register_index):
    """
    Saves the buffer address globally (used as an entry hook)
    """
    global _saved_buffer_address
    cpu = self.Machine.SystemBus.GetCPU()
    _saved_buffer_address = cpu.GetRegisterUnsafe(register_index).Value

def check_captured_buffer(buffer_length, symbol):
    """
    Reads the captured buffer and asserts it contains the same value as the given symbol name
    """
    global _saved_buffer_address
    if _saved_buffer_address is None:
        raise Exception("Error: expected an address to be captured before calling check_captured_buffer")

    check_memory_content(_saved_buffer_address, buffer_length, symbol)
