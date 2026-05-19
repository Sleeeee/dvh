#include "lab_bootstrap.h"
#include "lab_select.h"
#include "lab_blink.h"
#include "lab_registry.h"
#include "ilab.h"
#include "utils_screen.h"
#include "main.h"

void LabBootstrap_Start(void) {
  Utils_Screen_Welcome();

  uint8_t lab_id = LabSelect_Read();
  Utils_Screen_Display_Lab(lab_id);
  LabBlink_Selected(lab_id);
  ILab* lab = LabRegistry_GetById(lab_id);

  if (lab == NULL) {
    // Unable to load a lab
    Utils_Screen_Lab_Not_Found();
    while (1) {
      LabBlink_Broken();
    }
  }

  if (LabSelect_Reset_Pressed()) {
    Utils_Screen_Lab_Reset();
    LabBlink_Reset();

    if (lab->reset() != LAB_OK) {
      // Lab reset failed
      Utils_Screen_Lab_Reset_Failed();
      while (1) {
        LabBlink_Broken();
      }
    }
    LabBlink_Continue();
  }

  if (lab->init() != LAB_OK) {
    // Lab init failed
    Utils_Screen_Lab_Init_Failed();
    while (1) {
      LabBlink_Broken();
    }
  }

  HAL_Delay(2000); // Artificial delay for UX
  while (1) {
    lab->loop();
  }
}
