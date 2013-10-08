#include <SoftwareSerial.h>
#include <ArdXbee.h>

XbeeLocal xbee=XbeeLocal(2,3);
uint8_t devices;
char* tempBuffer;
int modulos[3][8];



void setup()
{
	Serial.begin(9600);
	pinMode(13,OUTPUT);
	pinMode(8,INPUT); 
}

void loop() 
{
	Serial.println("Loading...");
	delay(5000);
        // Configuracion de la Red Xbee
	int* dirs=xbee.nodeDiscover(); 					//Almadena las direcciones de los modulos conectados
	devices=xbee.getDevices();						//Numero de Modulos Detectados
	//int modulos[devices][8];
	for (int i=0;i<devices;i++)
	{
		for(int j=0;j<8;j++)
		{
			modulos[i][j]=dirs[j+(8*i)];
		}
	}
	Serial.print("Se detectaron ");
	Serial.print(devices);
	Serial.println(" Modulos");
	for (int i=0;i<devices;i++)
	{
		Serial.print("El modulo ");
		Serial.print(i);
		Serial.print(" Termina en: ");
		Serial.println(modulos[i][7]);
	}
	Serial.println("Esperando por mensajes...");

	// Rutina de Redireccionamiento

	while(true)
	{
		int mensaje=xbee.recData();
		if(mensaje>0)
		{
			digitalWrite(13,1); 
			bool ready=false;
			char command;
			uint8_t sender;
			uint8_t dest;
			Serial.print("Se ha recibido un mensaje que proviene del Modulo ");
			sender=checksender();
			Serial.println(sender);
			Serial.print("y esta destinado para el modulo ");
			dest=checkdest();
			Serial.println(dest);
			char buffer[mensaje];
			for(int i=0;i<mensaje;i++)
			{
				buffer[i]=xbee.inputBuffer[i+1];
			}
			if(digitalRead(8)) // Modo Manual
			{
				Serial.println("S = Enviar");
				Serial.println("D = Elimiar");
				Serial.println("B = BroadCast");
				Serial.println("A = Almacenar");
				Serial.println("P = Enviar Almacenado");			
				while(!ready)
				{
					if(Serial.available()>0)
					{
						command=Serial.read();
						switch (command) 
						{
						    case 'S':
						    	xbee.setAdress(modulos[dest]);
						    	xbee.sendData(buffer);
						    	ready=true;
						    	break;
						    case 'D':
						    	ready=true;
						      break;
						    case 'B':
						    	for(int i=0;i<devices;i++)
						    	{
						    		xbee.setAdress(modulos[i]);
						    		xbee.sendData(buffer);
						    	}
						    	ready=true;
						    	break;
						    case 'A':
						    	tempBuffer=buffer;
						    	Serial.println(tempBuffer);
						    	ready=true;
						    	break;
						    case 'P':
						    	xbee.setAdress(modulos[dest]);
						    	xbee.sendData(tempBuffer);
						    	ready=true;
						    	break;
						}
					}
				}
			}
			else	//Modo Automatico
			{
				xbee.setAdress(modulos[dest]);
				xbee.sendData(buffer);
				ready=true;
				Serial.println("Mensaje Enviado!");
			}
			digitalWrite(13,0); 
			Serial.println("Hecho!");
			Serial.println("Esperando por mas mensajes...");
		}
		
	}
}

uint8_t checksender()
{
	for(int i=0;i<devices;i++)
	{
		if(xbee.inputAdress[7]==modulos[i][7])
		{
			return i;
		}
	}
}
uint8_t checkdest()
{
	for(int i=0;i<devices;i++)
	{
		if(xbee.inputBuffer[0]==modulos[i][7])
		{
			return i;
		}
	}
}
