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

${USER_PASS}                u4rt_1s_pr3tty_v3rb0s3
${ROOT_PASS}                linkinpark
${FLAG_STRING}              Here, take this: DVH\{

*** Test Cases ***
Verify Lab UART
  [Documentation]           Verify flags are printed to UART given correct user input
  File Should Exist         ${ELF}  msg=Firmware ELF not found

  # Setup
  Execute Command           mach create "dvh_test_uart"
  Execute Command           machine LoadPlatformDescription @${PLATFORM}
  Execute Command           sysbus LoadELF @${ELF}

  # Config (select lab 0001)
  Create Terminal Tester    ${UART}
  Execute Command           sysbus.gpioPortA OnGPIO 4 True

  # Run
  Execute Command           start

  # Verify
  Wait For Line On UART     Init Password:                    timeout=10
  Wait For Line On UART     Press [ENTER] to start the shell
  Write To UART             \r
  Wait For Line On UART     ${FLAG_STRING}                    timeout=5

  Wait For Prompt On UART   anonymous@dvh                     timeout=5
  Write Line To UART        login
  Wait For Prompt On UART   Username:                         timeout=5
  Write To UART             monitoring_svc\r
  Wait For Prompt On UART   Password:                         timeout=5
  Write To UART             ${USER_PASS}\r
  Wait For Line On UART     ${FLAG_STRING}                    timeout=5

  Wait For Prompt On UART   monitoring_svc@dvh                timeout=5
  Write Line To UART        user_db --dump
  Wait For Line On UART     Dump complete                     timeout=5
  Wait For Prompt On UART   monitoring_svc@dvh                timeout=5
  Write Line To UART        root
  Wait For Prompt On UART   Root password:
  Write To UART             ${ROOT_PASS}\r
  Wait For Line On UART     ${FLAG_STRING}                    timeout=5
