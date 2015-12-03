#pragma once

// Provides debugging tools, but larger file size and slower
#define ARDUINO_DEBUG

enum DIGITAL_PINS
{
	DIGITAL_PIN_FORCE = 12,
	DIGITAL_PIN_ERROR = 13
};

enum ANALOG_PINS
{
	ANALOG_PIN_FORCE0 = 0,
	ANALOG_PIN_FORCE1 = 1
};

#if defined( ARDUINO_DEBUG )
#define DEBUG_PRINT(x) Serial.println( x )
#elif _DEBUG
#define DEBUG_PRINT
#endif