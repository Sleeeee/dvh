#include "lab_uart.h"
#include "lab_uart_commands.h"
#include "utils_uart.h"
#include "utils_shell.h"
#include "utils_flag.h"
#include <string.h>

extern UART_HandleTypeDef huart1;

// DVH{1ns1d3_th3_syst3m_be8811624e}
static const unsigned char LAB_UART_FLAG_ONE[] = {0x2e, 0x23, 0x3b, 0x0f, 0x58, 0x00, 0x19, 0x44, 0x17, 0x47, 0x36, 0x1a, 0x02, 0x46, 0x2c, 0x07, 0x10, 0x1d, 0x1e, 0x46, 0x1e, 0x2b, 0x0b, 0x0b, 0x52, 0x4d, 0x42, 0x45, 0x5f, 0x5c, 0x5e, 0x10, 0x0e};

// DVH{h0w_d1d_y0u_g3t_h3r3_e322687e2e}
static const unsigned char LAB_UART_FLAG_TWO[] = {0x2e, 0x23, 0x3b, 0x0f, 0x01, 0x5e, 0x1d, 0x2a, 0x17, 0x45, 0x0d, 0x31, 0x13, 0x45, 0x06, 0x2b, 0x0e, 0x5d, 0x1e, 0x2a, 0x1b, 0x47, 0x1b, 0x5d, 0x35, 0x10, 0x40, 0x46, 0x5b, 0x58, 0x52, 0x42, 0x16, 0x46, 0x0c, 0x13};

// DVH{h4v3_y0u_m3t_my_fr13nd_j0hn_9ef271f76e}
static const unsigned char LAB_UART_FLAG_THREE[] = {0x2e, 0x23, 0x3b, 0x0f, 0x01, 0x5a, 0x1c, 0x46, 0x2c, 0x0d, 0x59, 0x1b, 0x35, 0x18, 0x40, 0x00, 0x36, 0x03, 0x13, 0x2a, 0x15, 0x06, 0x58, 0x5d, 0x04, 0x11, 0x2c, 0x1e, 0x59, 0x06, 0x04, 0x2a, 0x4a, 0x11, 0x0f, 0x5c, 0x5d, 0x44, 0x15, 0x43, 0x5f, 0x0b, 0x17};

void Lab_UART_Transmit_Flag(const unsigned char* flag, int len) {
  char buffer[64];
  Utils_Flag_Decrypt(flag, len, buffer, sizeof(buffer));

  HAL_UART_Transmit(&huart1, (uint8_t*)"[DVH] Here, take this: ", 23, 100);
  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), 100);
  HAL_UART_Transmit(&huart1, (uint8_t*)"\r\n", 2, 100);

  memset(buffer, 0, sizeof(buffer));
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
  Utils_UART_ReceiveEnter();

  Utils_UART_Writeline("[DVH] Welcome to the UART shell !\r\n", 35);
  Lab_UART_Transmit_Flag(LAB_UART_FLAG_ONE, sizeof(LAB_UART_FLAG_ONE));
  Utils_Shell_Start("[anonymous@dvh]$ ", COMMANDS_ANONYMOUS);

  Utils_UART_Writeline("[DVH] Logged in successfully !\r\n", 32);
  Lab_UART_Transmit_Flag(LAB_UART_FLAG_TWO, sizeof(LAB_UART_FLAG_TWO));
  Utils_Shell_Start("[monitoring_svc@dvh]$ ", COMMANDS_USER);

  Utils_UART_Writeline("[DVH] Root authorization granted !\r\n", 36);
  Lab_UART_Transmit_Flag(LAB_UART_FLAG_THREE, sizeof(LAB_UART_FLAG_THREE));
  Utils_Shell_Start("[root@dvh]# ", COMMANDS_ROOT);
}

ILab Lab_UART = {
  .id = 1,
  .name = "UART",
  .init = Lab_UART_Init,
  .loop = Lab_UART_Loop
};
