//	https://free-electrons.com/labs/doc/nunchuk.pdf
//	https://github.com/rkrishnasanka/ArduinoNunchuk
//	http://www.robotshop.com/media/files/PDF/inex-zx-nunchuck-datasheet.pdf

#include <Wire.h>
#define nunchuk_ID 0x52
unsigned char buffer[6];
int count = 0;

void setup () {
	Serial.begin (9600);
	Wire.begin ();
	Wire.beginTransmission (nunchuk_ID);
	Wire.send (0x40);
	Wire.send (0x00);
	Wire.endTransmission ();
	delay (100);
}

void loop ()	{
	Wire.requestFrom (nunchuk_ID, 6);
	while (Wire.available ())	{
		buffer[count] = nunchuk_decode_byte (Wire.receive ());
		count++;
	}
	if (count >= 5)	{
		print ();
	}
	count = 0;
	send_zero ();
	delay (10);
}

void send_zero ()	{
	Wire.beginTransmission (nunchuk_ID);
	Wire.send (0x00);
	Wire.endTransmission ();
}

void print ()	{
	unsigned char joy_x_axis;
	unsigned char joy_y_axis;
	unsigned char z_button;
	unsigned char c_button;
	joy_x_axis = buffer[0];
	joy_y_axis = buffer[1];
	if ((buffer[5] & 0x01)!=0)
	{ z_button = 1; }
	else
	{ z_button = 0; }
	if ((buffer[5] & 0x02)!=0)
	{ c_button = 1; }
	else
	{ c_button = 0; }
	
	Serial.print (joy_x_axis, DEC);
	Serial.print ("\t");
	Serial.print (joy_y_axis, DEC);
	Serial.print ("\t");
	Serial.print (z_button, DEC);
	Serial.print ("\t");
	Serial.print (c_button, DEC);
	Serial.print ("\r\n");
}

char nunchuk_decode_byte (char x)	{
 x = (x ^ 0x17) + 0x17;
 return x;
}
