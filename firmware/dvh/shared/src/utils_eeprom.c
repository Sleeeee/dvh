#include "utils_eeprom.h"
#include "main.h"
#include <string.h>

extern I2C_HandleTypeDef hi2c2;

Utils_EEPROM_StatusTypeDef Utils_EEPROM_Wait(void) {
  uint32_t time = HAL_GetTick();
  while (HAL_I2C_IsDeviceReady(&hi2c2, UTILS_EEPROM_I2C_ADDR, 1, UTILS_EEPROM_TIMEOUT) != HAL_OK) {
    if ((HAL_GetTick() - time) > UTILS_EEPROM_TIMEOUT) { return UTILS_EEPROM_ERROR; }
  }
  return UTILS_EEPROM_OK;
}

Utils_EEPROM_StatusTypeDef Utils_EEPROM_IsConnected(void) {
  HAL_StatusTypeDef status = HAL_I2C_IsDeviceReady(&hi2c2, UTILS_EEPROM_I2C_ADDR, 2, UTILS_EEPROM_TIMEOUT);
  return (status == HAL_OK) ? UTILS_EEPROM_OK : UTILS_EEPROM_ERROR;
}

Utils_EEPROM_StatusTypeDef Utils_EEPROM_Read(uint8_t address, uint8_t *data, uint16_t len) {
  HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c2, UTILS_EEPROM_I2C_ADDR, address, I2C_MEMADD_SIZE_8BIT, data, len, UTILS_EEPROM_TIMEOUT);
  return (status == HAL_OK) ? UTILS_EEPROM_OK : UTILS_EEPROM_ERROR;
}

// WARNING : this does not account for WP (not linked to a DVH 0.1 GPIO)
Utils_EEPROM_StatusTypeDef Utils_EEPROM_Write(uint8_t address, uint8_t *data, uint16_t len) {
  uint8_t page_start = address / UTILS_EEPROM_PAGE_SIZE;
  uint8_t page_end = (address + len - 1) / UTILS_EEPROM_PAGE_SIZE;
  uint16_t offset = 0;

  for (uint8_t p = page_start; p <= page_end; p++) {
    uint8_t start = (p == page_start) ? address : p * UTILS_EEPROM_PAGE_SIZE;
    uint16_t write_len = (p == page_end) ? len - offset : ((p + 1) * UTILS_EEPROM_PAGE_SIZE) - start;

    HAL_StatusTypeDef status = HAL_I2C_Mem_Write(&hi2c2, UTILS_EEPROM_I2C_ADDR, start, I2C_MEMADD_SIZE_8BIT, data + offset, write_len, UTILS_EEPROM_TIMEOUT);

    if (status != HAL_OK) { return UTILS_EEPROM_ERROR; }

    offset += write_len;
    if (Utils_EEPROM_Wait() != UTILS_EEPROM_OK) { return UTILS_EEPROM_ERROR; }
  }
  return UTILS_EEPROM_OK;
}

Utils_EEPROM_StatusTypeDef Utils_EEPROM_EraseAll(void) {
  uint8_t blank_page[UTILS_EEPROM_PAGE_SIZE];
  memset(blank_page, 0xff, sizeof(blank_page));

  for (uint8_t i = 0; i < (UTILS_EEPROM_SIZE / UTILS_EEPROM_PAGE_SIZE); i++) {
    uint8_t start = i * UTILS_EEPROM_PAGE_SIZE;
    Utils_EEPROM_StatusTypeDef status = Utils_EEPROM_Write(start, blank_page, UTILS_EEPROM_PAGE_SIZE);

    if (status != UTILS_EEPROM_OK) { return UTILS_EEPROM_ERROR; }
  }
  return UTILS_EEPROM_OK;
}
