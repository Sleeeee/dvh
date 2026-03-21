#include "utils_secrets.h"
#include "utils_uart.h"
#include <stdio.h>
#include <string.h>

static const unsigned char XOR_KEY[] = {0x6a, 0x75, 0x73, 0x74, 0x69, 0x6e};

void Utils_Secrets_Decrypt(const unsigned char* secret, int len, char* buffer, int buf_size) {
  int safe_len = (len < (buf_size - 1)) ? len : (buf_size - 1);

  for (int i = 0; i < safe_len; i++) {
    buffer[i] = secret[i] ^ XOR_KEY[i % sizeof(XOR_KEY)];
  }
  buffer[safe_len] = '\0';
}

bool Utils_Secrets_Check_Password(const char* input, const unsigned char* secret, const int len) {
  char decrypted_secret[64];
  Utils_Secrets_Decrypt(secret, len, decrypted_secret, sizeof(decrypted_secret));

  bool match = (strcmp(input, decrypted_secret) == 0);
  memset(decrypted_secret, 0, sizeof(decrypted_secret));
  return match;
}

void Utils_Secrets_Transmit_Flag(const unsigned char* flag, int len) {
  char decrypted_flag[64];
  Utils_Secrets_Decrypt(flag, len, decrypted_flag, sizeof(decrypted_flag));

  char output[128];
  snprintf(output, sizeof(output), "[DVH] Here, take this: %s\r\n", decrypted_flag);
  Utils_UART_Writeline(output);

  memset(decrypted_flag, 0, sizeof(decrypted_flag));
  memset(output, 0, sizeof(output));
}
