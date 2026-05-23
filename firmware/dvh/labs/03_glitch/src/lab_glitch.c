#include "lab_glitch.h"
#include "lab_glitch_data.h"
#include "attiny.h"
#include "utils_secrets.h"
#include "utils_isp.h"
#include "utils_screen.h"
#include "main.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

extern UART_HandleTypeDef huart2;

Lab_StatusTypeDef Lab_Glitch_Init(void) {
  HAL_GPIO_WritePin(ATTINY_RST_GPIO_Port, ATTINY_RST_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LDO_EN_GPIO_Port, LDO_EN_Pin, GPIO_PIN_SET);
  HAL_Delay(100);
  return LAB_OK;
}

void Lab_Glitch_Loop(void) {
  static bool ui_printed = false;
  static bool browned_out = false;

  static uint32_t last_dot = 0;
  static uint32_t last_corrupt = 0;

  uint8_t c;

  if (!ui_printed) {
    Utils_Screen_Fill_Write("[ATTINY] Running integrity checks", UTILS_SCREEN_STANDARD);
    ui_printed = true;
  }
  HAL_StatusTypeDef status = HAL_UART_Receive(&huart2, &c, 1, 100);

  if (status == HAL_OK) {
    if ((char)c == 0xc0) {
      if (HAL_GetTick() > (last_dot + 500)) {
        Utils_Screen_WriteChar('.', UTILS_SCREEN_STANDARD);
        last_dot = HAL_GetTick();
      }
      if (HAL_GetTick() > (last_corrupt + 2500)) {
        Utils_Screen_Write("CORRUPTED, system will reboot now", UTILS_SCREEN_WARNING);
        last_corrupt = HAL_GetTick();
        ui_printed = false;
        HAL_Delay(1000);
      }
    }

    else if ((char)c == 0xca) {
      Utils_Screen_Write("UNSTABLE, ATTiny crashed", UTILS_SCREEN_WARNING);

      if (!browned_out) {
        char flag[64];
        Utils_Secrets_Decrypt(LAB_GLITCH_FLAG_ONE, LAB_GLITCH_FLAG_ONE_LEN, flag, sizeof(flag));
        Utils_Screen_Write(flag, UTILS_SCREEN_WARNING);

        browned_out = true;
        HAL_Delay(20000);
      }

    } else if ((char)c == 0xcb) {
      Utils_Screen_Write("VERIFIED, system is stable", UTILS_SCREEN_STANDARD);

      char flag[64];
      Utils_Secrets_Decrypt(LAB_GLITCH_FLAG_TWO, LAB_GLITCH_FLAG_TWO_LEN, flag, sizeof(flag));
      Utils_Screen_Write(flag, UTILS_SCREEN_WARNING);

      while (1) {
        HAL_Delay(20000); // Lab is over for now
      }

    } else {
      Utils_Screen_WriteChar((char)c, UTILS_SCREEN_WARNING);
    }

  } else if (status == HAL_TIMEOUT) {
    Utils_Screen_Fill_Write("[CRITICAL] ATTiny UART timeout", UTILS_SCREEN_WARNING);
  } else if (status == HAL_ERROR) {
    Utils_Screen_Fill_Write("[CRITICAL] UART hardware error", UTILS_SCREEN_WARNING);
    ui_printed = false;
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
