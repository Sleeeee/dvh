#ifndef LAB_UART_COMMANDS_H
#define LAB_UART_COMMANDS_H

#include "utils_shell.h"

Utils_Shell_StatusTypeDef Lab_UART_Cmd_GetUsers(char* args);
Utils_Shell_StatusTypeDef Lab_UART_Cmd_Login(char* args);

Utils_Shell_StatusTypeDef Lab_UART_Cmd_UserDb(char* args);
Utils_Shell_StatusTypeDef Lab_UART_Cmd_Root(char* args);

#endif
