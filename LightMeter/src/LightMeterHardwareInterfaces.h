#ifndef LIGHT_METER_HARDWARE_INTERFACES_H
#define LIGHT_METER_HARDWARE_INTERFACES_H

#include "HardwareInterfaces.h"
#include <Encoder.h>

//Specific DataObject s are used to create local instances that construct
//with the correct size.
//
//A few panel components might want the same fingerprint of data.
//
//pass references-of to getData and setData
class RotoryEncoderObject : public DataObject {
  public:
	RotoryEncoderObject(void) {
		size = 2;
		data = new uint8_t[size];
	};
	~RotoryEncoderObject(void) {
		delete[] data;
	}
};

//This is the most basic hardware type, the Arduino digital input
class TeensyEncoderIn : public GenericHardwareDescription
{
  public:
	TeensyEncoderIn(int inputPin1, int inputPin2);
	void readHardware(void);
	void writeHardware(void);

  protected:
	Encoder *hwEncoder;
	int pin1;
	int pin2;
};

#endif