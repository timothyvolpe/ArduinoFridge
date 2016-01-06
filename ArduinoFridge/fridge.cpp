#include "fridge.h"
#include "Arduino.h"
#include "def.h"

CFridge::CFridge()
{
	m_fridgeStage = 0;
	m_pDeviceConnection = NULL;
	m_pFridgePacket = NULL;
}
CFridge::~CFridge()
{
	if( m_pDeviceConnection ) {
		delete m_pDeviceConnection;
		m_pDeviceConnection = NULL;
	}
	if( m_pFridgePacket ) {
		delete m_pFridgePacket;
		m_pFridgePacket = NULL;
	}
}

bool CFridge::initialize()
{
	// Setup each resistor
	for( unsigned char i = 0; i < FORCE_RESISTOR_COUNT; i++ ) {
		m_resistorMaxs[i] = 1023; // Set maxs to the hard maximum (i.e. no calibration)
		m_forceValues[i] = 0;
	}
	m_fridgeStage = FRIDGE_STAGE_IDLE;
	// Set up the device connection
	m_pDeviceConnection = new CSerialConnection();
	m_pFridgePacket = new FridgePacket();
	m_pDeviceConnection->setCurrentPacket( m_pFridgePacket );
	m_pDeviceConnection->connect();
}

bool CFridge::loop()
{


	// Check stage
	switch( m_fridgeStage )
	{
	case FRIDGE_STAGE_IDLE:
		// Update the packet
		m_pFridgePacket->itempercent0 = this->getForcePercentage( FORCE_RESISTOR0 );
		m_pFridgePacket->itempercent1 = this->getForcePercentage( FORCE_RESISTOR1 );
		m_pDeviceConnection->doLoop();
		break;
	case FRIDGE_STAGE_CALIBRATE:
		// Check if the calibration period is over
		if( millis() > m_ulCalibrationStart + (unsigned long)m_uiCalibrationLength ) {
			// Calculate the calibrated averages
			for( unsigned char i = 0; i < FORCE_RESISTOR_COUNT; i++ ) {
				m_resistorMaxs[i] = m_calibrationTotals[i] / m_calibrationSamples;
				DEBUG_PRINT( m_resistorMaxs[i] );
			}
			m_fridgeStage = FRIDGE_STAGE_IDLE;
			DEBUG_PRINT( "CALIBRATION END " );
		}
		else {
			// Read each resistors value and add it to the total
			for( unsigned char i = 0; i < FORCE_RESISTOR_COUNT; i++ )
				m_calibrationTotals[i] += this->readValue( i );
			// Increment number of readings
			m_calibrationSamples++;
		}
		break;
	default:
		DEBUG_PRINT( "ERROR: Invalid fridge state!" );
		return false; // throw an error
	}
	return true;
}

void CFridge::calibrate( unsigned int uiTimeout )
{
	// Make sure we're not already calibrating
	if( m_fridgeStage == FRIDGE_STAGE_CALIBRATE ) {
		DEBUG_PRINT( "CALIBRATION IN PROGRESS!" );
		return;
	}

	DEBUG_PRINT( "CALIBRATION BEGIN" );

	// Record the start time
	m_ulCalibrationStart = millis();
	m_uiCalibrationLength = uiTimeout;
	// Change stage
	m_fridgeStage = FRIDGE_STAGE_CALIBRATE;
	// Clear the recording data
	m_calibrationSamples = 0;
	for( unsigned char i = 0; i < FORCE_RESISTOR_COUNT; i++ )
		m_calibrationTotals[i] = 0;
}

ForceResValue CFridge::readValue( unsigned char resistor )
{
	int iTest;

	// Read the analog input from the requested resistor
	switch( resistor )
	{
	case FORCE_RESISTOR0:
		return analogRead( ANALOG_PIN_FORCE0 );
	case FORCE_RESISTOR1:
		return analogRead( ANALOG_PIN_FORCE1 );;
	default:
		DEBUG_PRINT( "WARNING: Invalid force resistor specified" );
		return 0;
	}
}

unsigned char CFridge::getForcePercentage( unsigned char resistor )
{
	int value;

	// Read the value
	value = this->readValue( resistor );
	return (unsigned char)(((float)value/(float)m_resistorMaxs[resistor])*100.0f);
}