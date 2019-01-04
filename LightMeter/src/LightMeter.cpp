#include <Arduino.h>

#include "LuxPanel.h"

// Pinout declaration of TeensyView module
// The module is in Alternate configuration
#define PIN_RESET 2 // RST
#define PIN_DC 21   // DC
#define PIN_CS 20   // CS
#define PIN_SCK 14  // SCK
#define PIN_MOSI 7  // MOSI

OLEDFunctions oled(PIN_RESET, PIN_DC, PIN_CS, PIN_SCK, PIN_MOSI);

// Create an SFE_TSL2561 object, here called "light":
SFE_TSL2561 light;
boolean gain;    // Gain setting, 0 = X1, 1 = X16;
unsigned int ms; // Integration ("shutter") time in milliseconds

const uint8_t PIN_POWER_ON = 16;

LuxPanel myLuxPanel;

void setup() {
  // Assert power on pin
  pinMode(PIN_POWER_ON, OUTPUT);
  digitalWrite(PIN_POWER_ON, HIGH);
  Serial.begin(115200);

  // begin() will return true if communication is successful
  if (!lipo.begin()) {
    // bad state happenings
    myLuxPanel.lipoGood = false;
  } else {
    myLuxPanel.lipoGood = true;
    lipo.setCapacity(1000);
  }

  oled.begin();     // Initialize the OLED
  oled.clear(ALL);  // Clear the display's internal memory
  oled.display();   // Display what's in the buffer (splashscreen)
  delay(1200);      // Delay 1000 ms
  oled.clear(PAGE); // Clear the buffer.

  light.begin();
  unsigned char ID;

  if (light.getID(ID)) {
    Serial.print("Got factory ID: 0x");
    Serial.print(ID, HEX);
    Serial.println(", should be 0x5_");
  } else {
    Serial.println("ERROR");
  }
  gain = 0;
  unsigned char time = 2;
  Serial.println("Set timing...");
  light.setTiming(gain, time, ms);
  Serial.println("Powerup...");
  light.setPowerUp();
}

void loop() {
  delay(10);
  myLuxPanel.tickStateMachine(10);
  // Serial.println("End of loop");
}