#include "network.h"
#include "Arduino.h"
#include "def.h"

UpdatePacket::UpdatePacket() {
	timestamp = 0;
}
FridgePacket::FridgePacket() {
	itempercent0 = 0;
	itempercent1 = 1;
}

///////////////////////
// CDeviceConnection //
///////////////////////

CDeviceConnection::CDeviceConnection() {
	m_ulUpdateInterval = UPDATE_INTERVAL_DEFAULT;
	m_ulLastUpdate = 0;
	m_pCurrentPacket = NULL;
}
CDeviceConnection::~CDeviceConnection() {
}

void CDeviceConnection::doLoop()
{
	unsigned long currentTime = millis();

	// See if its time for an update
	if( m_ulLastUpdate + m_ulUpdateInterval < currentTime ) {
		this->broadcastUpdate( m_pCurrentPacket );
		m_ulLastUpdate = currentTime;
	}
	
}

double CDeviceConnection::getUpdateInterval() {
	return m_ulUpdateInterval;
}
void CDeviceConnection::setUpdateInterval( unsigned long updateInterval ) {
	m_ulUpdateInterval = updateInterval;
}
void CDeviceConnection::setCurrentPacket( UpdatePacket *pPacket ) {
	m_pCurrentPacket = pPacket;
}

///////////////////////
// CSerialConnection //
///////////////////////

CSerialConnection::CSerialConnection() {
}
CSerialConnection::~CSerialConnection() {
}

bool CSerialConnection::connect() {
	Serial.println( "Connected to serial (test device)" );
	return true;
}
void CSerialConnection::disconnect() {
	Serial.println( "Disconnected from serial (test device)" );
}

void CSerialConnection::broadcastUpdate( UpdatePacket *pPacket )
{
	FridgePacket *pFridgePacket;

	// Convert to a FridgePacket
	pFridgePacket = reinterpret_cast<FridgePacket*>(pPacket);
	if( !pFridgePacket )
		return;
	// Print test serial
	Serial.println( "Fridge Update:" );
	Serial.println( "Item0: " );
	Serial.println( pFridgePacket->itempercent0 );
	Serial.println( "Item1: " );
	Serial.println( pFridgePacket->itempercent1 );
}