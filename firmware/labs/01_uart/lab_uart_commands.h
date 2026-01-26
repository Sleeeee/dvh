#ifndef LAB_UART_COMMANDS_H
#define LAB_UART_COMMANDS_H

#include "utils_shell.h"

ShellStatus Lab_UART_Cmd_Echo(char* args);
ShellStatus Lab_UART_Cmd_Clear(char* args);
ShellStatus Lab_UART_Cmd_GetUsers(char* args);
ShellStatus Lab_UART_Cmd_Login(char* args);

ShellStatus Lab_UART_Cmd_UserDb(char* args);
ShellStatus Lab_UART_Cmd_Root(char* args);

ShellStatus Lab_UART_Cmd_Reboot(char* args);

#endif
