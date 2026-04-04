#include "../../../dvh/shared/include/utils_uart.h"

void Utils_UART_Writeline(const char* text) {
  strncat(SPY_UART_Buffer, text, sizeof(SPY_UART_Buffer) - strlen(SPY_UART_Buffer) - 1);
}

static const char* INPUT_QUEUE[2];
static int input_idx = 0;

void Set_Input(const char* line1, const char* line2) {
  INPUT_QUEUE[0] = line1;
  INPUT_QUEUE[1] = line2;
  input_idx = 0;
}

void Utils_UART_Readline_Ex(char* buffer, uint16_t max_len, Utils_UART_EchoModeTypeDef mode) {
  if (INPUT_QUEUE[input_idx]) strncpy(buffer, INPUT_QUEUE[input_idx++], max_len);
}
#define Utils_UART_Readline(...) Utils_UART_Readline_Ex(__VA_ARGS__, UTILS_UART_ECHO_NORMAL)

Utils_UART_ConfirmationTypeDef Utils_UART_ReadConfirmation(char* prompt) {
    return true;
}
