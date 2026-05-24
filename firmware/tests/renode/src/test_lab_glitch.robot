*** Settings ***
Resource                    ${RENODEKEYWORDS}
Library                     OperatingSystem

Suite Setup                 Setup
Suite Teardown              Teardown
Test Teardown               Teardown

*** Variables ***
${PLATFORM}                 ${CURDIR}/../platforms/dvh.repl
${ELF}                      ${CURDIR}/../../../build_simulation/firmware.elf
${UART}                     sysbus.usart2
${SPI}                      sysbus.spi1
${HOOKS_SCRIPT}             ${CURDIR}/utils/hooks_lab_glitch.py

${FLAG_STRING}              Utils_Screen: DVH\{

*** Test Cases ***
Verify Lab Glitch
  [Documentation]           Verify glitching can be correctly performed on the ATTiny to print flags
  File Should Exist         ${ELF}      msg=DVH ELF not found

  # Setup
  Execute Command           mach create "dvh_test_glitch"
  Execute Command           machine LoadPlatformDescription @${PLATFORM}
  Execute Command           sysbus LoadELF @${ELF}

  # Config (select lab 0011)
  Create Log Tester         0
  Execute Command           sysbus.gpioPortB OnGPIO 3 True
  Execute Command           sysbus.gpioPortA OnGPIO 15 True
  Execute Command           logLevel 3 ${SPI}

  # Run
  Execute Command           include @${HOOKS_SCRIPT}
  Execute Command           start

  # Verify
  Sleep                     5s
  Execute Command           ${UART} WriteChar 192
  Wait For Log Entry        Utils_Screen: [ATTINY] Running integrity checks  timeout=5
  Wait For Log Entry        Utils_Screen: CORRUPTED, system will reboot now  timeout=5

  Sleep                     5s
  Execute Command           ${UART} WriteChar 202
  Wait For Log Entry        Utils_Screen: UNSTABLE, ATTiny crashed           timeout=5
  Wait For Log Entry        ${FLAG_STRING}                                   timeout=5

  Sleep                     20s
  Execute Command           ${UART} WriteChar 203
  Wait For Log Entry        Utils_Screen: VERIFIED, system is stable         timeout=5
  Wait For Log Entry        ${FLAG_STRING}                                   timeout=5
