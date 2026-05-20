#include "lab_glitch.h"
#include "attiny.h"
#include "utils_isp.h"
#include "utils_screen.h"
#include "main.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

extern UART_HandleTypeDef huart2;

Lab_StatusTypeDef Lab_Glitch_Init(void) {
  HAL_GPIO_WritePin(LDO_EN_GPIO_Port, LDO_EN_Pin, GPIO_PIN_SET);
  HAL_Delay(100);

  return LAB_OK;
}

void Lab_Glitch_Loop(void) {
  uint8_t buffer[64] = {0};
  HAL_UART_Receive(&huart2, buffer, sizeof(buffer) - 1, 3000);

  if (strstr((char*)buffer, "Hello") != NULL) {
    Utils_Screen_Fill_Write((char *)buffer, UTILS_SCREEN_WARNING);
  } else {
    HAL_GPIO_WritePin(DOOR_OUT_GPIO_Port, DOOR_OUT_Pin, GPIO_PIN_SET); 
  }
}

Lab_StatusTypeDef Lab_Glitch_Reset(void) {
  // Power on ATTiny while in RESET mode
  HAL_GPIO_WritePin(ATTINY_RST_GPIO_Port, ATTINY_RST_Pin, GPIO_PIN_RESET);
  HAL_Delay(100);
  HAL_GPIO_WritePin(LDO_EN_GPIO_Port, LDO_EN_Pin, GPIO_PIN_SET);
  HAL_Delay(20);

  if (Utils_ISP_ProgrammingEnable_Retry(20) != UTILS_ISP_OK) {
    HAL_GPIO_WritePin(ATTINY_RST_GPIO_Port, ATTINY_RST_Pin, GPIO_PIN_SET); 
    return LAB_ERROR;
  }

  // Ensure chip is correct and working
  uint8_t device_code[3];
  Utils_ISP_ReadDeviceCode(device_code, sizeof(device_code));
  if ((device_code[0] != 0x1e) || (device_code[1] != 0x93) || (device_code[2] != 0x0b)) {
    return LAB_ERROR;
  }

  if (Utils_ISP_ChipErase() != UTILS_ISP_OK) {
    return LAB_ERROR;
  }
  HAL_Delay(20);
  HAL_GPIO_WritePin(ATTINY_RST_GPIO_Port, ATTINY_RST_Pin, GPIO_PIN_SET);
  HAL_Delay(5);
  HAL_GPIO_WritePin(ATTINY_RST_GPIO_Port, ATTINY_RST_Pin, GPIO_PIN_RESET);
  HAL_Delay(20);

  if (Utils_ISP_ProgrammingEnable_Retry(20) != UTILS_ISP_OK) {
    HAL_GPIO_WritePin(ATTINY_RST_GPIO_Port, ATTINY_RST_Pin, GPIO_PIN_SET); 
    return LAB_ERROR;
  }

  Utils_ISP_Write(attiny_bin, attiny_bin_len);
  HAL_GPIO_WritePin(ATTINY_RST_GPIO_Port, ATTINY_RST_Pin, GPIO_PIN_SET);
  return LAB_OK;
}

ILab Lab_Glitch = {
  .id = 3,
  .name = "Voltage glitching",
  .init = Lab_Glitch_Init,
  .loop = Lab_Glitch_Loop,
  .reset = Lab_Glitch_Reset
};
