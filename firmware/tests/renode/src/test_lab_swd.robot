*** Settings ***
Resource              ${RENODEKEYWORDS}
Library               OperatingSystem

Suite Setup           Setup
Suite Teardown        Teardown
Test Teardown         Teardown

*** Variables ***
${PLATFORM}           ${CURDIR}/../platforms/dvh.repl
${ELF}                ${CURDIR}/../../../build/firmware.elf
${HOOKS_SCRIPT}       ${CURDIR}/utils/hooks.py

*** Test Cases ***
Verify Lab SWD
  [Documentation]     Assert flags appear in memory

  File Should Exist   ${ELF}    msg=Error: Firmware ELF not found at ${ELF}

  Execute Command     mach create "dvh_test"
  Execute Command     machine LoadPlatformDescription @${PLATFORM}
  Execute Command     sysbus LoadELF @${ELF}

  # Load custom python helper functions
  Execute Command     include @${HOOKS_SCRIPT}

  

  # Flag one hook (entry of Lab_SWD_Solve_Flag_Two)
  # Read buffer address from R2 register (third argument in Lab_SWD_Solve_Flag_Two)
  Execute Command     sysbus.cpu AddHook `sysbus GetSymbolAddress "Lab_SWD_Solve_Flag_Two"` "check_register_buffer(2, 64, 'LAB_SWD_FLAG_ONE')"

  # Flag two hook (entry of Lab_SWD_Solve_Flag_Three)
  # Same logic as flag one hook
  Execute Command     sysbus.cpu AddHook `sysbus GetSymbolAddress "Lab_SWD_Solve_Flag_Three"` "check_register_buffer(2, 64, 'LAB_SWD_FLAG_TWO')"

  # Flag three hook (entry of Lab_SWD_Loop)
  # Use an entry hook to capture the buffer address, but check its value only once the function returns
  Execute Command     sysbus.cpu AddHook `sysbus GetSymbolAddress "Lab_SWD_Solve_Flag_Three"` "capture_buffer_address(2)"
  Execute Command     sysbus.cpu AddHook `sysbus GetSymbolAddress "Lab_SWD_Loop"` "check_captured_buffer(64, 'LAB_SWD_FLAG_THREE')"

  Execute Command     start
  Sleep               1s
