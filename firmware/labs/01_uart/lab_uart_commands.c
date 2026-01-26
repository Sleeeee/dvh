#include "lab_uart_commands.h"
#include "lab_uart_data.h"
#include "utils_uart.h"
#include "utils_secrets.h"
#include "main.h"
#include <stdio.h>
#include <string.h>

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
  char output[64];
  snprintf(output, sizeof(output), "Registered users:\r\n%s\t%s\r\n", LAB_UART_ROOT_USERNAME, LAB_UART_USER_USERNAME);
  Utils_UART_Writeline(output);
  return SHELL_CONTINUE;
}

// Anonymous commands
ShellStatus Lab_UART_Cmd_Login(char* args) {
  char username[64];
  char pass[64];
  Utils_UART_Writeline("Username: ");
  Utils_UART_Readline(username, sizeof(username));
  Utils_UART_Writeline("Password: ");
  Utils_UART_Readline_Ex(pass, sizeof(pass), ECHO_SILENT);

  if (strcmp(username, LAB_UART_USER_USERNAME) == 0) {
    if (Utils_Secrets_Check_Password(pass, LAB_UART_USER_PASSWORD, LAB_UART_USER_PASSWORD_LEN)) {
      Utils_UART_Writeline("[DVH] Logged in successfully !\r\n");
      return SHELL_EXIT;
    }

    Utils_UART_Writeline("Incorrect password.\r\n");

  } else if (strcmp(username, LAB_UART_ROOT_USERNAME) == 0) {
    Utils_UART_Writeline("Access denied. Please log in first.\r\n");
  
  } else {
    Utils_UART_Writeline("User not registered.\r\n");
  }

  return SHELL_CONTINUE;
}

// User commands
ShellStatus Lab_UART_Cmd_UserDb(char* args) {
  if (args == NULL || strcmp(args, "--help") == 0) {
    Utils_UART_Writeline("Usage: user_db [options]\r\n");
    Utils_UART_Writeline("    --help : Print this list\r\n");
    Utils_UART_Writeline("    --check : Verify integrity of user database\r\n");
    Utils_UART_Writeline("    --dump : Dump contents of the user database\r\n");

  } else if (strcmp(args, "--check") == 0) {
    Utils_UART_Writeline("[DB] Verifying user database integrity");
    for (int i = 0; i < 3; i++) {
      Utils_UART_Writeline(".");
      HAL_Delay(500);
    }
    Utils_UART_Writeline(" OK\r\n[DB] Database integrity ensured.\r\n");

  } else if (strcmp(args, "--dump") == 0) {
    char hash[128];
    char output[192];

    const char* HEADER_FORMAT = "%-4s %-16s %s\r\n";
    const char* ROW_FORMAT = "%-4d %-16s %s\r\n";

    Utils_UART_Writeline("[DB] Dumping user database...\r\n");
    HAL_Delay(500);
    snprintf(output, sizeof(output), HEADER_FORMAT, "ID", "Username", "Password Hash");
    Utils_UART_Writeline(output);

    Utils_Secrets_Decrypt(LAB_UART_ROOT_HASH, LAB_UART_ROOT_HASH_LEN, hash, sizeof(hash));
    snprintf(output, sizeof(output), ROW_FORMAT, 0, LAB_UART_ROOT_USERNAME, hash);
    Utils_UART_Writeline(output);

    Utils_Secrets_Decrypt(LAB_UART_USER_HASH, LAB_UART_USER_HASH_LEN, hash, sizeof(hash));
    snprintf(output, sizeof(output), ROW_FORMAT, 1, LAB_UART_USER_USERNAME, hash);
    Utils_UART_Writeline(output);

    memset(output, 0, sizeof(output));
    Utils_UART_Writeline("[DB] Dump complete.\r\n");

  } else {
    Utils_UART_Writeline("Unrecognized argument.\r\n");
  }

  return SHELL_CONTINUE;
}

ShellStatus Lab_UART_Cmd_Root(char* args) {
  char pass[64];
  Utils_UART_Writeline("Root password: ");
  Utils_UART_Readline_Ex(pass, sizeof(pass), ECHO_SILENT);

  if (Utils_Secrets_Check_Password(pass, LAB_UART_ROOT_PASSWORD, LAB_UART_ROOT_PASSWORD_LEN)) {
    Utils_UART_Writeline("[DVH] Root authorization granted !\r\n");
    return SHELL_EXIT;
  }

  HAL_Delay(2000);
  Utils_UART_Writeline("Incorrect password.\r\n");
  return SHELL_CONTINUE;
}

// Root commands
ShellStatus Lab_UART_Cmd_Reboot(char* args) {
  Utils_UART_Writeline("[DVH] System rebooted\r\n");
  HAL_Delay(200);
  Utils_UART_Writeline("\033[2J\033[H");
  return SHELL_EXIT;
}
