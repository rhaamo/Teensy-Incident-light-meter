#include <Arduino.h>

#include "LuxPanel.h"

#define KValue 16

extern OLEDFunctions oled;

// Create an SFE_TSL2561 object, here called "light":
extern SFE_TSL2561 light;
extern boolean gain;    // Gain setting, 0 = X1, 1 = X16;
extern unsigned int ms; // Integration ("shutter") time in milliseconds

const float fStopTable[24] = {1.4, 1.8, 2.0, 2.2, 2.5, 2.8, 3.2, 3.5,
                              4,   4.5, 5,   5.6, 6.3, 7.1, 8,   9,
                              10,  11,  13,  14,  16,  18,  20,  22};
const float exposureTable[3] = {0.041666666, 0.033333333, 0.016666666};
const float isoTable[7] = {100, 200, 400, 800, 1600, 3200, 6400};
uint8_t isoTableSize = 7;

extern const uint8_t PIN_POWER_ON = 16;

LuxPanel::LuxPanel(void) {
  upButton.setHardware(new ArduinoDigitalIn(22), 1);
  add(&upButton);

  downButton.setHardware(new ArduinoDigitalIn(23), 1);
  add(&downButton);

  encButton.setHardware(new ArduinoDigitalIn(4), 1);
  add(&encButton);

  dataWheel.setHardware(new TeensyEncoderIn(8, 9));
  add(&dataWheel);

  fStopSetting = 7;
  exposureSetting = 1;
  isoSetting = 3;
  state = PInit;
  good = false;

  luxAccumulator = 0;
  sampleCount = 0;
  lux = 0;

  triggerState = hSRun;

  lipoGood = false;
}

void LuxPanel::tickStateMachine(int msTicksDelta) {
  freshenComponents(msTicksDelta);

  bool drawDisplay = false;
  unsigned int data0, data1;
  drawDisplay = oled.drawBatteryWidget();

  //***** PROCESS THE LOGIC *****//
  // Now do the states.
  PState_t nextState = state;
  switch (state) {
  case PInit:
    // Can't be running, if button pressed move on
    oled.clear(PAGE);
    drawDisplay = true;
    nextState = PDisplayPhotoValueInit;
    break;
  case PDisplayPhotoValueInit:
    drawDisplay = true;
    oled.clear(PAGE);
    oled.drawMenu("Photo", isoTable[isoSetting], false, true);
    nextState = PDisplayPhotoValue;
    break;
  case PDisplayPhotoValue:
    if (dataWheel.serviceChanged()) {
      if (dataWheel.getDiff() > 0) {
        if (fStopSetting < 23) {
          fStopSetting++;
        }
        dataWheel.removeDiff(7);
      }
      if (dataWheel.getDiff() < 0) {
        if (fStopSetting > 0) {
          fStopSetting--;
        }
        dataWheel.removeDiff(7);
      }
    }
    if (upButton.serviceRisingEdge()) {
      nextState = PDisplayVideoValueInit;
    } else {
      if (light.getData(data0, data1)) {
        // get the lux
        double tempLux;
        good = light.getLux(gain, ms, data0, data1,
                            tempLux); // True if neither sensor is saturated
        updateLux(tempLux);
        // exposure time = stop squared * K / ( ISO * Lux )
        float T = pow(fStopTable[fStopSetting], 2) * KValue /
                  (lux * (isoTable[isoSetting]));
        oled.eraseLowerArea();
        oled.drawLeftBracket(32, 10);
        oled.drawRightBracket(122, 10);
        oled.drawFNumStyle1(fStopTable[fStopSetting], 2, 13);
        oled.drawExposureStyle1(T, 60, 14);
        if (good == false) {
          oled.drawNo(33, 12);
        }
        if (triggerState == hSHeld) {
          oled.holdStyle1(100, 10);
        }
        if (triggerState == hSAveraging) {
          oled.aveStyle1(100, 10);
        }
        oled.setCursor(111, 17);
        oled.print("S");
        drawDisplay = true;
      }
    }
    downButton.serviceRisingEdge();
    break;
  case PDisplayVideoValueInit:
    drawDisplay = true;
    oled.clear(PAGE);
    oled.drawMenu("Video", isoTable[isoSetting], true, true);

    // oled.drawBrackets();
    nextState = PDisplayVideoValue;
    break;
  case PDisplayVideoValue:
    if (dataWheel.serviceChanged()) {
      if (dataWheel.getDiff() > 0) {
        if (exposureSetting < 2) {
          exposureSetting++;
        }
        dataWheel.removeDiff(7);
      }
      if (dataWheel.getDiff() < 0) {
        if (exposureSetting > 0) {
          exposureSetting--;
        }
        dataWheel.removeDiff(7);
      }
    }
    if (downButton.serviceRisingEdge()) {
      nextState = PDisplayPhotoValueInit;
    } else if (upButton.serviceRisingEdge()) {
      nextState = PSetISOInit;
    } else {
      if (light.getData(data0, data1)) {
        // get the lux
        double tempLux;
        good = light.getLux(gain, ms, data0, data1,
                            tempLux); // True if neither sensor is saturated
        updateLux(tempLux);
        // exposure time = stop squared * K / ( ISO * Lux )
        // float T =
        // pow(fStopTable[fStopSetting],2)*KValue/(lux*(isoTable[isoSetting]));
        float N =
            sqrt((exposureTable[exposureSetting] * lux * isoTable[isoSetting]) /
                 KValue);
        oled.eraseLowerArea();
        oled.drawLeftBracket(32, 10);
        oled.drawRightBracket(122, 10);
        oled.drawExposureStyle2(exposureTable[exposureSetting], 2, 13);
        oled.drawFNumStyle2(N, 60, 14);
        if (good == false) {
          oled.drawNo(33, 12);
        }
        if (triggerState == hSHeld) {
          oled.holdStyle1(100, 10);
        }
        if (triggerState == hSAveraging) {
          oled.aveStyle1(100, 10);
        }
        drawDisplay = true;
      }
    }
    break;
  case PSetISOInit:
    dataWheel.clearDiff();
    drawDisplay = true;
    oled.clear(PAGE);
    // menu part
    oled.drawMenu("ISO", true, true);

    // Draw last value
    oled.drawISOScale(isoSetting);

    nextState = PSetISO;
    break;
  case PSetISO:
    if (dataWheel.serviceChanged()) {
      drawDisplay = true;
      // Erase part
      oled.setCursor(0, 11);
      oled.print("      ");

      if (dataWheel.getDiff() > 0) {
        if (isoSetting < 6) {
          isoSetting++;
        }
        dataWheel.removeDiff(8);
      }
      if (dataWheel.getDiff() < 0) {
        if (isoSetting > 0) {
          isoSetting--;
        }
        dataWheel.removeDiff(8);
      }
      // Draw new value
      oled.drawISOScale(isoSetting);
    }
    if (downButton.serviceRisingEdge()) {
      nextState = PDisplayVideoValueInit;
    }
    if (upButton.serviceRisingEdge()) {
      nextState = pSystemInit;
    }
    break;
  case pSystemInit:
    dataWheel.clearDiff();
    drawDisplay = true;
    oled.clear(PAGE);
    // menu part
    oled.drawMenu("System", true, false);

    // Draw last value
    oled.setCursor(64, 11);
    oled.print((float)lipo.voltage() / 1000, 2);
    oled.print("V");

    oled.setCursor(0, 20);
    oled.print((float)lipo.soc(), 0);
    oled.print("%");

    // Clear wheel state
    dataWheel.serviceChanged();
    nextState = pSystem;
    break;
  case pSystem:
    // Draw new value
    if (lipoGood == true) {
      oled.setCursor(0, 11);
      oled.print("OK!");
    } else {
      oled.setCursor(0, 11);
      oled.print("NOBATT");
    }
    oled.setCursor(64, 20);
    oled.print(lipo.current(AVG));
    oled.print("mA    ");
    oled.setCursor(0, 20);
    oled.print((float)lipo.soc(), 0);
    oled.print("%");
    oled.setCursor(64, 11);
    oled.print((float)lipo.voltage() / 1000, 2);
    if (encButton.serviceRisingEdge()) {
      oled.clear(PAGE);
      oled.setFontType(1);
      oled.setCursor(6, 6);
      oled.print("Power down.");
      oled.display();
      delay(1000);
      digitalWrite(PIN_POWER_ON, LOW);
      delay(4000);
      oled.clear(PAGE);
      oled.setCursor(6, 6);
      oled.print("Failed!!!");
      oled.display();
      delay(1000);
      oled.setFontType(0);
      oled.clear(PAGE);
      oled.display();
      nextState = pSystemInit;
    }
    drawDisplay = true;
    if (downButton.serviceRisingEdge()) {
      nextState = PSetISOInit;
    }
    if (upButton.serviceRisingEdge()) {
      nextState = PDisplayLuxValueInit;
    }
    break;
  case PDisplayLuxValueInit:
    drawDisplay = true;
    oled.clear(PAGE);
    // menu part
    oled.drawMenu("Lux", false, false);
    nextState = PDisplayLuxValue;
    break;
  case PDisplayLuxValue:
    if (downButton.serviceRisingEdge()) {
      nextState = pSystemInit;
    }
    if (upButton.serviceRisingEdge()) {
      nextState = pMaryInit;
    }
    if (light.getData(data0, data1)) {
      drawDisplay = true;
      double tempLux;
      good = light.getLux(gain, ms, data0, data1,
                          tempLux); // True if neither sensor is saturated
      updateLux(tempLux);
      oled.setCursor(0, 11);
      oled.print(data0);
      oled.print("   ");
      oled.setCursor(64, 11);
      oled.print(data1);
      oled.print("   ");

      // Print out the results:
      oled.setCursor(0, 20);
      oled.print("lux:");
      oled.print(lux);
      oled.print("   ");
      oled.setCursor(116, 20);
      if (good) {
        oled.print("  ");
      } else {
        oled.drawNo(120, 20);
      }
    } else {
      drawDisplay = true;
      oled.setCursor(0, 11);
      oled.print("ERROR");
    }

    break;
  case pMaryInit:
    dataWheel.clearDiff();
    encButton.serviceRisingEdge();
    drawDisplay = true;
    oled.clear(PAGE);
    // menu part
    oled.drawMenu("Mary", true, false);

    heartCount = 0;

    nextState = pMary;
    break;
  case pMary:
    if (encButton.serviceRisingEdge()) {
      if (heartCount < 7) {
        oled.drawHeart(heartCount * 18, 12);
        drawDisplay = true;
        heartCount++;
      }
    }
    // Draw new value
    if (downButton.serviceRisingEdge()) {
      nextState = PDisplayLuxValueInit;
    }
    if (upButton.serviceRisingEdge()) {
      // nextState = ;
    }
    break;
  default:
    nextState = PInit;
    break;
  }
  state = nextState;

  if (drawDisplay)
    oled.display();

  holdState_t nextTriggerState = triggerState;
  switch (triggerState) {
  case hSRun:
    // Can't be running, if button pressed move on
    if (encButton.serviceRisingEdge()) {
      nextTriggerState = hSAveraging;
      luxAccumulator = lux;
      sampleCount = 1;
    }
    encButton.serviceFallingEdge();
    break;
  case hSAveraging:
    if (encButton.serviceFallingEdge()) {
      nextTriggerState = hSHeld;
    }
    break;
  case hSHeld:
    if (encButton.serviceRisingEdge()) {
      nextTriggerState = hSRun;
      luxAccumulator = 0;
      sampleCount = 0;
    }
    break;

  default:
    luxAccumulator = 0;
    sampleCount = 0;
    lux = 0;
    nextTriggerState = hSRun;
    break;
  }
  triggerState = nextTriggerState;
}

void LuxPanel::powerDown(void) {
  //
}

void LuxPanel::updateLux(double inputLux) {
  if (triggerState == hSAveraging) {
    luxAccumulator += inputLux;
    sampleCount++;
    lux = luxAccumulator / sampleCount;
  }
  if (triggerState == hSRun) {
    lux = inputLux;
  }
  if (triggerState == hSHeld) {
    lux = luxAccumulator / sampleCount;
  }
}
