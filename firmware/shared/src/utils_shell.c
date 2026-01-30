#include "utils_shell.h"
#include "utils_uart.h"
#include <stdio.h>
#include <string.h>

void Utils_Shell_Help(const ShellCommand* commands) {
  Utils_UART_Writeline("Available commands:\r\n");

  const ShellCommand* curr = commands;
  while (curr->name != NULL) {
    char output[64];
    snprintf(output, sizeof(output), "    - %s : %s\r\n", curr->name, curr->description);
    Utils_UART_Writeline(output);
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

  Utils_UART_Writeline("Unrecognized command. Type 'help' to list available commands.\r\n");
  return SHELL_CONTINUE;
}

ShellStatus Utils_Shell_Process_Input(char* input, const ShellCommand* commands) {
  if (input[0] == '\0') { return SHELL_CONTINUE; }

    // Retrieve command (first word) and arguments (rest of the input)
    char* cmd = strtok(input, " ");
    char* args = strtok(NULL, "");

    if (!cmd) { return SHELL_CONTINUE; }

    if (strcmp(cmd, "help") == 0) {
      Utils_Shell_Help(commands);
      return SHELL_CONTINUE;
    }

    return Utils_Shell_Execute(cmd, args, commands);
}

void Utils_Shell_Start(const char* prompt, const ShellCommand* commands) {
  char input[64];

  while (1) {
    Utils_UART_Writeline(prompt);
    Utils_UART_Readline(input, sizeof(input));

    if (Utils_Shell_Process_Input(input, commands) == SHELL_EXIT) {
      return;
    }
  }
}
