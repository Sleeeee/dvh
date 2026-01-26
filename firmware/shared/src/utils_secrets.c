#include "utils_secrets.h"
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
