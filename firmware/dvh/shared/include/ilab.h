#ifndef ILAB_H
#define ILAB_H

#include "lab_bootstrap.h"
#include <stdint.h>

typedef enum {
  LAB_OK,
  LAB_ERROR
} Lab_StatusTypeDef;

typedef struct {
  uint8_t id;
  const char* name;

  void (*init)(void);
  void (*loop)(void);
  Lab_StatusTypeDef (*reset)(void);
} ILab;

#endif
