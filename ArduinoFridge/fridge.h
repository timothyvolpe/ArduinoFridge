#pragma once

enum : unsigned short
{
	FORCE_RESISTOR0 = 0,
	FORCE_RESISTOR1 = 1,
	FORCE_RESISTOR_COUNT
};

enum : unsigned char
{
	FRIDGE_STAGE_UNKNOWN = 0,
	FRIDGE_STAGE_IDLE,
	FRIDGE_STAGE_CALIBRATE
};

typedef unsigned short ForceResValue;

class CFridge
{
private:
	unsigned char m_fridgeStage;

	unsigned long m_ulCalibrationStart;
	unsigned int m_uiCalibrationLength;
	unsigned long m_calibrationTotals[FORCE_RESISTOR_COUNT];
	unsigned int m_calibrationSamples;

	unsigned int m_resistorMaxs[FORCE_RESISTOR_COUNT];

	unsigned int m_forceValues[FORCE_RESISTOR_COUNT];
public:
	CFridge();
	~CFridge();

	// bool initialize() - performs setup, should be called in setup()
	bool initialize();

	// void loop() - the fridge logic, should be called in loop()
	bool loop();

	// void calibrate( unsigned int uiTimeout ) - calibrates the sensors
	//		unsigned int uiTimeout - amount of time to record the max sensor value
	void calibrate( unsigned int uiTimeout );

	// ForceResValue readValue( ForceResFlags resistor ) - reads the value from the resistor
	//		ForceResValue resistor - the resistor to read from
	ForceResValue readValue( unsigned char resistor );

	// float getForcePercentage() - returns the force percentage of the calibrated maximum
	//		unsigned char resistor - the resistor to read from
	float getForcePercentage( unsigned char resistor );
};