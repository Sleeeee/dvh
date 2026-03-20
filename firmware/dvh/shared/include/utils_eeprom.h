#ifndef UTILS_EEPROM_H
#define UTILS_EEPROM_H

#include <stdint.h>

// Configuration for M24C02-W
#define UTILS_EEPROM_I2C_ADDR (0x50 << 1) // 0xa0
#define UTILS_EEPROM_SIZE 256
#define UTILS_EEPROM_PAGE_SIZE 16
#define UTILS_EEPROM_TIMEOUT 100

typedef enum {
  UTILS_EEPROM_OK,
  UTILS_EEPROM_ERROR
} Utils_EEPROM_StatusTypeDef;

Utils_EEPROM_StatusTypeDef Utils_EEPROM_IsConnected(void);

Utils_EEPROM_StatusTypeDef Utils_EEPROM_Read(uint8_t address, uint8_t *data, uint16_t len);

Utils_EEPROM_StatusTypeDef Utils_EEPROM_Write(uint8_t address, uint8_t *data, uint16_t len);

Utils_EEPROM_StatusTypeDef Utils_EEPROM_EraseAll(void);

#endif
