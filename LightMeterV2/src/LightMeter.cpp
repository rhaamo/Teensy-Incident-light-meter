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
PushButton uiPbEnter = PushButton(4, ENABLE_INTERNAL_PULLUP);
PushButton uiPbUp = PushButton(22, ENABLE_INTERNAL_PULLUP);
PushButton uiPbDown = PushButton(23, ENABLE_INTERNAL_PULLUP);

void setup() {
  // Assert power on pin
  pinMode(PIN_POWER_ON, OUTPUT);
  digitalWrite(PIN_POWER_ON, HIGH);
  Serial.begin(115200);

  // begin() will return true if communication is successful
  if (!lipo.begin()) {
    // bad state happenings
    myLightMeter.lipoGood = false;
  } else {
    myLightMeter.lipoGood = true;
    lipo.setCapacity(LIPO_CAPACITY);
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
}

void loop() {
  delay(10); // wait 10ms
  myLightMeter.process();
}

// Boing boing
void cfgPushButton(Bounce &bB) {
  bB.interval(15); // set to 15ms, default 10ms
}

// Constructor, do basic init here
LightMeter::LightMeter(void) {
  // Setup Pushbuttons and encoder
  uiPbEnter.configureButton(cfgPushButton);
  uiPbUp.configureButton(cfgPushButton);
  uiPbDown.configureButton(cfgPushButton);

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
  // Update buttons states
  uiPbEnter.update();
  uiPbUp.update();
  uiPbDown.update();
  long curEncoderPos = uiEncoder.read();

  // Do not update display for now
  bool drawDisplay = false;

  drawDisplay = oled.drawBatteryWidget();

  // Which state is next; Filled by one button action in the menu if any
  MState_t nextState = state;

  // Handle current set state
  switch (state) {
    case Minit:
      // Initial state
      oled.clear(PAGE);
      drawDisplay = true;
      nextState = MDisplayPhotoValueInit;
      break;

    case MDisplayPhotoValueInit:
      // Init photo light metering
      drawDisplay = true;
      oled.clear(PAGE);
      oled.drawMenu("Photo", isoTable[ConfigUser.isoSetting], false, true);
      nextState = MDisplayPhotoValue;
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
      // Check UP pb state
      if (uiPbUp.isPressed()) {
        // Switch to Video mode
        nextState = MDisplayVideoValueInit;
      } else {
        // Get measurements and display it
        getLuxAndCompute(true); // T value
        drawDisplay = true;
      }

      // Last, set new pos
      encoderPos = curEncoderPos;
      break;

    case MDisplayVideoValueInit:
      drawDisplay = true;
      oled.clear(PAGE);
      oled.drawMenu("Video", isoTable[ConfigUser.isoSetting], true, true);
      nextState = MDisplayVideoValue;
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
      // Check DOWN pb state
      if (uiPbDown.isPressed()) {
        // Switch to Photo mode
        nextState = MDisplayPhotoValueInit;
      } else if (uiPbUp.isPressed()) {
        // Switch to ISO Selection
        nextState = MSetISOInit;
      } else {
        // Get measurements and display it
        getLuxAndCompute(false); // N value
        drawDisplay = true;
      }

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

      nextState = MSetISO;
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

      // Check DOWN pb state
      if (uiPbDown.isPressed()) {
        nextState = MDisplayVideoValueInit;
      }
      if (uiPbUp.isPressed()) {
        nextState = MSystemInit;
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

      nextState = MSystem;
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

      if (uiPbEnter.isPressed()) {
        oled.clear(PAGE);
        oled.setFontType(1);
        oled.setCursor(6, 6);
        oled.print("Power down.");
        oled.display();
        delay(1000);
        powerDown();
        delay(4000);

        oled.clear(PAGE);
        oled.setCursor(6, 6);
        oled.print("Failed !");
        oled.display();
        delay(1000);

        oled.setFontType(0);
        oled.clear(PAGE);
        oled.display();
        nextState = MSystemInit;
      }
      drawDisplay = true;
      if (uiPbDown.isPressed()) {
        nextState = MSetISOInit;
      }
      if (uiPbUp.isPressed()) {
        nextState = MDisplayLuxValueInit;
      }
      break;

    case MDisplayLuxValueInit:
      drawDisplay = true;
      oled.clear(PAGE);

      oled.drawMenu("Lux", false, false);
      nextState = MDisplayLuxValue;
      break;

    case MDisplayLuxValue:
      if (uiPbDown.isPressed()) {
        nextState = MSystemInit;
      }
      if (uiPbUp.isPressed()) {
        nextState = MMaryInit;
      }
      getLux();
      break;

    case MMaryInit:
      drawDisplay = true;
      oled.clear(PAGE);

      oled.drawMenu("Mary", true, false);
      heartCount = 0;
      nextState = MMary;
      break;

    case MMary:
      if (uiPbEnter.isPressed()) {
        if (heartCount < 7) {
          oled.drawHeart(heartCount * 18, 12);
          drawDisplay = true;
          heartCount++;
        }
      }

      // Draw new value
      if (uiPbDown.isPressed()) {
        nextState = MDisplayLuxValueInit;
      }
      if (uiPbUp.isPressed()) {
        // nextState = ;
      }
      break;

    default:
      nextState = Minit;
      break;
  }

  // "Current state" now became the next one, will be processed on next loop
  state = nextState;

  // Do we need to draw the display ?
  if (drawDisplay) {
    oled.display();
  }

  // Handle the LUX calculation trigger
  holdState_t nextTriggerState = triggerState;
  switch (triggerState) {
    case hSRun:
      // Can't be running, if button pressed move on
      if (uiPbEnter.isPressed()) {
        nextTriggerState = hSAveraging;
        luxAccumulator = lux;
        sampleCount = 1;
      }
      break;
    
    case hSAveraging:
      if (uiPbEnter.isPressed()) {
        nextTriggerState = hSHeld;
      }
      break;

    case hSHeld:
      if (uiPbEnter.isPressed()) {
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