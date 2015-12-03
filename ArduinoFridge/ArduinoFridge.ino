/*
 Name:		ArduinoFridge.ino
 Created:	11/25/2015 9:09:22 PM
 Author:	Timothy
*/

#include "def.h"
#include "fridge.h"

bool isErrorLit, isError;
CFridge *pFridge;

// the setup function runs once when you press reset or power the board
void setup()
{
	isError = false;
	isErrorLit = false;
	pFridge = NULL;

	// Setup digital pins
	pinMode( DIGITAL_PIN_FORCE, OUTPUT ); // Force LED
	pinMode( DIGITAL_PIN_ERROR, OUTPUT ); // Error LED

	// Create the fridge class
	pFridge = new CFridge();
	if( !pFridge )
		isError = true;
	pFridge->initialize();

	// Serial
	Serial.begin( 9600 );
}

// the loop function runs over and over again until power down or reset
void loop()
{
	int iForceVal;

	// Check for error
	if ( isError && !isErrorLit ) {
		isErrorLit = true;
		digitalWrite( DIGITAL_PIN_ERROR, HIGH ); // Turn on the error led
	}

	// Perform fridge loop
	if( !pFridge->loop() )
		isError = true;

	// Check for force
	iForceVal = analogRead( ANALOG_PIN_FORCE0 );
	if( iForceVal > 512 )
		digitalWrite( DIGITAL_PIN_FORCE, HIGH );
	else
		digitalWrite( DIGITAL_PIN_FORCE, LOW );

	// Print a test value
	//Serial.println( iForceVal );
}


void serialEvent()
{
	while( Serial.available() ) {
		char inChar = (char)Serial.read();
		if( inChar == 'c' ) {
			pFridge->calibrate( 4000 );
			break;
		}
	}
}