#ifndef UTILS_UART_H
#define UTILS_UART_H

#include "main.h"

typedef enum {
  UTILS_UART_CONTINUE,
  UTILS_UART_EXIT
} Utils_UART_StatusTypeDef;

typedef enum {
  UTILS_UART_ECHO_NORMAL,
  UTILS_UART_ECHO_SILENT,
  UTILS_UART_ECHO_MASKED
} Utils_UART_EchoModeTypeDef;

typedef enum {
  UTILS_UART_CONFIRMED,
  UTILS_UART_DENIED
} Utils_UART_ConfirmationTypeDef;

void Utils_UART_Writeline(const char* text);
void Utils_UART_ReceiveEnter(void);
void Utils_UART_Readline_Ex(char* buffer, uint16_t max_len, Utils_UART_EchoModeTypeDef mode);

#define Utils_UART_Readline(...) Utils_UART_Readline_Ex(__VA_ARGS__, UTILS_UART_ECHO_NORMAL)

Utils_UART_ConfirmationTypeDef Utils_UART_ReadConfirmation(char* prompt);

#endif
