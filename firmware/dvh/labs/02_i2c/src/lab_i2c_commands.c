#include "lab_i2c_commands.h"
#include "lab_i2c_data.h"
#include "utils_eeprom.h"
#include "utils_uart.h"
#include "utils_secrets.h"
#include "main.h"
#include <string.h>
#include <stdio.h>

Utils_Shell_StatusTypeDef Lab_I2C_Cmd_Login(char* args) {
  Utils_UART_Writeline("[INFO] Fetching user password from the EEPROM...\r\n");
  HAL_Delay(500);

  uint8_t password[LAB_I2C_USER_PASSWORD_LEN + 1];
  memset(password, 0, sizeof(password));
  if (Utils_EEPROM_Read(0x00, password, LAB_I2C_USER_PASSWORD_LEN) != UTILS_EEPROM_OK) {
    Utils_UART_Writeline("An error occured while fetching data from the EEPROM.\r\n");
    return UTILS_SHELL_CONTINUE;
  }

  Utils_UART_Writeline("Password: ");
  char input[64];
  Utils_UART_Readline_Ex(input, sizeof(input), UTILS_UART_ECHO_SILENT);

  if (strcmp(input, (char*)password) == 0) {
    Utils_UART_Writeline("[DVH] Logged in sucessfully !\r\n");
    return UTILS_SHELL_EXIT;
  }

  Utils_UART_Writeline("Incorrect password\r\n");
  return UTILS_SHELL_CONTINUE;
}

Utils_Shell_StatusTypeDef Lab_I2C_Cmd_Coffee(char* args) {
  if (args == NULL || strcmp(args, "--help") == 0) {
    Utils_UART_Writeline("Usage: coffee [options]\r\n");
    Utils_UART_Writeline("    --help : Print this list\r\n");
    Utils_UART_Writeline("    --lovers : Print confirmed coffee lovers\r\n");
    Utils_UART_Writeline("    --facts : Learn fun facts about coffee\r\n");

  } else if (strcmp(args, "--lovers") == 0) {
    Utils_UART_Writeline("[COFFEE] Retrieving coffee lovers...\r\n");
    HAL_Delay(500);
    Utils_UART_Writeline("monitoring_svc\tjustin\talan\troot\r\n");

  } else if (strcmp(args, "--facts") == 0) {
    Utils_UART_Writeline("[COFFEE] Oh so you want to hear about coffee facts ? Let's get started !\r\n");

    int i = 0;
    Utils_UART_Writeline(LAB_I2C_COFFEE_FACTS[i]);

    while (Utils_UART_ReadConfirmation("Do you want another coffee fact ? [y/n] ") == UTILS_UART_CONFIRMED) {
      if (LAB_I2C_COFFEE_FACTS[++i] == NULL) {
        Utils_UART_Writeline("\r\nThe coffee pot is empty! No more facts available.");
        break;
      }
      Utils_UART_Writeline(LAB_I2C_COFFEE_FACTS[i]);
    }
    Utils_UART_Writeline("\r\nSee you another time for brand new coffee facts !\r\n");

  } else {
    Utils_UART_Writeline("Unrecognized argument.\r\n");
  }

  return UTILS_SHELL_CONTINUE;
}

Utils_Shell_StatusTypeDef Lab_I2C_Cmd_Root(char* args) {
  Utils_UART_Writeline("[INFO] Confirming authorization...\r\n");
  HAL_Delay(500);

  uint8_t magic[2];
  if (Utils_EEPROM_Read(0x30, magic, 2) != UTILS_EEPROM_OK) {
    Utils_UART_Writeline("An error occured while retrieving the magic bytes from the EEPROM.\r\n");
    return UTILS_SHELL_CONTINUE;
  }

  char output[32];
  snprintf(output, sizeof(output), "Found magic bytes 0x%02x%02x\r\n", magic[0], magic[1]);
  Utils_UART_Writeline(output);

  if (magic[0] == 0xca && magic[1] == 0xfe) {
    Utils_UART_Writeline("[DVH] Root authorization granted !\r\n");
    return UTILS_SHELL_EXIT;
  }

  Utils_UART_Writeline("Authorization refused. Have you read the EEPROM configuration notes ?\r\n");
  return UTILS_SHELL_CONTINUE;
}
