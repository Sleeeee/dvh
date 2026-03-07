#include "lab_registry.h"
#include <stddef.h>

#include "lab_swd.h"
#include "lab_uart.h"
#include "lab_i2c.h"

static ILab* labs[] = {
  &Lab_SWD,
  &Lab_UART,
  &Lab_I2C,
  NULL
};

ILab* LabRegistry_GetById(uint8_t id) {
  for (int i = 0; labs[i] != NULL; i++) {
    if (labs[i]->id == id) {
      return labs[i];
    }
  }
  return NULL;
}
