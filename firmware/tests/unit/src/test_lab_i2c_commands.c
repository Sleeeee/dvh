#include "../lib/unity.h"
#include "../../../dvh/labs/02_i2c/src/lab_i2c_commands.c"
#include "../../../dvh/labs/02_i2c/src/lab_i2c_data.c"
#include "../../../dvh/shared/src/utils_eeprom.c"
#include "../../../dvh/shared/src/utils_shell.c"
#include "../../../dvh/shared/src/utils_secrets.c"
#include "../mocks/mock_utils_uart.c"
#include "stm32f1xx_hal.h"
#include <string.h>

void setUp(void) {}

void tearDown(void) {}

int main(void) {
  UNITY_BEGIN();
  return UNITY_END();
}
