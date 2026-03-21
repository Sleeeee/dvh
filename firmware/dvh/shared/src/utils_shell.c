#include "utils_shell.h"
#include "utils_uart.h"
#include <stdio.h>
#include <string.h>

Utils_Shell_StatusTypeDef Utils_Shell_Cmd_Echo(char* args) {
  if (args && *args) {
    Utils_UART_Writeline(args);
    Utils_UART_Writeline("\r\n");
  } else {
    Utils_UART_Writeline("Usage: echo [text]\r\n");
  }
  return UTILS_SHELL_CONTINUE;
}

Utils_Shell_StatusTypeDef Utils_Shell_Cmd_Clear(char* args) {
  Utils_UART_Writeline("\033[2J\033[H");
  return UTILS_SHELL_CONTINUE;
}

Utils_Shell_StatusTypeDef Utils_Shell_Cmd_Reboot(char* args) {
  Utils_UART_Writeline("[DVH] System rebooted\r\n");
  HAL_Delay(200);
  Utils_UART_Writeline("\033[2J\033[H");
  return UTILS_SHELL_EXIT;
}

void Utils_Shell_Help(const Utils_Shell_Command* commands) {
  Utils_UART_Writeline("Available commands:\r\n");

  const Utils_Shell_Command* curr = commands;
  while (curr->name != NULL) {
    char output[64];
    snprintf(output, sizeof(output), "    - %s : %s\r\n", curr->name, curr->description);
    Utils_UART_Writeline(output);
    curr++;
  }
}

Utils_Shell_StatusTypeDef Utils_Shell_Execute(char* cmd, char* args, const Utils_Shell_Command* commands) {
  const Utils_Shell_Command* curr = commands;

  while (curr->name != NULL) {
    if (strcmp(cmd, curr->name) == 0) {
      return curr->handler(args); // Execute and return shell StatusTypeDef
    }
    curr++;
  }

  Utils_UART_Writeline("Unrecognized command. Type 'help' to list available commands.\r\n");
  return UTILS_SHELL_CONTINUE;
}

Utils_Shell_StatusTypeDef Utils_Shell_Process_Input(char* input, const Utils_Shell_Command* commands) {
  if (input[0] == '\0') { return UTILS_SHELL_CONTINUE; }

    // Retrieve command (first word) and arguments (rest of the input)
    char* cmd = strtok(input, " ");
    char* args = strtok(NULL, "");

    if (!cmd) { return UTILS_SHELL_CONTINUE; }

    if (strcmp(cmd, "help") == 0) {
      Utils_Shell_Help(commands);
      return UTILS_SHELL_CONTINUE;
    }

    return Utils_Shell_Execute(cmd, args, commands);
}

void Utils_Shell_Start(const char* prompt, const Utils_Shell_Command* commands) {
  char input[64];

  while (1) {
    Utils_UART_Writeline(prompt);
    Utils_UART_Readline(input, sizeof(input));

    if (Utils_Shell_Process_Input(input, commands) == UTILS_SHELL_EXIT) {
      return;
    }
  }
}
