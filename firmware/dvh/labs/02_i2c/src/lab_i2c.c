#include "lab_i2c.h"
#include "main.h"

void Lab_I2C_Init(void) {
  // Nothing to do yet
}

void Lab_I2C_Loop(void) {
  // Nothing to do yet
}

void Lab_I2C_Reset(void) {
  HAL_Delay(2000);
}

ILab Lab_I2C = {
  .id = 2,
  .name = "I2C",
  .init = Lab_I2C_Init,
  .loop = Lab_I2C_Loop,
  .reset = Lab_I2C_Reset
};
