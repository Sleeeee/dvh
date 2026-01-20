#ifndef ILAB_H
#define ILAB_H

#include <stdint.h>

typedef struct ILab ILab;

struct ILab {
  uint8_t id;
  const char* name;

  void (*init)(void);
  void (*loop)(void);
};

#endif
