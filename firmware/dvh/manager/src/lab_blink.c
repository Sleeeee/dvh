#include "lab_blink.h"
#include "main.h"

void LabBlink_Selected(uint8_t count) {
  for (int i = 0; i < count; i++) {
    HAL_GPIO_WritePin(DOOR_IN_GPIO_Port, DOOR_IN_Pin, GPIO_PIN_SET);
    HAL_Delay(200);
    HAL_GPIO_WritePin(DOOR_IN_GPIO_Port, DOOR_IN_Pin, GPIO_PIN_RESET);
    HAL_Delay(200);
  }
}

void LabBlink_Not_Found(void) {
  HAL_GPIO_WritePin(DOOR_IN_GPIO_Port, DOOR_IN_Pin, GPIO_PIN_SET);
  HAL_Delay(1000);
}

void LabBlink_Reset(void) {
  HAL_GPIO_WritePin(DOOR_IN_GPIO_Port, DOOR_IN_Pin, GPIO_PIN_SET);
  HAL_Delay(1000);
  HAL_GPIO_WritePin(DOOR_IN_GPIO_Port, DOOR_IN_Pin, GPIO_PIN_RESET);
}

void LabBlink_Continue(void) {
  for (int i = 0; i < 3; i++) {
    HAL_GPIO_WritePin(DOOR_IN_GPIO_Port, DOOR_IN_Pin, GPIO_PIN_SET);
    HAL_Delay(50);
    HAL_GPIO_WritePin(DOOR_IN_GPIO_Port, DOOR_IN_Pin, GPIO_PIN_RESET);
    HAL_Delay(50);
  }
}
