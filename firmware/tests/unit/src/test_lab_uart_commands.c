#include "../lib/unity.h"
#include "../../../labs/01_uart/lab_uart_commands.c"
#include "../../../labs/01_uart/lab_uart_data.c"
#include "../../../shared/src/utils_secrets.c"
#include "stm32f1xx_hal.h"
#include <string.h>

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

void Utils_UART_Readline_Ex(char* buffer, uint16_t max_len, EchoMode mode) {
  if (INPUT_QUEUE[input_idx]) strncpy(buffer, INPUT_QUEUE[input_idx++], max_len);
}
#define Utils_UART_Readline(...) Utils_UART_Readline_Ex(__VA_ARGS__, ECHO_NORMAL)

void setUp(void) {
  SPY_UART_Clear();
  input_idx = 0;
}

void tearDown(void) {}

void test_Lab_UART_Cmd_Echo_should_echo(void) {
  Lab_UART_Cmd_Echo("hello world");

  TEST_ASSERT_EQUAL_STRING("hello world\r\n", SPY_UART_Buffer);
}

void test_Lab_UART_Cmd_Echo_should_fail(void) {
  Lab_UART_Cmd_Echo(NULL);

  TEST_ASSERT_NOT_NULL(strstr(SPY_UART_Buffer, "Usage: echo"));
}

void test_Lab_UART_Cmd_GetUsers_should_list(void) {
  Lab_UART_Cmd_GetUsers(NULL);

  TEST_ASSERT_NOT_NULL(strstr(SPY_UART_Buffer, "root"));
  TEST_ASSERT_NOT_NULL(strstr(SPY_UART_Buffer, "monitoring_svc"));
}

void test_Lab_UART_Cmd_Login_should_succeed(void) {
  Set_Input("monitoring_svc", "u4rt_1s_pr3tty_v3rb0s3");

  ShellStatus result = Lab_UART_Cmd_Login(NULL);

  TEST_ASSERT_EQUAL(SHELL_EXIT, result);
  TEST_ASSERT_NOT_NULL(strstr(SPY_UART_Buffer, "Logged in successfully"));
}

void test_Lab_UART_Cmd_Login_should_fail(void) {
  Set_Input("monitoring_svc", "wr0ng_p4ss");

  ShellStatus result = Lab_UART_Cmd_Login(NULL);

  TEST_ASSERT_EQUAL(SHELL_CONTINUE, result);
  TEST_ASSERT_NOT_NULL(strstr(SPY_UART_Buffer, "Incorrect password"));
}

void test_Lab_UART_Cmd_UserDb_should_dump(void) {
  Lab_UART_Cmd_UserDb("--dump");

  TEST_ASSERT_NOT_NULL(strstr(SPY_UART_Buffer, "root"));
  TEST_ASSERT_NOT_NULL(strstr(SPY_UART_Buffer, "Password Hash"));
}

void test_Lab_UART_Cmd_Root_should_succeed(void) {
  Set_Input("linkinpark", NULL); 

  ShellStatus result = Lab_UART_Cmd_Root(NULL);

  TEST_ASSERT_EQUAL(SHELL_EXIT, result);
  TEST_ASSERT_NOT_NULL(strstr(SPY_UART_Buffer, "Root authorization granted"));
}

void test_Lab_UART_Cmd_Root_should_fail(void) {
  Set_Input("wr0ng_p4ss", NULL);

  ShellStatus result = Lab_UART_Cmd_Root(NULL);

  TEST_ASSERT_EQUAL(SHELL_CONTINUE, result);
  TEST_ASSERT_NOT_NULL(strstr(SPY_UART_Buffer, "Incorrect password"));
}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_Lab_UART_Cmd_Echo_should_echo);
  RUN_TEST(test_Lab_UART_Cmd_Echo_should_fail);
  RUN_TEST(test_Lab_UART_Cmd_GetUsers_should_list);
  RUN_TEST(test_Lab_UART_Cmd_Login_should_succeed);
  RUN_TEST(test_Lab_UART_Cmd_Login_should_fail);
  RUN_TEST(test_Lab_UART_Cmd_UserDb_should_dump);
  RUN_TEST(test_Lab_UART_Cmd_Root_should_succeed);
  RUN_TEST(test_Lab_UART_Cmd_Root_should_fail);

  return UNITY_END();
}
