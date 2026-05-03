*** Settings ***
Resource                    ${RENODEKEYWORDS}
Library                     OperatingSystem

Suite Setup                 Setup
Suite Teardown              Teardown
Test Teardown               Teardown

*** Variables ***
${PLATFORM}                 ${CURDIR}/../platforms/dvh.repl
${ELF}                      ${CURDIR}/../../../build_simulation/firmware.elf
${UART}                     sysbus.usart1

${USER_PASS}                burn3d_1n_th3_r0m
${FLAG_STRING}              Here, take this: DVH\{

*** Test Cases ***
Verify Lab I2C
  [Documentation]           Verify I2C can be correctly sniffed or controlled to print flags
  File Should Exist         ${ELF}  msg=Firmware ELF not found

  # Setup
  Execute Command           mach create "dvh_test_i2c"
  Execute Command           machine LoadPlatformDescription @${PLATFORM}
  Execute Command           sysbus LoadELF @${ELF}

  # Config (select lab 0010 and press reset button)
  Create Terminal Tester    ${UART}
  Execute Command           sysbus.gpioPortB OnGPIO 3 True
  Execute Command           sysbus.gpioPortB OnGPIO 9 True

  # Run
  Execute Command           start

  # Verify
  # The Renode I2C driver is broken. Because of this, we cannot perform real tests that mimic I2C behavior. This may be fixed in the future, which is why this file is still present.
