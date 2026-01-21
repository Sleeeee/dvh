#include "lab_registry.h"
#include <stddef.h>

#include "00_swd/lab_swd.h"
#include "01_uart/lab_uart.h"

static ILab* labs[] = {
  &Lab_SWD,
  &Lab_UART,
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
