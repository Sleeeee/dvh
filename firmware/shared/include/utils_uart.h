#ifndef UTILS_UART_H
#define UTILS_UART_H

#include "main.h"

void Utils_UART_Writeline(const char* text, int len);
void Utils_UART_ReceiveEnter(void);
void Utils_UART_Readline(char* buffer, uint16_t max_length);

#endif
