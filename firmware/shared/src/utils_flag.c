#include "utils_flag.h"

static const unsigned char XOR_KEY[] = {0x6a, 0x75, 0x73, 0x74, 0x69, 0x6e};

void Utils_Flag_Decrypt(const unsigned char* flag, int len, char* buffer, int buf_size) {
  int safe_len = (len < (buf_size - 1)) ? len : (buf_size - 1);

  for (int i = 0; i < safe_len; i++) {
    buffer[i] = flag[i] ^ XOR_KEY[i % sizeof(XOR_KEY)];
  }
  buffer[safe_len] = '\0';
}
