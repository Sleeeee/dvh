#include "../lib/unity.h"
#include "../../../shared/src/utils_uart.c"
#include "stm32f1xx_hal.h"
#include <string.h>

UART_HandleTypeDef huart1;

void setUp(void) {
  SPY_UART_Clear();
}

void tearDown(void) {}

void test_Utils_UART_Readchar_should_echo(void) {
  char buffer[10];
  uint16_t idx = 0;

  Utils_UART_Readchar(buffer, 'E', &idx, 10, ECHO_NORMAL);

  TEST_ASSERT_EQUAL_CHAR('E', buffer[0]);
  TEST_ASSERT_EQUAL_INT(1, idx);
  TEST_ASSERT_EQUAL_STRING("E", SPY_UART_Buffer);
}

void test_Utils_UART_Readchar_should_silence(void) {
  char buffer[10];
  uint16_t idx = 0;

  Utils_UART_Readchar(buffer, 'S', &idx, 10, ECHO_SILENT);

  TEST_ASSERT_EQUAL_CHAR('S', buffer[0]);
  TEST_ASSERT_EQUAL_INT(1, idx);
  TEST_ASSERT_EQUAL_STRING("", SPY_UART_Buffer);
}

void test_Utils_UART_Readchar_should_mask(void) {
  char buffer[10];
  uint16_t idx = 0;

  Utils_UART_Readchar(buffer, 'M', &idx, 10, ECHO_MASKED);
  TEST_ASSERT_EQUAL_CHAR('M', buffer[0]);
  TEST_ASSERT_EQUAL_STRING("*", SPY_UART_Buffer);
}

void test_Utils_UART_Readchar_should_delete(void) {
  char buffer[10];
  uint16_t idx = 0;

  Utils_UART_Readchar(buffer, 'D', &idx, 10, ECHO_NORMAL);
  Utils_UART_Readchar(buffer, '\b', &idx, 10, ECHO_NORMAL);

  TEST_ASSERT_EQUAL_INT(0, idx);
  TEST_ASSERT_EQUAL_STRING("D\b \b", SPY_UART_Buffer);
}

void test_Utils_UART_Readchar_should_exit(void) {
  char buffer[10];
  uint16_t idx = 0;

  int ret = Utils_UART_Readchar(buffer, '\r', &idx, 10, ECHO_NORMAL);

  TEST_ASSERT_EQUAL_INT(1, ret);
  TEST_ASSERT_EQUAL_STRING("\r\n", SPY_UART_Buffer);
}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_Utils_UART_Readchar_should_echo);
  RUN_TEST(test_Utils_UART_Readchar_should_silence);
  RUN_TEST(test_Utils_UART_Readchar_should_mask);
  RUN_TEST(test_Utils_UART_Readchar_should_delete);
  RUN_TEST(test_Utils_UART_Readchar_should_exit);

  return UNITY_END();
}
