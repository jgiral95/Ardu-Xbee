import serial
import numpy as np

ser=serial.Serial('/dev/ttyUSB1',baudrate=9600)

adress=[0x00,0x13,0xA2,0x00,0x40,0x61,0x4B,0x06]
dest=0x07
mensaje="Hello"
maxlen=19+len(mensaje)

chksum=0;

frame=range(maxlen)

frame[0]=0x7E
frame[1]=0x00
frame[2]=maxlen-4
frame[3]=0x10
frame[4]=0x01
frame[5:12]=adress
frame[13]=0xFF
frame[14]=0xFE
frame[15]=0x00
frame[16]=0x00
frame[17]=dest
for i in range(len(mensaje)):
	frame[i+18]=ord(mensaje[i])
for i in range(maxlen-4):
	chksum+=frame[i+3]
frame[maxlen-1]=np.uint8(0xFF-chksum)

for i in frame:
	ser.write(chr(i))
