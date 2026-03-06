#ifndef UTILS_SECRETS_H
#define UTILS_SECRETS_H

#include <stdbool.h>

void Utils_Secrets_Decrypt(const unsigned char* secret, int len, char* buffer, int buf_size);

bool Utils_Secrets_Check_Password(const char* input, const unsigned char* secret, const int len);

#endif
