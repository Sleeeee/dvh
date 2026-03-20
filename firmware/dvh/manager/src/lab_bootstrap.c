#include "lab_bootstrap.h"
#include "lab_select.h"
#include "lab_blink.h"
#include "lab_registry.h"
#include "ilab.h"
#include "main.h"

void LabBootstrap_Start(void) {
  LabSelect_Init(); // WARNING: DVH 0.1 GPIO init
  HAL_Delay(100);

  uint8_t lab_id = LabSelect_Read();
  LabBlink_Selected(lab_id);
  ILab* lab = LabRegistry_GetById(lab_id);

  if (lab == NULL) {
    while (1) { LabBlink_Broken(); } // Unable to load a lab
  }

  if (LabSelect_Reset_Pressed()) {
    LabBlink_Reset();
    if (lab->reset() != LAB_OK) {
      while (1) { LabBlink_Broken(); } // Lab reset failed
    }
    LabBlink_Continue();
  }

  lab->init();
  while (1) {
    lab->loop();
  }
}
