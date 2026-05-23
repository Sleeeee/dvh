#include "lab_glitch_commands.h"
#include "lab_glitch_data.h"
#include "utils_secrets.h"
#include "utils_screen.h"
#include "main.h"

void Lab_Glitch_Cmd_InitState(Lab_Glitch_State* state) {
  state->ui_printed = false;
  state->flag_one_printed = false;
  state->flag_two_printed = false;
  state->last_dot = 0;
  state->last_corrupt = 0;
  state->last_crash = 0;
}

void Lab_Glitch_Cmd_ProcessByte(Lab_Glitch_State* state, uint8_t c, uint32_t tick) {
  if (!state->ui_printed) {
    Utils_Screen_Fill_Write("[ATTINY] Running integrity checks", UTILS_SCREEN_STANDARD);
    state->ui_printed = true;
  }

  // Standard state
  if (c == 0xc0) {
    if (tick > (state->last_dot + 500)) {
      Utils_Screen_WriteChar('.', UTILS_SCREEN_STANDARD);
      state->last_dot = tick;
    }

    if (tick > (state->last_corrupt + 2500)) {
      Utils_Screen_Write("CORRUPTED, system will reboot now", UTILS_SCREEN_WARNING);
      state->last_corrupt = tick;
      state->ui_printed = false;
    }

  } else if (c == 0xca) {
    // Crash state
    if (tick > state->last_crash + 1000) {
      Utils_Screen_Write("UNSTABLE, ATTiny crashed", UTILS_SCREEN_WARNING);
      state->last_crash = tick;
      state->ui_printed = false;
    }

    // Print flag one on first occurence
    if (!state->flag_one_printed) {
      char flag[64];
      Utils_Secrets_Decrypt(LAB_GLITCH_FLAG_ONE, LAB_GLITCH_FLAG_ONE_LEN, flag, sizeof(flag));
      Utils_Screen_Write(flag, UTILS_SCREEN_WARNING);

      state->flag_one_printed = true;
      HAL_Delay(20000);
    }

  } else if (c == 0xcb) {
    // Successful glitch state
    Utils_Screen_Write("VERIFIED, system is stable", UTILS_SCREEN_STANDARD);

    char flag[64];
    Utils_Secrets_Decrypt(LAB_GLITCH_FLAG_TWO, LAB_GLITCH_FLAG_TWO_LEN, flag, sizeof(flag));
    Utils_Screen_Write(flag, UTILS_SCREEN_WARNING);
    state->flag_two_printed = true;

  } else {
    Utils_Screen_WriteChar((char)c, UTILS_SCREEN_WARNING);
  }
}
