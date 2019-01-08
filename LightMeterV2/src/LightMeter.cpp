#include "LightMeter.h"

/* TODO
 * Switch to Adafruit TSL2561 lib
 * RGB handling of the encoder
 * User defined K-constant value
 * fStop user range switcher
 * user integration time choice [13ms, 101ms, 402ms]
 */

OLEDFunctions oled(PIN_RESET, PIN_DC, PIN_CS, PIN_SCK, PIN_MOSI);

SFE_TSL2561 luxMeter;

LightMeter myLightMeter;

// User Inputs
Encoder uiEncoder(8, 9);
PushButton uiPbEnter = PushButton(4);
PushButton uiPbUp = PushButton(22);
PushButton uiPbDown = PushButton(23);

// Boing boing
void cfgPushButton(Bounce &bB) {
  bB.interval(20); // set to 15ms, default 10ms
}

void onButtonReleased(Button& btn, uint16_t duration) {
  Serial.println("button pressed");
  // UP button
  if (btn.is(uiPbUp)) {
    Serial.println("UP button pressed");
    if (myLightMeter.state == MDisplayPhotoValue) {
      myLightMeter.state = MDisplayVideoValueInit;
    } else if (myLightMeter.state == MDisplayVideoValue) {
      myLightMeter.state = MSetISOInit;
    } else if (myLightMeter.state == MSetISO) {
      myLightMeter.state = MSystemInit;
    } else if (myLightMeter.state == MSystem) {
      myLightMeter.state = MSetISOInit;
    } else if (myLightMeter.state == MDisplayLuxValue) {
      myLightMeter.state = MMaryInit;
    } else if (myLightMeter.state == MMary) {
      // end of menu
    }

  // DOWN button
  } else if (btn.is(uiPbDown)) {
    Serial.println("DOWN button pressed");
    if (myLightMeter.state == MDisplayVideoValue) {
      myLightMeter.state = MDisplayPhotoValueInit;
    } else if (myLightMeter.state == MSetISO) {
      myLightMeter.state = MDisplayVideoValueInit;
    } else if (myLightMeter.state == MSystem) {
      myLightMeter.state = MDisplayLuxValueInit;
    } else if (myLightMeter.state == MDisplayLuxValue) {
      myLightMeter.state = MSystemInit;
    } else if (myLightMeter.state == MMary) {
      myLightMeter.state = MDisplayLuxValueInit;
    }

  // ENTER button
  } else if (btn.is(uiPbEnter)) {
    Serial.println("ENTER button pressed");
    if (myLightMeter.state == MSystem) {
      oled.clear(PAGE);
      oled.setFontType(1);
      oled.setCursor(6, 6);
      oled.print("Power down.");
      oled.display();
      delay(1000);
      myLightMeter.powerDown();
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
    } else if (myLightMeter.state == MMary) {
      if (myLightMeter.heartCount < 7) {
        oled.drawHeart(myLightMeter.heartCount * 18, 12);
        myLightMeter.heartCount++;
      }

    // End of menu
    // Trigger stats
    } else if (myLightMeter.triggerState == hSRun) {
      myLightMeter.triggerState = hSAveraging;
      myLightMeter.luxAccumulator = myLightMeter.lux;
      myLightMeter.sampleCount = 1;
    } else if (myLightMeter.triggerState == hSAveraging) {
      myLightMeter.triggerState = hSHeld;
    } else if (myLightMeter.triggerState == hSHeld) {
      myLightMeter.triggerState = hSRun;
      myLightMeter.luxAccumulator = 0;
      myLightMeter.sampleCount = 0;
    }
  }

}

void setup() {
  // Assert power on pin
  pinMode(PIN_POWER_ON, OUTPUT);
  digitalWrite(PIN_POWER_ON, HIGH);
  Serial.begin(115200);

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
  oled.display();   // Display what's in the buffer (splashscreen)
  delay(1200);      // Delay 1200 ms
  oled.clear(PAGE); // Clear the buffer.

  luxMeter.begin();

  if (luxMeter.getID(myLightMeter.sensorID)) {
    Serial.print("Got factory ID: 0x");
    Serial.print(myLightMeter.sensorID, HEX);
    Serial.println(", should be 0x5_");
  } else {
    Serial.println("ERR: Cannot get sensor ID");
  }
  Serial.println("Set timing...");
  luxMeter.setTiming(myLightMeter.gain, myLightMeter.sensorTime, myLightMeter.sensorMs);
  Serial.println("Powerup...");
  luxMeter.setPowerUp();

  // Setup Pushbuttons and encoder
  uiPbEnter.configureButton(cfgPushButton);
  uiPbUp.configureButton(cfgPushButton);
  uiPbDown.configureButton(cfgPushButton);

  uiPbEnter.onRelease(onButtonReleased);
  uiPbUp.onRelease(onButtonReleased);
  uiPbDown.onRelease(onButtonReleased);
}

void loop() {
  //Serial.println("in loop");
  delay(10); // wait 10ms
  myLightMeter.process();
  //Serial.println("out loop");
}

// Constructor, do basic init here
LightMeter::LightMeter(void) {
  // Defaults
  luxAccumulator = 0;
  sampleCount = 0;
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
  // Our config struct can't be bigger than the position of the MARKER in the EEPROM
  // 2000 for a teensy 3.2, 1000 for a 2.0, 4000 for a >= 3.5
  if (sizeof(ConfigUser) >= addrMarker) {
    Serial.println("sizeof(ConfigUser) >= 2000 !!!");
    return;
  }
	EEPROM.put(addrConfigUser, ConfigUser);
  EEPROM.put(addrMarker, progName);
}

// Sleep time
void LightMeter::powerDown() {
  // Last minute things
  saveConfigUser();
  delay(10);

  // Try to shutdown
  digitalWrite(PIN_POWER_ON, LOW);
}

// Get and display calculated fStop or speed
void LightMeter::getLuxAndCompute(bool fstop) {
  // Get the value
  double tempLux;
  bool good;
  float value;
  unsigned int data0, data1;
  // Get values
  if (!luxMeter.getData(data0, data1)) {
    return;
  }

  /*
   * exposure time = stop squared * K / ( ISO * Lux )
   * f x f    Lux x ISO
   * -----  = ---------
   * shut       K cal
   */

  // Returns true if sensor is not saturated
  good = luxMeter.getLux(gain, sensorMs, data0, data1, tempLux);
  updateLux(tempLux);
  if (fstop) {
    // T value; Shutter time
    value = pow(fStopTable[ConfigUser.fStopSetting], 2) * KValue / (lux * (isoTable[ConfigUser.isoSetting]));
  } else {
    // N value; Aperture
    value = sqrt((exposureTable[ConfigUser.exposureSetting] * lux * isoTable[ConfigUser.isoSetting]) / KValue);
  }
  oled.eraseLowerArea();
  oled.drawLeftBracket(32, 10);
  oled.drawRightBracket(122, 10);
  if (fstop) {
    oled.drawFNumStyle1(fStopTable[ConfigUser.fStopSetting], 2, 13);
    oled.drawExposureStyle1(value, 60, 14);
  } else {
    oled.drawExposureStyle2(exposureTable[ConfigUser.exposureSetting], 2, 13);
    oled.drawFNumStyle2(value, 60, 14);
  }
  if (!good) {
    oled.drawNo(33, 12);
  }
  if (triggerState == hSHeld) {
    oled.holdStyle1(100, 10);
  }
  if (triggerState == hSAveraging) {
    oled.aveStyle1(100, 10);
  }
  if (fstop) {
    oled.setCursor(111, 17);
    oled.print("S");
  }
}

// Get and display bare LUX value
void LightMeter::getLux() {
  double tempLux;
  bool good;
  unsigned int data0, data1;
  if (!luxMeter.getData(data0, data1)) {
    oled.setCursor(0, 11);
    oled.print("ERROR");
    return;
  }

  good = luxMeter.getLux(gain, sensorMs, data0, data1, tempLux);
  updateLux(tempLux);

  oled.setCursor(0, 11);
  oled.print(data0);
  oled.print("   ");
  
  oled.setCursor(64, 11);
  oled.print(data1);
  oled.print("   ");

  // Print out the result
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
}

void LightMeter::process(void) {
  //Serial.println("processing");
  // Update buttons states
  uiPbEnter.update();
  uiPbUp.update();
  uiPbDown.update();
  long curEncoderPos = uiEncoder.read();

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
      if (curEncoderPos != encoderPos) {
        // yes
        if (curEncoderPos < encoderPos) {
          // moved down
          if (ConfigUser.fStopSetting > 0) {
            ConfigUser.fStopSetting--;
          }
        } else if (curEncoderPos > encoderPos) {
          // moved up
          if (ConfigUser.fStopSetting < 23) {
            ConfigUser.fStopSetting++;
          }
        }
      }
     
      // Get measurements and display it
      getLuxAndCompute(true); // T value
      drawDisplay = true;

      // Last, set new pos
      encoderPos = curEncoderPos;
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
      if (curEncoderPos != encoderPos) {
        // yes
        if (curEncoderPos < encoderPos) {
          // moved down
          if (ConfigUser.exposureSetting > 0) {
            ConfigUser.exposureSetting--;
          }
        } else if (curEncoderPos > encoderPos) {
          // moved up
          if (ConfigUser.exposureSetting < 2) {
            ConfigUser.exposureSetting++;
          }
        }
      }

      // Get measurements and display it
      getLuxAndCompute(false); // N value
      drawDisplay = true;

      // Last, set new pos
      encoderPos = curEncoderPos;
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
      if (curEncoderPos != encoderPos) {
        // yes
        drawDisplay = true;
        oled.setCursor(0, 11);
        oled.print("      ");

        if (curEncoderPos < encoderPos) {
          // moved down
          if (ConfigUser.isoSetting > 0) {
            ConfigUser.isoSetting--;
          }
        } else if (curEncoderPos > encoderPos) {
          // moved up
          if (ConfigUser.isoSetting < isoTableSize) {
            ConfigUser.isoSetting++;
          }
        }

        // Draw new value
        oled.drawISOScale(ConfigUser.isoSetting);

        saveConfigUser();
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
    case MKCalibrationInit:
    case MKCalibration:
    case MIntegrationTimeInit:
    case MIntegrationTime:

    default:
      state = Minit;
      break;
  }

  // Do we need to draw the display ?
  if (drawDisplay) {
    oled.display();
  }

  // Handle the LUX calculation trigger
  switch (triggerState) {
    case hSRun:
      // handled in onButtonReleased
      break;
    
    case hSAveraging:
      // handled in onButtonReleased
      break;

    case hSHeld:
      // handled in onButtonReleased
      break;

    default:
      luxAccumulator = 0;
      sampleCount = 0;
      lux = 0;
      triggerState = hSRun;
      break;
  }
}

void LightMeter::updateLux(double inputLux) {
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
