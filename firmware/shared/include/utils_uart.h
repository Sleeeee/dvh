#ifndef UTILS_UART_H
#define UTILS_UART_H

#include "main.h"

typedef enum {
  ECHO_NORMAL,
  ECHO_SILENT,
  ECHO_MASKED
} EchoMode;

void Utils_UART_Writeline(const char* text);
void Utils_UART_ReceiveEnter(void);
void Utils_UART_Readline_Ex(char* buffer, uint16_t max_len, EchoMode mode);

#define Utils_UART_Readline(...) Utils_UART_Readline_Ex(__VA_ARGS__, ECHO_NORMAL)

#endif
