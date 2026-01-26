#include "lab_uart.h"
#include "lab_uart_commands.h"
#include "utils_uart.h"
#include "utils_shell.h"
#include "utils_flag.h"
#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef huart1;

// DVH{1ns1d3_th3_syst3m_be8811624e}
static const unsigned char LAB_UART_FLAG_ONE[] = {0x2e, 0x23, 0x3b, 0x0f, 0x58, 0x00, 0x19, 0x44, 0x17, 0x47, 0x36, 0x1a, 0x02, 0x46, 0x2c, 0x07, 0x10, 0x1d, 0x1e, 0x46, 0x1e, 0x2b, 0x0b, 0x0b, 0x52, 0x4d, 0x42, 0x45, 0x5f, 0x5c, 0x5e, 0x10, 0x0e};

// DVH{h0w_d1d_y0u_g3t_h3r3_e322687e2e}
static const unsigned char LAB_UART_FLAG_TWO[] = {0x2e, 0x23, 0x3b, 0x0f, 0x01, 0x5e, 0x1d, 0x2a, 0x17, 0x45, 0x0d, 0x31, 0x13, 0x45, 0x06, 0x2b, 0x0e, 0x5d, 0x1e, 0x2a, 0x1b, 0x47, 0x1b, 0x5d, 0x35, 0x10, 0x40, 0x46, 0x5b, 0x58, 0x52, 0x42, 0x16, 0x46, 0x0c, 0x13};

// DVH{h4v3_y0u_m3t_my_fr13nd_j0hn_9ef271f76e}
static const unsigned char LAB_UART_FLAG_THREE[] = {0x2e, 0x23, 0x3b, 0x0f, 0x01, 0x5a, 0x1c, 0x46, 0x2c, 0x0d, 0x59, 0x1b, 0x35, 0x18, 0x40, 0x00, 0x36, 0x03, 0x13, 0x2a, 0x15, 0x06, 0x58, 0x5d, 0x04, 0x11, 0x2c, 0x1e, 0x59, 0x06, 0x04, 0x2a, 0x4a, 0x11, 0x0f, 0x5c, 0x5d, 0x44, 0x15, 0x43, 0x5f, 0x0b, 0x17};

// u4rt_1s_pr3tty_v3rb0s3
static const unsigned char LAB_UART_USER_PASSWORD[] = {0x1f, 0x41, 0x01, 0x00, 0x36, 0x5f, 0x19, 0x2a, 0x03, 0x06, 0x5a, 0x1a, 0x1e, 0x0c, 0x2c, 0x02, 0x5a, 0x1c, 0x08, 0x45, 0x00, 0x47};

void Lab_UART_Transmit_Flag(const unsigned char* flag, int len) {
  char decrypted_flag[64];
  Utils_Flag_Decrypt(flag, len, decrypted_flag, sizeof(decrypted_flag));

  char output[128];
  snprintf(output, sizeof(output), "[DVH] Here, take this: %s\r\n", decrypted_flag);
  Utils_UART_Writeline(output);

  memset(decrypted_flag, 0, sizeof(decrypted_flag));
  memset(output, 0, sizeof(output));
}

void Lab_UART_Boot_Sequence(const unsigned char* user_pass, int len) {
  // Useless system logs
  Utils_UART_Writeline("[BOOT] System Init...\r\n");
  HAL_Delay(200);
  Utils_UART_Writeline("[INFO] CPU Clock: 72 MHz\r\n");
  HAL_Delay(50);
  Utils_UART_Writeline("[INFO] Drivers: UART, GPIO, FLASH loaded.\r\n");
  HAL_Delay(200);

  // Leak user password
  char decrypted_pass[64];
  Utils_Flag_Decrypt(user_pass, len, decrypted_pass, sizeof(decrypted_pass));

  char pass_output[128];
  snprintf(pass_output, sizeof(pass_output), "[DEBUG] Init Password: '%s'\r\n", decrypted_pass);

  Utils_UART_Writeline("[DEBUG] Default User: 'monitoring_svc'\r\n");
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
  {"root", Lab_UART_Cmd_Root, "Log in as root"},
  {NULL, NULL, NULL}
};

static const ShellCommand COMMANDS_ROOT[] = {
  {"echo", Lab_UART_Cmd_Echo, "Echo text back to the terminal"},
  {"clear", Lab_UART_Cmd_Clear, "Clear the terminal"},
  {"get_users", Lab_UART_Cmd_GetUsers, "Fetch the list of existing users"},
  {"reboot", Lab_UART_Cmd_Reboot, "Reboot the shell"},
  {NULL, NULL, NULL}
};

void Lab_UART_Init(void) {
  // Nothing to do yet
}

void Lab_UART_Loop(void) {
  // Boot sequence debug logs
  Lab_UART_Boot_Sequence(LAB_UART_USER_PASSWORD, sizeof(LAB_UART_USER_PASSWORD));

  // Wait for user to interact
  Utils_UART_ReceiveEnter();

  Utils_UART_Writeline("[DVH] Welcome to the UART shell !\r\n");
  Lab_UART_Transmit_Flag(LAB_UART_FLAG_ONE, sizeof(LAB_UART_FLAG_ONE));
  Utils_Shell_Start("[anonymous@dvh]$ ", COMMANDS_ANONYMOUS);

  Utils_UART_Writeline("[DVH] Logged in successfully !\r\n");
  Lab_UART_Transmit_Flag(LAB_UART_FLAG_TWO, sizeof(LAB_UART_FLAG_TWO));
  Utils_Shell_Start("[monitoring_svc@dvh]$ ", COMMANDS_USER);

  Utils_UART_Writeline("[DVH] Root authorization granted !\r\n");
  Lab_UART_Transmit_Flag(LAB_UART_FLAG_THREE, sizeof(LAB_UART_FLAG_THREE));
  Utils_Shell_Start("[root@dvh]# ", COMMANDS_ROOT);
}

ILab Lab_UART = {
  .id = 1,
  .name = "UART",
  .init = Lab_UART_Init,
  .loop = Lab_UART_Loop
};
