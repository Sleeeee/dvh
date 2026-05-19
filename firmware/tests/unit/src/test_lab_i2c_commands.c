#include "../lib/unity.h"
#include "../../../dvh/labs/02_i2c/src/lab_i2c_commands.c"
#include "../../../dvh/labs/02_i2c/src/lab_i2c_data.c"
#include "../../../dvh/shared/src/utils_eeprom.c"
#include "../../../dvh/shared/src/utils_shell.c"
#include "../../../dvh/shared/src/utils_secrets.c"
#include "../mocks/mock_utils_uart.c"
#include "../mocks/mock_utils_screen.c"
#include "stm32f1xx_hal.h"
#include <string.h>

void setUp(void) {
  SPY_UART_Clear();
  input_idx = 0;
  SPY_I2C_Clear();
  SPY_Screen_Clear();
}

void tearDown(void) {}

void test_Lab_I2C_Cmd_Login_should_succeed(void) {
  strncpy((char*)&SPY_EEPROM_Buffer[0x00], "burn3d_1n_th3_r0m", LAB_I2C_USER_PASSWORD_LEN);
  SPY_UART_SetInput("burn3d_1n_th3_r0m", NULL);

  Utils_Shell_StatusTypeDef result = Lab_I2C_Cmd_Login(NULL);

  TEST_ASSERT_EQUAL(UTILS_SHELL_EXIT, result);
  TEST_ASSERT_NOT_NULL(strstr(SPY_UART_Buffer, "Logged in successfully"));
  TEST_ASSERT_EQUAL(SPY_Screen_AccessDenied_Calls, 0);
}

void test_Lab_I2C_Cmd_Login_should_failwrongpass(void) {
  strncpy((char*)&SPY_EEPROM_Buffer[0x00], "burn3d_1n_th3_r0m", LAB_I2C_USER_PASSWORD_LEN);
  SPY_UART_SetInput("wr0ng_p4ss", NULL);

  Utils_Shell_StatusTypeDef result = Lab_I2C_Cmd_Login(NULL);

  TEST_ASSERT_EQUAL(UTILS_SHELL_CONTINUE, result);
  TEST_ASSERT_NOT_NULL(strstr(SPY_UART_Buffer, "Incorrect password"));
  TEST_ASSERT_EQUAL(SPY_Screen_AccessDenied_Calls, 1);
  TEST_ASSERT_EQUAL(SPY_Screen_Anonymous_Calls, 1);
}

void test_Lab_I2C_Cmd_Login_should_failhardware(void) {
  SPY_I2C_Mock_Status = HAL_ERROR;
  strncpy((char*)&SPY_EEPROM_Buffer[0x00], "burn3d_1n_th3_r0m", LAB_I2C_USER_PASSWORD_LEN);
  SPY_UART_SetInput("burn3d_1n_th3_r0m", NULL);

  Utils_Shell_StatusTypeDef result = Lab_I2C_Cmd_Login(NULL);

  TEST_ASSERT_EQUAL(UTILS_SHELL_CONTINUE, result);
  TEST_ASSERT_NOT_NULL(strstr(SPY_UART_Buffer, "An error occured"));
  TEST_ASSERT_EQUAL(SPY_Screen_AccessDenied_Calls, 1);
  TEST_ASSERT_EQUAL(SPY_Screen_Anonymous_Calls, 1);
}

void test_Lab_I2C_Cmd_Root_should_succeed(void) {
  SPY_EEPROM_Buffer[0x30] = 0xca;
  SPY_EEPROM_Buffer[0x31] = 0xfe;

  Utils_Shell_StatusTypeDef result = Lab_I2C_Cmd_Root(NULL);

  TEST_ASSERT_EQUAL(UTILS_SHELL_EXIT, result);
  TEST_ASSERT_NOT_NULL(strstr(SPY_UART_Buffer, "Found magic bytes 0xcafe"));
  TEST_ASSERT_NOT_NULL(strstr(SPY_UART_Buffer, "Root authorization granted"));
  TEST_ASSERT_EQUAL(SPY_Screen_AccessDenied_Calls, 0);
}

void test_Lab_I2C_Cmd_Root_should_failwrongmagic(void) {
  SPY_EEPROM_Buffer[0x30] = 0xfe;
  SPY_EEPROM_Buffer[0x31] = 0xca;

  Utils_Shell_StatusTypeDef result = Lab_I2C_Cmd_Root(NULL);

  TEST_ASSERT_EQUAL(UTILS_SHELL_CONTINUE, result);
  TEST_ASSERT_NOT_NULL(strstr(SPY_UART_Buffer, "Found magic bytes 0xfeca"));
  TEST_ASSERT_NOT_NULL(strstr(SPY_UART_Buffer, "Authorization refused"));
  TEST_ASSERT_EQUAL(SPY_Screen_AccessDenied_Calls, 1);
  TEST_ASSERT_EQUAL(SPY_Screen_User_Calls, 1);
}

void test_Lab_I2C_Cmd_Root_should_failhardware(void) {
  SPY_I2C_Mock_Status = HAL_ERROR;
  SPY_EEPROM_Buffer[0x30] = 0xca;
  SPY_EEPROM_Buffer[0x31] = 0xfe;

  Utils_Shell_StatusTypeDef result = Lab_I2C_Cmd_Root(NULL);

  TEST_ASSERT_EQUAL(UTILS_SHELL_CONTINUE, result);
  TEST_ASSERT_NULL(strstr(SPY_UART_Buffer, "Found magic bytes")); // Ensure it returns before this
  TEST_ASSERT_NOT_NULL(strstr(SPY_UART_Buffer, "An error occured"));
  TEST_ASSERT_EQUAL(SPY_Screen_AccessDenied_Calls, 1);
  TEST_ASSERT_EQUAL(SPY_Screen_User_Calls, 1);
}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_Lab_I2C_Cmd_Login_should_succeed);
  RUN_TEST(test_Lab_I2C_Cmd_Login_should_failwrongpass);
  RUN_TEST(test_Lab_I2C_Cmd_Login_should_failhardware);
  RUN_TEST(test_Lab_I2C_Cmd_Root_should_succeed);
  RUN_TEST(test_Lab_I2C_Cmd_Root_should_failwrongmagic);
  RUN_TEST(test_Lab_I2C_Cmd_Root_should_failhardware);

  return UNITY_END();
}
