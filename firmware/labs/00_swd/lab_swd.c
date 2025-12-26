#include "lab_swd.h"
#include <stdio.h>

// Attributes tell the compiler not to delete the flag
__attribute__((used, section(".text"))) 
const char FLAG_ONE[] = "DVH{H1dd3n_1n_pl41n_s1ght_fb242a8175}";

void Lab_SWD_Init(void) {
  // Nothing to do yet
}

void Lab_SWD_Loop(void) {
  volatile char buffer[64];
  sprintf((char*)buffer, "%s", FLAG_ONE);
}
