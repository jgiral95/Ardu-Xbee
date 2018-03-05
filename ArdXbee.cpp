#include <ArdXbee.h>
#include <Arduino.h>
#include <SoftwareSerial.h>

XbeeLocal::XbeeLocal(uint8_t rx,uint8_t tx):UARTport(rx,tx)
{
	pinMode(rx,INPUT);
	pinMode(tx,OUTPUT);
	UARTport=SoftwareSerial(rx,tx);
	UARTport.begin(9600);
	attachedDevices=0;
}
void XbeeLocal::setAdress(int destAdress[8])
{
	for(int i=0;i<8;i++)
	{
		OuputAdress[i]=destAdress[i];
	}
}
bool XbeeLocal::sendData(char data[])
{
	while(UARTport.available()>0)
	{
		UARTport.read();
	}

	int temSum=0;

	UARTport.write(0x7E);
	UARTport.write((byte)0x00);
	UARTport.write(strlen(data)+14);
	UARTport.write(0x10);
	UARTport.write(0x01);

	for(int i=0;i<8;i++)
	{
		temSum+=OuputAdress[i];
		UARTport.write(OuputAdress[i]);
	}

	UARTport.write(0xFF);
	UARTport.write(0xFE);
	UARTport.write((byte)0x00);
	UARTport.write((byte)0x00);

	for(int i=0;i<strlen(data);i++)
	{
		temSum+=data[i];
		UARTport.write(data[i]);

	}

	temSum+=17+255+254;
	int Chk=0xFF-char(temSum);
	UARTport.write(Chk);

	delay(100);

	char tempBuffer[10];
	char FT=0x8B;

	if(UARTport.available()>0)
	{
		if(UARTport.find("~"))
		{
			UARTport.readBytes(tempBuffer,10);
			if(tempBuffer[2]==FT && tempBuffer[7]==0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	else
	{
		while(UARTport.available()>0)
		{
			UARTport.read();
		}
		return false;
	}
}
bool XbeeLocal::sendData(int destAdress[8],char data[])
{
	while(UARTport.available()>0)
	{
		UARTport.read();
	}

	int temSum=0;

	UARTport.write(0x7E);
	UARTport.write((byte)0x00);
	UARTport.write(strlen(data)+14);
	UARTport.write(0x10);
	UARTport.write(0x01);

	for(int i=0;i<8;i++)
	{
		temSum+=destAdress[i];
		UARTport.write(destAdress[i]);
	}

	UARTport.write(0xFF);
	UARTport.write(0xFE);
	UARTport.write((byte)0x00);
	UARTport.write((byte)0x00);

	for(int i=0;i<strlen(data);i++)
	{
		temSum+=data[i];
		UARTport.write(data[i]);

	}

	temSum+=17+255+254;
	int Chk=0xFF-char(temSum);
	UARTport.write(Chk);

	delay(100);

	char tempBuffer[10];
	char FT=0x8B;

	if(UARTport.available()>0)
	{
		if(UARTport.find("~"))
		{
			UARTport.readBytes(tempBuffer,10);
			if(tempBuffer[2]==FT && tempBuffer[7]==0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	else
	{
		while(UARTport.available()>0)
		{
			UARTport.read();
		}
		return false;
	}
}
int XbeeLocal::recData()
{
	char tempBuffer[35];
	char FT=0x90;
	
	if(UARTport.available()>0)
	{
		if(UARTport.find("~"))
		{
			UARTport.readBytes(tempBuffer,3);
			int len=(tempBuffer[1]-12);
			if(tempBuffer[2]==FT)
			{
				UARTport.readBytes(tempBuffer,len+12);
				for(int i=0;i<8;i++)
				{
					inputAdress[i]=tempBuffer[i];
				}
				for(int i=0;i<len;i++)
				{
					inputBuffer[i]=tempBuffer[i+11];
				}
				while(UARTport.available()>0)
				{
					UARTport.read();
				}
				return len;
			}
			else
			{
				while(UARTport.available()>0)
				{
					UARTport.read();
				}
				return 0;
			}
		}
	}
	else
	{
		return 0;
	}
}

char* XbeeLocal::recData(bool _param)
{
	char tempBuffer[35];
	char FT=0x90;

	if(UARTport.available()>0)
	{
		if(UARTport.find("~"))
		{
			UARTport.readBytes(tempBuffer,3);
			int len=(tempBuffer[1]-12);
			if(tempBuffer[2]==FT)
			{
				char* out=(char*)malloc(tempBuffer[1]-4);
				UARTport.readBytes(tempBuffer,len+12);
				for(int i=0;i<8;i++)
				{
					out[i]=tempBuffer[i];
				}
				for(int i=0;i<len;i++)
				{
					out[i+8]=tempBuffer[i+11];
				}
				while(UARTport.available()>0)
				{
					UARTport.read();
				}
				Serial.println(tempBuffer);
				Serial.println(out);
				return out;
			}
			else
			{
				while(UARTport.available()>0)
				{
					UARTport.read();
				}
				return "0";
			}
		}
	}
	else
	{
		return "0";
	}
}
int* XbeeLocal::nodeDiscover()
{
	char tempBuffer[35];
	int timeout=0;
	char FT=0x88;
	int devices=0;
	int* out=(int*)malloc(8+(devices*8));

	int Command[8]={0x7E,0x00,0x04,0x08,0x01,0x4E,0x44,0x64};
	for(int i=0;i<8;i++)
	{
		UARTport.write(Command[i]);
	}
	while(1)
	{
		if(UARTport.available()>0)
		{
			if(UARTport.find("~"))
			{
				UARTport.readBytes(tempBuffer,3);
				int len=tempBuffer[1];
				if(tempBuffer[2]==FT)
				{
					UARTport.readBytes(tempBuffer,len);
					for(int i=0;i<8;i++)
					{
						out[i+(devices*8)]=tempBuffer[i+6];
					}
					timeout=0;
					devices++;
				}
			}
		}
		else if(timeout==300)
		{
			if(devices>0)
			{
				attachedDevices=devices;
				return out;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			timeout+=1;
			delay(10);
		}
	}
}
int XbeeLocal::getDevices()
{
	return attachedDevices;
}

XbeeRemote::XbeeRemote(int _adress[8])
{
	for(int i=0;i<8;i++)
	{
		adress[i]=_adress[i];
	}
}
int* XbeeRemote::getAdress()
{
	return adress;
}