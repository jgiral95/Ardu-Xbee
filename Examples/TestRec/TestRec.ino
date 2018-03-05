#include <SoftwareSerial.h>
#include <ArdXbee.h>

XbeeLocal xbee=XbeeLocal(2,3);

void setup()
{
	Serial.begin(9600);
}

int mensaje;

void loop()
{
  mensaje=xbee.recData();
  if(mensaje>0)
  {
   Serial.print("El mensaje recibido tiene una longitud de ");
   Serial.print(mensaje);
   Serial.println(" y es:");
   for(int i=0;i<mensaje;i++)
   {
     Serial.print(xbee.inputBuffer[i]);   
   }
   Serial.println("");
  }
}
