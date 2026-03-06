#ifndef UTILS_SHELL_H
#define UTILS_SHELL_H

typedef enum {
  SHELL_CONTINUE,
  SHELL_EXIT
} ShellStatus;

typedef ShellStatus (*CmdHandler)(char* args);

typedef struct {
  const char* name;
  CmdHandler handler;
  const char* description;
} ShellCommand;

void Utils_Shell_Start(const char* prompt, const ShellCommand* commands);

#endif
