#include <Arduino.h>

#include "LightMeterPanelComponents.h"

//---Knob--------------------------------------------------------
RotoryEncoder::RotoryEncoder( void )
{
	newData = 0;
	diff = 0;
}

void RotoryEncoder::setHardware( GenericHardwareDescription * input )
{
	hardwareInterface = input;
}

bool RotoryEncoder::hasFreshData( void )
{
	uint8_t returnVar = 0;
	if( newData )
	{
		returnVar = 1;
	}
	return returnVar;
}

void RotoryEncoder::freshen( uint16_t msTickDelta )
{
	//Throw away input

	//Cause the interface to get the data
	hardwareInterface->readHardware();
	
	//Collect the data
	KnobDataObject tempObject;
	hardwareInterface->getData(&tempObject);
	
	lastInput = *(uint16_t *)tempObject.data;
	diff = lastInput - state;
	if(( diff >  hysteresis )||( diff <  (0 - hysteresis) ))
	{
		newData = 1;
	}
	else if( diff == 0 )
	{
		clearDiff();
	}
	
	
}

int16_t RotoryEncoder::getDiff( void )
{
	return diff;
}

void RotoryEncoder::clearDiff( void )
{
	diff = 0;
	lastInput = 0;
	state = 0;
	hardwareInterface->writeHardware();
}

void RotoryEncoder::removeDiff( uint16_t deltaDiff )
{
	if( diff >= deltaDiff )
	{
		state = state + deltaDiff;
	}
	else if( diff <= (0 - deltaDiff) )
	{
		state = state - deltaDiff;
	}
	else
	{
		state = lastInput;
	}
	diff = lastInput - state;
	
}

uint8_t RotoryEncoder::serviceChanged( void )
{
	uint8_t returnVar = 0;
	if( newData == 1 )
	{
		returnVar = 1;
		newData = 0;
	}
	return returnVar;
}
