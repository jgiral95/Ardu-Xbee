#ifndef ArdXbee_h
#define ArdXbee_h

#include <Arduino.h>
#include <SoftwareSerial.h>

class XbeeLocal
{
public:
	XbeeLocal(uint8_t,uint8_t);
	void setAdress(int[8]);
	bool sendData(char[]);
	bool sendData(int[8],char[]);
	int recData();
	char* recData(bool);
	int* nodeDiscover();
	int getDevices();
	char inputBuffer[20];
	int inputAdress[8];
private:
	int OuputAdress[8];
	SoftwareSerial UARTport;
	int attachedDevices;
};

class XbeeRemote
{
public:
	XbeeRemote(int[8]);
	int* getAdress();
private:
	int adress[8];
};
#endif