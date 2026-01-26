#include "lab_uart.h"
#include "lab_uart_data.h"
#include "lab_uart_commands.h"
#include "utils_uart.h"
#include "utils_shell.h"
#include "utils_secrets.h"
#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef huart1;

void Lab_UART_Transmit_Flag(const unsigned char* flag, int len) {
  char decrypted_flag[64];
  Utils_Secrets_Decrypt(flag, len, decrypted_flag, sizeof(decrypted_flag));

  char output[128];
  snprintf(output, sizeof(output), "[DVH] Here, take this: %s\r\n", decrypted_flag);
  Utils_UART_Writeline(output);

  memset(decrypted_flag, 0, sizeof(decrypted_flag));
  memset(output, 0, sizeof(output));
}

void Lab_UART_Boot_Sequence(const char* username, const unsigned char* pass, int len) {
  // Useless system logs
  Utils_UART_Writeline("[BOOT] System Init...\r\n");
  HAL_Delay(200);
  Utils_UART_Writeline("[INFO] CPU Clock: 72 MHz\r\n");
  HAL_Delay(50);
  Utils_UART_Writeline("[INFO] Drivers: UART, GPIO, FLASH loaded.\r\n");
  HAL_Delay(200);

  // Leak user password
  char decrypted_pass[64];
  Utils_Secrets_Decrypt(pass, len, decrypted_pass, sizeof(decrypted_pass));

  char pass_output[128];
  snprintf(pass_output, sizeof(pass_output), "[DEBUG] Default User: '%s'\r\n[DEBUG] Init Password: '%s'\r\n", username, decrypted_pass);
  Utils_UART_Writeline(pass_output);

  memset(decrypted_pass, 0, sizeof(decrypted_pass));
  memset(pass_output, 0, sizeof(pass_output));
  HAL_Delay(200);

  // More useless system logs
  Utils_UART_Writeline("[BOOT] Starting Memory Integrity Check...\r\n");
  char flood_output[64];
  uint32_t addr = 0x20000000;

  for (int i = 0; i < 50; i++) {
    snprintf(flood_output, sizeof(flood_output), "[BOOT] Checking address 0x%08X... OK\r\n", (unsigned int)(addr + (i * 0x100)));
    Utils_UART_Writeline(flood_output);
    HAL_Delay(20);
  }

  Utils_UART_Writeline("[BOOT] Memory Check PASSED.\r\n");
  Utils_UART_Writeline("[BOOT] Boot sequence finished. Press [ENTER] to start the shell.\r\n");
}

static const ShellCommand COMMANDS_ANONYMOUS[] = {
  {"echo", Lab_UART_Cmd_Echo, "Echo text back to the terminal"},
  {"clear", Lab_UART_Cmd_Clear, "Clear the terminal"},
  {"get_users", Lab_UART_Cmd_GetUsers, "Fetch the list of existing users"},
  {"login", Lab_UART_Cmd_Login, "Log in as an existing user"},
  {NULL, NULL, NULL}
};

static const ShellCommand COMMANDS_USER[] = {
  {"echo", Lab_UART_Cmd_Echo, "Echo text back to the terminal"},
  {"clear", Lab_UART_Cmd_Clear, "Clear the terminal"},
  {"get_users", Lab_UART_Cmd_GetUsers, "Fetch the list of existing users"},
  {"user_db", Lab_UART_Cmd_UserDb, "Manage the internal user database"},
  {"root", Lab_UART_Cmd_Root, "Log in as root"},
  {NULL, NULL, NULL}
};

static const ShellCommand COMMANDS_ROOT[] = {
  {"echo", Lab_UART_Cmd_Echo, "Echo text back to the terminal"},
  {"clear", Lab_UART_Cmd_Clear, "Clear the terminal"},
  {"get_users", Lab_UART_Cmd_GetUsers, "Fetch the list of existing users"},
  {"user_db", Lab_UART_Cmd_UserDb, "Manage the internal user database"},
  {"reboot", Lab_UART_Cmd_Reboot, "Reboot the shell"},
  {NULL, NULL, NULL}
};

void Lab_UART_Init(void) {
  // Nothing to do yet
}

void Lab_UART_Loop(void) {
  // Boot sequence debug logs
  Lab_UART_Boot_Sequence(LAB_UART_USER_USERNAME, LAB_UART_USER_PASSWORD, LAB_UART_USER_PASSWORD_LEN);

  // Wait for user to interact
  Utils_UART_ReceiveEnter();

  // Loop user in anonymous shell until SHELL_EXIT (login)
  Utils_UART_Writeline("[DVH] Welcome to the UART shell !\r\n");
  Lab_UART_Transmit_Flag(LAB_UART_FLAG_ONE, LAB_UART_FLAG_ONE_LEN);
  Utils_Shell_Start("[anonymous@dvh]$ ", COMMANDS_ANONYMOUS);

  // Loop user in user shell until SHELL_EXIT (root)
  Lab_UART_Transmit_Flag(LAB_UART_FLAG_TWO, LAB_UART_FLAG_TWO_LEN);
  Utils_Shell_Start("[monitoring_svc@dvh]$ ", COMMANDS_USER);

  // Loop user in root shell until SHELL_EXIT (reboot)
  Lab_UART_Transmit_Flag(LAB_UART_FLAG_THREE, LAB_UART_FLAG_THREE_LEN);
  Utils_Shell_Start("[root@dvh]# ", COMMANDS_ROOT);
}

ILab Lab_UART = {
  .id = 1,
  .name = "UART",
  .init = Lab_UART_Init,
  .loop = Lab_UART_Loop
};
