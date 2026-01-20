*** Settings ***
Resource              ${RENODEKEYWORDS}
Library               OperatingSystem

Suite Setup           Setup
Suite Teardown        Teardown
Test Teardown         Teardown

*** Variables ***
${PLATFORM}           ${CURDIR}/../platforms/dvh.repl
${ELF}                ${CURDIR}/../../../build_simulation/firmware.elf
${HOOKS_SCRIPT}       ${CURDIR}/utils/hooks.py

*** Test Cases ***
Verify Lab SWD
    [Documentation]    Verify flags in memory match ELF symbols
    File Should Exist  ${ELF}    msg=Firmware ELF not found

    # Setup
    Execute Command    mach create "dvh_test"
    Execute Command    machine LoadPlatformDescription @${PLATFORM}
    Execute Command    sysbus LoadELF @${ELF}
    
    # Config
    Create Log Tester  0
    Execute Command    logLevel 0
    Execute Command    logLevel 3 sysbus.nvic

    # Run
    Execute Command    include @${HOOKS_SCRIPT}
    Execute Command    start
    
    # Verify
    Wait For Log Entry    Success: LAB_SWD_FLAG_ONE found!    timeout=5
    Wait For Log Entry    Success: LAB_SWD_FLAG_TWO found!    timeout=5
    Wait For Log Entry    Success: LAB_SWD_FLAG_THREE found!  timeout=5
