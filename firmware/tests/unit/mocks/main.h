#ifndef MAIN_H
#define MAIN_H

#include "stm32f1xx_hal.h"

#define GPIOZ ((GPIO_TypeDef*)0)

#define DOOR_OUT_Pin 0
#define DOOR_OUT_GPIO_Port GPIOZ

#define LAB_RST_Pin 0
#define LAB_RST_GPIO_Port GPIOZ
#define LAB_S0_Pin 0
#define LAB_S0_GPIO_Port GPIOZ
#define LAB_S1_Pin 0
#define LAB_S1_GPIO_Port GPIOZ
#define LAB_S2_Pin 0
#define LAB_S2_GPIO_Port GPIOZ
#define LAB_S3_Pin 0
#define LAB_S3_GPIO_Port GPIOZ

#endif
