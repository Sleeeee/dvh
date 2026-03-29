#include "../lib/unity.h"
#include "../../../dvh/shared/src/utils_shell.c"
#include "stm32f1xx_hal.h"
#include <string.h>

void Utils_UART_Writeline(const char* text) {
  strncat(SPY_UART_Buffer, text, sizeof(SPY_UART_Buffer) - strlen(SPY_UART_Buffer) - 1);
}

void Utils_UART_Readline_Ex(char* buffer, uint16_t max_len, Utils_UART_EchoModeTypeDef mode) {
  if (max_len > 0) buffer[0] = '\0';
}

char LAST_ARGS[64];
Utils_Shell_StatusTypeDef Cmd_Test(char* args) {
  if (args) strcpy(LAST_ARGS, args);
  else strcpy(LAST_ARGS, "(null)");
  return UTILS_SHELL_CONTINUE;
}

static const Utils_Shell_Command TEST_CMDS[] = {
  {"test", Cmd_Test, "Test command"},
  {NULL, NULL, NULL}
};

void setUp(void) {
  SPY_UART_Clear();
  memset(LAST_ARGS, 0, sizeof(LAST_ARGS));
}

void tearDown(void) {}

void test_Utils_Shell_Cmd_Echo_should_echo(void) {
  Utils_Shell_Cmd_Echo("hello world");

  TEST_ASSERT_EQUAL_STRING("hello world\r\n", SPY_UART_Buffer);
}

void test_Utils_Shell_Cmd_Echo_should_fail(void) {
  Utils_Shell_Cmd_Echo(NULL);

  TEST_ASSERT_NOT_NULL(strstr(SPY_UART_Buffer, "Usage: echo"));
}

void test_Utils_Shell_Process_Input_should_split(void) {
  char input[] = "test hello world";

  Utils_Shell_Process_Input(input, TEST_CMDS);

  TEST_ASSERT_EQUAL_STRING("hello world", LAST_ARGS);
}

void test_Utils_Shell_Process_Input_should_accept(void) {
  char input[] = "test";

  Utils_Shell_Process_Input(input, TEST_CMDS);

  TEST_ASSERT_EQUAL_STRING("(null)", LAST_ARGS);
}

void test_Utils_Shell_Process_Input_should_help(void) {
  char input[] = "help";

  Utils_Shell_Process_Input(input, TEST_CMDS);

  TEST_ASSERT_NOT_NULL(strstr(SPY_UART_Buffer, "- test : Test command"));
}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_Utils_Shell_Cmd_Echo_should_echo);
  RUN_TEST(test_Utils_Shell_Cmd_Echo_should_fail);
  RUN_TEST(test_Utils_Shell_Process_Input_should_split);
  RUN_TEST(test_Utils_Shell_Process_Input_should_accept);
  RUN_TEST(test_Utils_Shell_Process_Input_should_help);

  return UNITY_END();
}
