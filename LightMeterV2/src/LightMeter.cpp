#include "LightMeter.h"

Adafruit_TSL2591 luxMeter = Adafruit_TSL2591(2591);

OLEDFunctions oled(PIN_RESET, PIN_DC, PIN_CS, PIN_SCK, PIN_MOSI);

LightMeter myLightMeter;

// User Inputs
Encoder uiEncoder(9, 8);
PushButton uiPbEnter = PushButton(4);
PushButton uiPbUp = PushButton(22);
PushButton uiPbDown = PushButton(23);

JLed ledGreen = JLed(PIN_LED_OK);
JLed ledRed = JLed(PIN_LED_KO);

// Boing boing
void cfgPushButton(Bounce &bB) {
  bB.interval(20); // set to 15ms, default 10ms
}

/*
 * Menu order:
 * Photo -> Video -> SetISO -> System -> DisplayLux ->
 *  Mary -> StopRange -> Calibration -> IntegrationTime
 *
 * Button down = next menu, up = previous menu
 */
void onButtonReleased(Button &btn, uint16_t duration) {
  myLightMeter.lastActivity = millis(); // reset counter

  Serial.println("button pressed");
  // UP button
  if (btn.is(uiPbUp)) {
    if (myLightMeter.state == MDisplayPhotoValue) {
      // top of menu
    } else if (myLightMeter.state == MDisplayVideoValue) {
      myLightMeter.state = MDisplayPhotoValueInit;
    } else if (myLightMeter.state == MSetISO) {
      myLightMeter.saveConfigUser();
      myLightMeter.state = MDisplayVideoValueInit;
    } else if (myLightMeter.state == MSystem) {
      myLightMeter.state = MSetISOInit;
    } else if (myLightMeter.state == MDisplayLuxValue) {
      myLightMeter.state = MSystemInit;
    } else if (myLightMeter.state == MMary) {
      myLightMeter.state = MDisplayLuxValueInit;
    }

    // DOWN button
  } else if (btn.is(uiPbDown)) {
    if (myLightMeter.state == MDisplayPhotoValue) {
      myLightMeter.state = MDisplayVideoValueInit;
    } else if (myLightMeter.state == MDisplayVideoValue) {
      myLightMeter.state = MSetISOInit;
    } else if (myLightMeter.state == MSetISO) {
      myLightMeter.saveConfigUser();
      myLightMeter.state = MSystemInit;
    } else if (myLightMeter.state == MSystem) {
      myLightMeter.state = MDisplayLuxValueInit;
    } else if (myLightMeter.state == MDisplayLuxValue) {
      myLightMeter.state = MMaryInit;
    } else if (myLightMeter.state == MMary) {
      // bottom of menu
    }

    // ENTER button
  } else if (btn.is(uiPbEnter)) {
    Serial.println("ENTER button pressed");
    if (myLightMeter.state == MSystem) {
      myLightMeter.powerDown(false);

      myLightMeter.state = MSystemInit;
    } else if (myLightMeter.state == MMary) {
      if (myLightMeter.heartCount < 7) {
        oled.drawHeart(myLightMeter.heartCount * 18, 12);
        ledRed.Off();
        ledGreen.Blink(100, 100).Repeat(2);
        myLightMeter.heartCount++;
      }

      // End of menu
      // Trigger stats
    } else if (myLightMeter.triggerState == hSRun) {
      myLightMeter.triggerState = hSHeld;
    } else if (myLightMeter.triggerState == hSHeld) {
      myLightMeter.triggerState = hSRun;
    }
  }
}

void setup() {
  // Assert power on pin
  pinMode(PIN_POWER_ON, OUTPUT);
  digitalWrite(PIN_POWER_ON, HIGH);
  Serial.begin(115200);
  // while(!Serial);

  // Leds output, off by default
  pinMode(PIN_LED_OK, OUTPUT);
  digitalWrite(PIN_LED_OK, LOW);
  pinMode(PIN_LED_KO, OUTPUT);
  digitalWrite(PIN_LED_KO, LOW);

  // begin() will return true if communication is successful
  if (!lipo.begin()) {
    // bad state happenings
    myLightMeter.lipoGood = false;
    Serial.println("No battery or battery issue.");
  } else {
    myLightMeter.lipoGood = true;
    lipo.setCapacity(LIPO_CAPACITY);
    Serial.println("Battery should be good.");
  }

  oled.begin();     // Initialize the OLED
  oled.clear(ALL);  // Clear the display's internal memory
  oled.drawLogo();  // Display custom logo
  delay(2000);      // Delay 1200 ms
  oled.clear(PAGE); // Clear the buffer.

  // Setup Pushbuttons and encoder
  uiPbEnter.configureButton(cfgPushButton);
  uiPbUp.configureButton(cfgPushButton);
  uiPbDown.configureButton(cfgPushButton);

  uiPbEnter.onRelease(onButtonReleased);
  uiPbUp.onRelease(onButtonReleased);
  uiPbDown.onRelease(onButtonReleased);

  luxMeter.begin();
  luxMeter.setTiming(SENSOR_INTTIME_DEFAULT);
  luxMeter.enable();
}

void loop() {
  delay(10); // wait 10ms

  if (millis() - myLightMeter.lastActivity > SLEEP_AFTER) {
    myLightMeter.powerDown(true);
  }

  myLightMeter.process();
  // Serial.println("loop");
}

// Constructor, do basic init here
LightMeter::LightMeter(void) {
  // Defaults
  lux = 0;
  lipoGood = false;
  triggerState = hSRun;

  // Load user configuration from EEPROM
  loadConfigUser();
}

// Load user config, but first check if datas might be valid
void LightMeter::loadConfigUser() {
  // Get marker stored in EEPROM
  String marker;
  EEPROM.get(addrMarker, marker);

  if (marker == progName) {
    // Marker match, get user config
    EEPROM.get(addrConfigUser, ConfigUser);
  } else {
    // EEPROM is empty, generate the default content
    saveConfigUser();
  }
}

void LightMeter::saveConfigUser() {
  // Our config struct can't be bigger than the position of the MARKER in the
  // EEPROM 2000 for a teensy 3.2, 1000 for a 2.0, 4000 for a >= 3.5
  if (sizeof(ConfigUser) >= addrMarker) {
    Serial.println("sizeof(ConfigUser) >= 2000 !!!");
    ledStatus(LED_KO, 500, 3);
    return;
  }
  EEPROM.put(addrConfigUser, ConfigUser);
  EEPROM.put(addrMarker, progName);
  ledStatus(LED_OK, 300, 1);
}

// Sleep time
void LightMeter::powerDown(bool autosleep) {
  oled.clear(PAGE);
  oled.setFontType(1);
  oled.setCursor(6, 6);
  if (autosleep) {
    oled.print("Auto sleep zZz");
  } else {
    oled.print("Power down.");
  }
  oled.display();
  delay(1000);

  // Last minute things
  saveConfigUser();
  delay(10);

  // Try to shutdown
  digitalWrite(PIN_POWER_ON, LOW);

  myLightMeter.ledStatus(LED_KO, 1000, 3);

  // Oops failed
  lastActivity = millis(); // reset counter
  delay(4000);

  oled.clear(PAGE);
  oled.setCursor(6, 6);
  oled.print("Failed !");
  oled.display();
  delay(1000);

  oled.setFontType(0);
  oled.clear(PAGE);
  oled.display();

  myLightMeter.state = MSystemInit;
}

// state 0 OK, state 1 KO
void LightMeter::ledStatus(int state, int delay, int repeat) {
  if (state == 0) {
    ledGreen.Blink(delay, delay).Repeat(repeat);
    ledRed.Off();
  } else {
    ledRed.Blink(delay, delay).Repeat(repeat);
    ledGreen.Off();
  }
}

void LightMeter::getRawLux() {
  if (needHigh) {
    luxMeter.setGain(TSL2591_GAIN_HIGH);
    luxMeter.setTiming(SENSOR_INTTIME_HIGH); // 200ms
  } else {
    luxMeter.setGain(TSL2591_GAIN_LOW);
    luxMeter.setTiming(SENSOR_INTTIME_DEFAULT); // 400ms
  }

  sensors_event_t event;
  luxMeter.getEvent(&event);
  float tempLux = event.light;
  if ((event.light ==
       0) | // When sensor get saturated (normally in transition between gain)
      (event.light >
       4294966000.0) | // Lux value is restored to 201 for an effective change
                       // in Gain (from 428x to 1x)
      (event.light < -4294966000.0)) // Overflow is displayed indicating that
                                     // this value is not correct
  {
    tempLux = 201;
    overflow = 1;
  } else {
    overflow = 0;
  }

  tempLux = tempLux *
            DomeMultiplier; // DomeMultiplier = 2.3 (calibration) INCIDENT only
  if (tempLux < 40) {
    needHigh = 1;
  } // ~ 1 +1/3 OFFSET (*0.26 in lux calibration)
  if (tempLux > 200) {
    needHigh = 0;
  } // Turns off High hain
  // if (needHigh) {tempLux = tempLux*.26;}   // OFFSET corrected

  if (triggerState == hSRun) {
    lux = tempLux;
  }
}

// Get and display calculated fStop or speed
void LightMeter::getLuxAndCompute(bool fstop) {
  getRawLux();
  float value;

  /*
   * exposure time = stop squared * K / ( ISO * Lux )
   * f x f    Lux x ISO
   * -----  = ---------
   * shut       K cal
   */

  if (fstop) {
    // T value; Shutter time, in seconds
    value = pow(fStopTable[ConfigUser.fStopSetting], 2) * KValue /
            (lux * (isoTable[ConfigUser.isoSetting]));
  } else {
    // N value; Aperture
    value = sqrt((exposureTable[ConfigUser.exposureSetting] * lux *
                  isoTable[ConfigUser.isoSetting]) /
                 CValue);
  }
  Serial.print("lux: ");
  Serial.print(lux);
  Serial.print(" ;stop: ");
  Serial.print(fStopTable[ConfigUser.fStopSetting]);
  Serial.print(" ;speed: ");
  if (value < 1) {
    Serial.print("1/");
  }
  Serial.print(value);
  Serial.print(" ;gain: ");
  tsl2591Gain_t gain = luxMeter.getGain();
  switch (gain) {
  case TSL2591_GAIN_LOW:
    Serial.print(F("1x (Low)"));
    break;
  case TSL2591_GAIN_MED:
    Serial.print(F("25x (Medium)"));
    break;
  case TSL2591_GAIN_HIGH:
    Serial.print(F("428x (High)"));
    break;
  case TSL2591_GAIN_MAX:
    Serial.print(F("9876x (Max)"));
    break;
  }
  Serial.print(" ;timing: ");
  Serial.print((luxMeter.getTiming() + 1) * 100, DEC);
  Serial.println(F(" ms"));

  oled.eraseLowerArea();
  oled.drawLeftBracket(37, 10);
  oled.drawRightBracket(122, 10);
  if (fstop) {
    oled.drawFNumStyle1(fStopTable[ConfigUser.fStopSetting], 2, 13);
    oled.drawExposureStyle1(value, 60, 14);
  } else {
    oled.drawExposureStyle2(exposureTable[ConfigUser.exposureSetting], 2, 13);
    oled.drawFNumStyle2(value, 60, 14);
  }
  if (overflow) {
    oled.drawNo(33, 12);
    ledStatus(LED_KO, 500, 3);
  }
  if (triggerState == hSHeld) {
    oled.holdStyle1(100, 10);
  }
  if (fstop) {
    oled.setCursor(111, 17);
    oled.print("S");
  }
}

// Get and display bare LUX value
void LightMeter::getLux() {
  (void)getRawLux();

  // From getRawLux()
  sensor_t sensor;
  luxMeter.getSensor(&sensor);

  if (needHigh) {
    luxMeter.setGain(TSL2591_GAIN_HIGH);
  } else {
    luxMeter.setGain(TSL2591_GAIN_LOW);
  }
  // END

  // From Adafruit
  uint16_t ir, full;
  uint32_t lum = luxMeter.getFullLuminosity();
  lum = luxMeter.getFullLuminosity();
  ir = lum >> 16;
  full = lum & 0xFFFF;
  // END

  oled.setCursor(0, 11);
  oled.print("ir: ");
  oled.print(ir);
  oled.print("   ");

  oled.setCursor(64, 11);
  oled.print("vis: ");
  oled.print(full - ir);
  oled.print("   ");

  // Print out the result
  oled.setCursor(0, 20);
  oled.print("lux:");
  oled.print(lux);
  oled.print("   ");

  oled.setCursor(116, 20);
  if (!overflow) {
    oled.print("  ");
  } else {
    oled.drawNo(120, 20);
    ledStatus(LED_KO, 1000, 3);
  }
  if (triggerState == hSHeld) {
    oled.holdStyle1(100, 22);
  } else {
    // Clear hold
    oled.setCursor(100, 22);
    oled.print("    ");
  }
}

void LightMeter::process(void) {
  // Update buttons states
  uiPbEnter.update();
  uiPbUp.update();
  uiPbDown.update();
  long curEncoderPos = uiEncoder.read();

  ledGreen.Update();
  ledRed.Update();

  // Do not update display for now
  bool drawDisplay = false;

  drawDisplay = oled.drawBatteryWidget();

  // Handle current set state
  switch (state) {
  case Minit:
    // Initial state
    oled.clear(PAGE);
    drawDisplay = true;
    state = MDisplayPhotoValueInit;
    break;

  case MDisplayPhotoValueInit:
    // Init photo light metering
    drawDisplay = true;
    oled.clear(PAGE);
    oled.drawMenu("Photo", isoTable[ConfigUser.isoSetting], false, true);
    state = MDisplayPhotoValue;
    break;

  case MDisplayPhotoValue:
    // Photo light metering
    // Did the encoder move ?
    if (curEncoderPos != lastEncoderPos) {
      // yes
      if (curEncoderPos <= lastEncoderPos - ROT_ENC_DETENTS_COUNTS) {
        // moved down
        if (ConfigUser.fStopSetting > 0) {
          ConfigUser.fStopSetting--;
        }
      } else if (curEncoderPos >= lastEncoderPos + ROT_ENC_DETENTS_COUNTS) {
        // moved up
        if (ConfigUser.fStopSetting < FSTOP_COUNT - 1) {
          ConfigUser.fStopSetting++;
        }
      }
    }

    // Get measurements and display it
    getLuxAndCompute(true); // T value
    drawDisplay = true;

    break;

  case MDisplayVideoValueInit:
    drawDisplay = true;
    oled.clear(PAGE);
    oled.drawMenu("Video", isoTable[ConfigUser.isoSetting], true, true);
    state = MDisplayVideoValue;
    break;

  case MDisplayVideoValue:
    // Video light metering
    // Did the encoder move ?
    if (curEncoderPos != lastEncoderPos) {
      // yes
      if (curEncoderPos <= lastEncoderPos - ROT_ENC_DETENTS_COUNTS) {
        // moved down
        if (ConfigUser.exposureSetting > 0) {
          ConfigUser.exposureSetting--;
        }
      } else if (curEncoderPos >= lastEncoderPos + ROT_ENC_DETENTS_COUNTS) {
        // moved up
        if (ConfigUser.exposureSetting < EXPOSURE_COUNT - 1) {
          ConfigUser.exposureSetting++;
        }
      }
    }

    // Get measurements and display it
    getLuxAndCompute(false); // N value
    drawDisplay = true;

    break;

  case MSetISOInit:
    drawDisplay = true;
    oled.clear(PAGE);
    // Menu part
    oled.drawMenu("ISO", true, true);

    // Draw last value
    oled.drawISOScale(ConfigUser.isoSetting);

    state = MSetISO;
    break;

  case MSetISO:
    // ISO Selection
    // Did the encoder move ?
    if (curEncoderPos != lastEncoderPos) {
      // yes
      drawDisplay = true;
      oled.setCursor(0, 11);
      oled.print("      ");

      if (curEncoderPos <= lastEncoderPos - ROT_ENC_DETENTS_COUNTS) {
        // moved down
        if (ConfigUser.isoSetting > 0) {
          Serial.println("ISO--");
          Serial.println(ConfigUser.isoSetting);
          Serial.println(isoTable[ConfigUser.isoSetting]);
          ConfigUser.isoSetting--;
        }
      } else if (curEncoderPos >= lastEncoderPos + ROT_ENC_DETENTS_COUNTS) {
        // moved up
        if (ConfigUser.isoSetting < isoTableSize - 1) {
          Serial.println("ISO++");
          Serial.println(ConfigUser.isoSetting);
          Serial.println(isoTable[ConfigUser.isoSetting]);
          ConfigUser.isoSetting++;
        }
      }

      // Draw new value
      oled.drawISOScale(ConfigUser.isoSetting);
    }

    break;

  case MSystemInit:
    drawDisplay = true;
    oled.clear(PAGE);

    oled.drawMenu("System", true, false);

    oled.setCursor(64, 11);
    oled.print((float)lipo.voltage() / LIPO_CAPACITY, 2);
    oled.print("V");

    oled.setCursor(0, 20);
    oled.print((float)lipo.soc(), 0);
    oled.print("%");

    state = MSystem;
    break;

  case MSystem:
    oled.setCursor(0, 11);
    if (lipoGood) {
      oled.print("OK!");
    } else {
      oled.print("NOBATT");
    }

    oled.setCursor(64, 20);
    oled.print(lipo.current(AVG));
    oled.print("mA    ");

    oled.setCursor(0, 20);
    oled.print((float)lipo.soc(), 0);
    oled.print("%");

    oled.setCursor(64, 11);
    oled.print((float)lipo.voltage() / LIPO_CAPACITY, 2);

    // Shutdown is handled in button callback

    drawDisplay = true;

    break;

  case MDisplayLuxValueInit:
    drawDisplay = true;
    oled.clear(PAGE);

    oled.drawMenu("Lux", false, false);
    state = MDisplayLuxValue;
    break;

  case MDisplayLuxValue:
    getLux();
    drawDisplay = true;
    break;

  case MMaryInit:
    drawDisplay = true;
    oled.clear(PAGE);

    oled.drawMenu("Mary", true, false);
    heartCount = 0;
    state = MMary;
    break;

  case MMary:
    drawDisplay = true; // might need special handling since it was handled
    // by if(enter pressed) if (hC < 7)
    break;

  case MFStopRangeInit:
  case MFStopRange:

  default:
    state = Minit;
    break;
  }

  // Do we need to draw the display ?
  if (drawDisplay) {
    oled.display();
  }

  if (curEncoderPos != lastEncoderPos) {
    // Encoder moved, set new position
    myLightMeter.lastActivity = millis(); // reset counter
    lastEncoderPos = curEncoderPos;
  }

  // Handle the LUX calculation trigger
  switch (triggerState) {
  case hSRun:
    // handled in onButtonReleased
    break;

  case hSHeld:
    // handled in onButtonReleased
    break;

  default:
    lux = 0;
    triggerState = hSRun;
    break;
  }
}
