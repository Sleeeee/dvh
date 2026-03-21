#ifndef LAB_I2C_DATA_H
#define LAB_I2C_DATA_H

#include "utils_shell.h"

extern const unsigned char LAB_I2C_FLAG_ONE[];
extern const int LAB_I2C_FLAG_ONE_LEN;

extern const unsigned char LAB_I2C_FLAG_TWO[];
extern const int LAB_I2C_FLAG_TWO_LEN;

extern const unsigned char LAB_I2C_FLAG_THREE[];
extern const int LAB_I2C_FLAG_THREE_LEN;

extern const unsigned char LAB_I2C_USER_PASSWORD[];
extern const int LAB_I2C_USER_PASSWORD_LEN;

extern const unsigned char LAB_I2C_CONFIG_NOTE[];
extern const int LAB_I2C_CONFIG_NOTE_LEN;

extern const Utils_Shell_Command LAB_I2C_COMMANDS_ANONYMOUS[];
extern const Utils_Shell_Command LAB_I2C_COMMANDS_USER[];
extern const Utils_Shell_Command LAB_I2C_COMMANDS_ROOT[];

extern const char* LAB_I2C_COFFEE_FACTS[];

#endif
