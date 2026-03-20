#include "lab_i2c.h"
#include "lab_i2c_data.h"
#include "utils_eeprom.h"
#include "utils_secrets.h"
#include "main.h"
#include <string.h>

void Lab_I2C_Init(void) {
  // Nothing to do yet
}

void Lab_I2C_Loop(void) {
  // Nothing to do yet
}

Lab_StatusTypeDef Lab_I2C_Reset(void) {
  char decrypted_flag[64];
  Utils_Secrets_Decrypt(LAB_I2C_FLAG_ONE, LAB_I2C_FLAG_ONE_LEN, decrypted_flag, sizeof(decrypted_flag));

  if (Utils_EEPROM_IsConnected() != UTILS_EEPROM_OK) { return LAB_ERROR; }

  // Include string terminator in write
  if (Utils_EEPROM_Write(0x00, (uint8_t*)decrypted_flag, strlen(decrypted_flag) + 1) != UTILS_EEPROM_OK) { return LAB_ERROR; }

  memset(decrypted_flag, 0, sizeof(decrypted_flag));
  return LAB_OK;
}

ILab Lab_I2C = {
  .id = 2,
  .name = "I2C",
  .init = Lab_I2C_Init,
  .loop = Lab_I2C_Loop,
  .reset = Lab_I2C_Reset
};
