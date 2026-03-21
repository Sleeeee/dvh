#include "lab_i2c.h"
#include "lab_i2c_data.h"
#include "utils_eeprom.h"
#include "utils_uart.h"
#include "utils_shell.h"
#include "utils_secrets.h"
#include "main.h"
#include <string.h>
#include <stdio.h>

Lab_StatusTypeDef Lab_I2C_Init(void) {
  HAL_Delay(500);
  uint8_t password[LAB_I2C_USER_PASSWORD_LEN + 1];
  memset(password, 0, sizeof(password));

  if (Utils_EEPROM_Read(0x00, password, LAB_I2C_USER_PASSWORD_LEN) != UTILS_EEPROM_OK) {
    Utils_UART_Writeline("An error occured while fetching data from the EEPROM.\r\n");
    return LAB_ERROR;
  }
  memset(password, 0, sizeof(password));
  return LAB_OK;
}

void Lab_I2C_Loop(void) {
  Utils_UART_ReceiveEnter();
  Utils_UART_Writeline("[INFO] Successfully fetched user password from the EEPROM.\r\n");
  Utils_Shell_Start("[anonymous@dvh]$ ", LAB_I2C_COMMANDS_ANONYMOUS);

  Utils_Secrets_Transmit_Flag(LAB_I2C_FLAG_ONE, LAB_I2C_FLAG_ONE_LEN);
  Utils_Shell_Start("[user@dvh]$ ", LAB_I2C_COMMANDS_USER);

  // Flag two is planted directly inside the EEPROM, along with instructions to progress to flag three

  Utils_Secrets_Transmit_Flag(LAB_I2C_FLAG_THREE, LAB_I2C_FLAG_THREE_LEN);
  Utils_Shell_Start("[root@dvh]# ", LAB_I2C_COMMANDS_ROOT);
}

Lab_StatusTypeDef Lab_I2C_Reset(void) {
  if (Utils_EEPROM_IsConnected() != UTILS_EEPROM_OK) { return LAB_ERROR; }
  if (Utils_EEPROM_EraseAll() != UTILS_EEPROM_OK) { return LAB_ERROR; }

  char secret[128];
  Utils_Secrets_Decrypt(LAB_I2C_USER_PASSWORD, LAB_I2C_USER_PASSWORD_LEN, secret, sizeof(secret));

  // Include string terminator in write
  if (Utils_EEPROM_Write(0x00, (uint8_t*)secret, strlen(secret) + 1) != UTILS_EEPROM_OK) { return LAB_ERROR; }
  memset(secret, 0, sizeof(secret));

  char decrypted_flag[LAB_I2C_FLAG_TWO_LEN + 1];
  char decrypted_note[LAB_I2C_CONFIG_NOTE_LEN + 1];
  Utils_Secrets_Decrypt(LAB_I2C_FLAG_TWO, LAB_I2C_FLAG_TWO_LEN, decrypted_flag, sizeof(decrypted_flag));
  Utils_Secrets_Decrypt(LAB_I2C_CONFIG_NOTE, LAB_I2C_CONFIG_NOTE_LEN, decrypted_note, sizeof(decrypted_note));
  snprintf(secret, sizeof(secret), "%s%s", decrypted_note, decrypted_flag);

  if (Utils_EEPROM_Write(0x80, (uint8_t*)secret, strlen(secret) + 1) != UTILS_EEPROM_OK) { return LAB_ERROR; }
  memset(secret, 0, sizeof(secret));
  memset(decrypted_flag, 0, sizeof(decrypted_flag));
  memset(decrypted_note, 0, sizeof(decrypted_note));

  return LAB_OK;
}

ILab Lab_I2C = {
  .id = 2,
  .name = "I2C",
  .init = Lab_I2C_Init,
  .loop = Lab_I2C_Loop,
  .reset = Lab_I2C_Reset
};
