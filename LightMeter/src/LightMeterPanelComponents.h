#ifndef LIGHT_METER_PANEL_COMPONENTS_H
#define LIGHT_METER_PANEL_COMPONENTS_H

// Extends "HardwareInterfaces.h"

#include "HardwareInterfaces.h"
#include "LightMeterHardwareInterfaces.h"
#include "PanelComponents.h"
#include "stdint.h"
#include "timeKeeper.h"

//---Knob--------------------------------------------------------
class RotoryEncoder : public PanelComponent {
public:
  RotoryEncoder(void);
  void setHardware(GenericHardwareDescription *input);
  bool hasFreshData(void);
  void freshen(uint16_t msTickDelta);
  int16_t getDiff(void);
  void clearDiff(void);
  void removeDiff(uint16_t deltaDiff);
  uint8_t serviceChanged(void);

private:
  int16_t diff;
  int16_t lastInput;
  int16_t state;
  const uint8_t hysteresis = 8;
  //	int8_t lastSlope = 1;
  uint8_t newData;
};

#endif