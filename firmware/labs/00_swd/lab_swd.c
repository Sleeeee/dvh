#include "lab_swd.h"
#include <stdio.h>

static void Solve_Flag_One(const char* flag, volatile char* buffer) {
  // Print the flag into the buffer
  // Casting from volatile char* so sprintf understands
  sprintf((char*)buffer, "%s", flag);
}

static void Solve_Flag_Two(const unsigned char* flag, volatile char* buffer) {
  // XOR the encrypted flag bytes using the key
  const unsigned char XOR_KEY = 0xab;

  for (int i = 0; i < sizeof(flag); i++) {
    buffer[i] = flag[i] ^ XOR_KEY;
  }
}

static void Solve_Flag_Three(const unsigned char* flag, volatile char* buffer) {
  // Combination of XOR and single-byte arithmetics to print the flag
  const unsigned char INCREMENT = 0x31;
  const unsigned char XOR_KEY = 0x0f;

  for (int i = 0; i < sizeof(flag); i++) {
    buffer[i] = (((flag[i] ^ XOR_KEY) + (3 * INCREMENT)) ^ XOR_KEY) - INCREMENT;
  }
}

void Lab_SWD_Init(void) {
  const char FLAG_ONE[] = "DVH{pl41n_text_d3t3ct1v3_fb242a8175}";

  // DVH{h1d1ng_b3h1nd_x0r_4e2f398b6b}
  const unsigned char FLAG_TWO[] = {0xef, 0xfd, 0xe3, 0xd0, 0xc3, 0x9a, 0xcf, 0x9a, 0xc5, 0xcc, 0xf4, 0xc9, 0x98, 0xc3, 0x9a, 0xc5, 0xcf, 0xf4, 0xd3, 0x9b, 0xd9, 0xf4, 0x9f, 0xce, 0x99, 0xcd, 0x98, 0x92, 0x93, 0xc9, 0x9d, 0xc9, 0xd6};

  // DVH{y0ur_b1n_1s_l34k1ng_c8ffc55f5e}
  const unsigned char FLAG_THREE[] = {0xe8, 0xfa, 0xec, 0x1f, 0x1d, 0xd4, 0x19, 0x16, 0x03, 0x06, 0xd5, 0xf2, 0x03, 0xd5, 0x17, 0x03, 0xf0, 0xd7, 0xd8, 0x0f, 0xd5, 0xf2, 0x0b, 0x03, 0x07, 0xdc, 0x0a, 0x0a, 0x07, 0xd9, 0xd9, 0x0a, 0xd9, 0x09, 0x01, 0xa4};

  volatile char buffer[64];

  Solve_Flag_One(FLAG_ONE, buffer);
  Solve_Flag_Two(FLAG_TWO, buffer);
  Solve_Flag_Three(FLAG_THREE, buffer);
}

void Lab_SWD_Loop(void) {
  // Nothing to do yet
}
