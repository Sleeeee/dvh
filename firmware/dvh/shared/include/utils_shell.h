#ifndef UTILS_SHELL_H
#define UTILS_SHELL_H

typedef enum {
  UTILS_SHELL_CONTINUE,
  UTILS_SHELL_EXIT
} Utils_Shell_StatusTypeDef;

typedef Utils_Shell_StatusTypeDef (*CmdHandler)(char* args);

typedef struct {
  const char* name;
  CmdHandler handler;
  const char* description;
} Utils_Shell_Command;

Utils_Shell_StatusTypeDef Utils_Shell_Cmd_Echo(char* args);
Utils_Shell_StatusTypeDef Utils_Shell_Cmd_Clear(char* args);
Utils_Shell_StatusTypeDef Utils_Shell_Cmd_Reboot(char* args);
void Utils_Shell_Start(const char* prompt, const Utils_Shell_Command* commands);

#endif
