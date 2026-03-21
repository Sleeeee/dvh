#ifndef LAB_I2C_COMMANDS_H
#define LAB_I2C_COMMANDS_H

#include "utils_shell.h"

Utils_Shell_StatusTypeDef Lab_I2C_Cmd_Login(char* args);
Utils_Shell_StatusTypeDef Lab_I2C_Cmd_Coffee(char* args);
Utils_Shell_StatusTypeDef Lab_I2C_Cmd_Root(char* args);

#endif
