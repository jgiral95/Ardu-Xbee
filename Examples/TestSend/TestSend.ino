#include <ArdXbee.h>
#include <SoftwareSerial.h>

XbeeLocal xbee=XbeeLocal(2,3);
char mensaje[]="Better Call Saul";

void setup()
{
  Serial.begin(9600);
  int adress[]={0x00,0x13,0xA2,0x00,0x40,0x61,0x4B,0x06};
  xbee.setAdress(adress);
}

void loop()
{
  if(Serial.available())
  {
    if(xbee.sendData(mensaje))
    {
      Serial.println("Mensaje Enviado Satisfactoriamente");
    }
    else
    {
      Serial.println("El mensaje no ha sido enviado");
    }
    while(Serial.available()>0)
    {
      Serial.read();
    }
  }
}
