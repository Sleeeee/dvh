#include "utils_shell.h"
#include "utils_uart.h"
#include <string.h>

void Utils_Shell_Help(const ShellCommand* commands) {
  Utils_UART_Writeline("Available commands:\r\n");

  const ShellCommand* curr = commands;
  while (curr->name != NULL) {
    Utils_UART_Writeline("    - ");
    Utils_UART_Writeline(curr->name);
    Utils_UART_Writeline(" : ");
    Utils_UART_Writeline(curr->description);
    Utils_UART_Writeline("\r\n");

    curr++;
  }
}

ShellStatus Utils_Shell_Execute(char* cmd, char* args, const ShellCommand* commands) {
  const ShellCommand* curr = commands;

  while (curr->name != NULL) {
    if (strcmp(cmd, curr->name) == 0) {
      return curr->handler(args); // Execute and return ShellStatus
    }
    curr++;
  }

  Utils_UART_Writeline("Unrecognized command.\r\n");
  return SHELL_CONTINUE;
}

void Utils_Shell_Start(const char* prompt, const ShellCommand* commands) {
  char input[64];

  while (1) {
    Utils_UART_Writeline(prompt);
    Utils_UART_Readline(input, sizeof(input));

    if (input[0] == '\0') { continue; }

    // Retrieve command (first word) and arguments (rest of the input)
    char* cmd = strtok(input, " ");
    char* args = strtok(NULL, "");

    if (!cmd) { continue; }

    if (strcmp(cmd, "help") == 0) {
      Utils_Shell_Help(commands);
      continue;
    }

    if (Utils_Shell_Execute(cmd, args, commands) == SHELL_EXIT) {
      return;
    };
  }
}
