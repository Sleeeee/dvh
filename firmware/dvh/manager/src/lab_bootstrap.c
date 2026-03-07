#include "lab_bootstrap.h"
#include "lab_select.h"
#include "lab_blink.h"
#include "lab_registry.h"
#include "main.h"

void LabBootstrap_Start(void) {
  LabSelect_Init(); // WARNING: DVH 0.1 GPIO init
  HAL_Delay(100);

  uint8_t lab_id = LabSelect_Read();
  LabBlink_Selected(lab_id);
  ILab* lab = LabRegistry_GetById(lab_id);

  if (lab == NULL) {
    while (1) {
      // Unable to load a lab
      LabBlink_Not_Found();
    }
  }

  if (LabSelect_Reset_Pressed()) {
    LabBlink_Reset();
    lab->reset();
    LabBlink_Continue();
  }

  lab->init();
  while (1) {
    lab->loop();
  }
}
