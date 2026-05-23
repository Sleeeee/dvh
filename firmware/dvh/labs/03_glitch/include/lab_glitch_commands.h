#ifndef LAB_GLITCH_COMMANDS_H
#define LAB_GLITCH_COMMANDS_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
  bool ui_printed;
  bool flag_one_printed;
  bool flag_two_printed;
  uint32_t last_dot;
  uint32_t last_corrupt;
  uint32_t last_crash;
} Lab_Glitch_State;

void Lab_Glitch_Cmd_InitState(Lab_Glitch_State* state);

void Lab_Glitch_Cmd_ProcessByte(Lab_Glitch_State* state, uint8_t c, uint32_t tick);

#endif
