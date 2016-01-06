#pragma once

#define UPDATE_INTERVAL_DEFAULT 5000 // 15 seconds


struct UpdatePacket
{
	UpdatePacket();

	double timestamp;
};

struct FridgePacket : public UpdatePacket
{
	FridgePacket();

	unsigned char itempercent0;
	unsigned char itempercent1;
};

///////////////////////
// CDeviceConnection //
///////////////////////

class CDeviceConnection
{
private:
	unsigned long m_ulLastUpdate;
	unsigned long m_ulUpdateInterval;

	UpdatePacket *m_pCurrentPacket;
protected:
public:
	CDeviceConnection();
	virtual ~CDeviceConnection();

	// bool connect() - connects to the device
	virtual bool connect() = 0;
	// void disconnect() - disconnects from the device
	virtual void disconnect() = 0;

	// void doLoop() - called each loop, determines if we need to broadcast an update
	void doLoop();

	// void broadcastUpdate( UpdatePacket *pPacket ) - broadcasts an update
	//		pPacket - the data to send to the device
	virtual void broadcastUpdate( UpdatePacket *pPacket ) = 0;

	// double getUpdateInterval() - returns the update interval in milliseconds
	double getUpdateInterval();
	// void setUpdateInterval( unsigned long updateInterval ) - sets the update interval in milliseconds
	//		updateInterval - the new update interval in milliseconds
	void setUpdateInterval( unsigned long updateInterval );

	// void setCurrentPacket( UpdatePacket *pPacket ) - sets a new packet to broadcast
	//		UpdatePacket *pPacket - the packet to broadcast
	void setCurrentPacket( UpdatePacket *pPacket );
};

///////////////////////
// CSerialConnection //
///////////////////////

class CSerialConnection : public CDeviceConnection
{
public:
	CSerialConnection();
	~CSerialConnection(); 

	bool connect();
	void disconnect();

	void broadcastUpdate( UpdatePacket *pPacket );
};