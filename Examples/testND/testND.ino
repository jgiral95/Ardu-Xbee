#include <SoftwareSerial.h>
#include <ArdXbee.h>

XbeeLocal xbee=XbeeLocal(2,3);

void setup()
{
	Serial.begin(9600);
}

int* dirs;
int len;

void loop()
{
  dirs=xbee.nodeDiscover();
  len=xbee.getDevices()*8;
  for(int i=0;i<len;i++)
  {
    Serial.write(dirs[i]);
  }
  while(1)
  {
    ;
  }
}

