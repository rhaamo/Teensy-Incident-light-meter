#include <Arduino.h>

#include "LightMeterHardwareInterfaces.h"

// Arduino Analog in
TeensyEncoderIn::TeensyEncoderIn(int inputPin1, int inputPin2) {
  pinMode(inputPin1, INPUT);
  pinMode(inputPin2, INPUT);
  localData.size = 2;
  localData.data = new uint8_t[localData.size];
  pin1 = inputPin1;
  pin2 = inputPin2;

  hwEncoder = new Encoder(inputPin1, inputPin2);
}

void TeensyEncoderIn::readHardware(void) {
  *(uint16_t *)localData.data = (uint16_t)hwEncoder->read();
};

// This is set to just clear
void TeensyEncoderIn::writeHardware(void) { hwEncoder->write(0); };
