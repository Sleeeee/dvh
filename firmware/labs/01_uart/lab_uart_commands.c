#include "lab_uart_commands.h"
#include "utils_uart.h"
#include "string.h"

// Shared commands
ShellStatus Lab_UART_Cmd_Echo(char* args) {
  if (args && *args) {
    Utils_UART_Writeline(args);
    Utils_UART_Writeline("\r\n");
  } else {
    Utils_UART_Writeline("Usage: echo [text]\r\n");
  }
  return SHELL_CONTINUE;
}

ShellStatus Lab_UART_Cmd_Clear(char* args) {
  Utils_UART_Writeline("\033[2J\033[H");
  return SHELL_CONTINUE;
}

ShellStatus Lab_UART_Cmd_GetUsers(char* args) {
  Utils_UART_Writeline("Registered users:\r\nroot\tmonitoring_svc\r\n");
  return SHELL_CONTINUE;
}

// Anonymous commands
ShellStatus Lab_UART_Cmd_Login(char* args) {
  // TODO : add password check
  return SHELL_EXIT;
}

// User commands
ShellStatus Lab_UART_Cmd_Root(char* args) {
  // TODO : add password check
  return SHELL_EXIT;
}

// Root commands
ShellStatus Lab_UART_Cmd_Reboot(char* args) {
  Utils_UART_Writeline("[DVH] System rebooted\r\n");
  return SHELL_EXIT;
}
