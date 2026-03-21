#include "lab_uart.h"
#include "lab_uart_data.h"
#include "utils_uart.h"
#include "utils_shell.h"
#include "utils_secrets.h"
#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef huart1;

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

Lab_StatusTypeDef Lab_UART_Init(void) {
  // Nothing to do yet
  return LAB_OK;
}

void Lab_UART_Loop(void) {
  // Boot sequence debug logs
  Lab_UART_Boot_Sequence(LAB_UART_USER_USERNAME, LAB_UART_USER_PASSWORD, LAB_UART_USER_PASSWORD_LEN);

  // Wait for user to interact
  Utils_UART_ReceiveEnter();

  // Loop user in anonymous shell until SHELL_EXIT (login)
  Utils_UART_Writeline("[DVH] Welcome to the UART shell !\r\n");
  Utils_Secrets_Transmit_Flag(LAB_UART_FLAG_ONE, LAB_UART_FLAG_ONE_LEN);
  Utils_Shell_Start("[anonymous@dvh]$ ", LAB_UART_COMMANDS_ANONYMOUS);

  // Loop user in user shell until SHELL_EXIT (root)
  Utils_Secrets_Transmit_Flag(LAB_UART_FLAG_TWO, LAB_UART_FLAG_TWO_LEN);
  Utils_Shell_Start("[monitoring_svc@dvh]$ ", LAB_UART_COMMANDS_USER);

  // Loop user in root shell until SHELL_EXIT (reboot)
  Utils_Secrets_Transmit_Flag(LAB_UART_FLAG_THREE, LAB_UART_FLAG_THREE_LEN);
  Utils_Shell_Start("[root@dvh]# ", LAB_UART_COMMANDS_ROOT);
}

Lab_StatusTypeDef Lab_UART_Reset(void) {
  // Nothing to do yet
  return LAB_OK;
}

ILab Lab_UART = {
  .id = 1,
  .name = "UART",
  .init = Lab_UART_Init,
  .loop = Lab_UART_Loop,
  .reset = Lab_UART_Reset
};
