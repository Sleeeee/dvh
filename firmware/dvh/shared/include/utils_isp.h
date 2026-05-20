#ifndef UTILS_ISP_H
#define UTILS_ISP_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef enum {
  UTILS_ISP_OK,
  UTILS_ISP_ERROR
} Utils_ISP_StatusTypeDef;

Utils_ISP_StatusTypeDef Utils_ISP_ProgrammingEnable(void);

Utils_ISP_StatusTypeDef Utils_ISP_ProgrammingEnable_Retry(uint8_t count);

void Utils_ISP_ReadDeviceCode(uint8_t* device_code, size_t size);

Utils_ISP_StatusTypeDef Utils_ISP_ChipErase(void);

void Utils_ISP_Write(unsigned char* data, unsigned int len);

#endif
