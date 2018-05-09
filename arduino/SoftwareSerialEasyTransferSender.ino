#include <Wire.h>
#include <EasyTransfer.h>
#define nunchuk_ID 0x52
unsigned char buffer[6];
int count = 0;
unsigned char joy_x_axis;
unsigned char joy_y_axis;
unsigned char z_button;
unsigned char c_button;
unsigned int accel_x_axis;
EasyTransfer ET; 

struct SEND_DATA_STRUCTURE{
  int16_t joy_x;
  int16_t joy_y;
  int16_t c;
  int16_t z;
  int16_t accel_x;
};

SEND_DATA_STRUCTURE mydata;

void setup() {
  Serial.begin (9600);
  ET.begin(details(mydata), &Serial);
  Wire.begin ();
  Wire.beginTransmission (nunchuk_ID);
  Wire.write (0x40);
  Wire.write (0x00);
  Wire.endTransmission ();
  delay (100);
}

void loop() {
  Wire.requestFrom (nunchuk_ID, 6);
  while (Wire.available ()) {
    buffer[count] = nunchuk_decode_byte (Wire.read ());
    count++;
  }
  if (count >= 5) {
    print ();
  }
  count = 0;
  send_zero ();
  delay (10);
}

void send_zero () {
  Wire.beginTransmission (nunchuk_ID);
  Wire.write (0x00);
  Wire.endTransmission ();
}

void print () {
  
  joy_x_axis = buffer[0];
  joy_y_axis = buffer[1];
  accel_x_axis = (buffer[2]) << 2;
  if ((buffer[5] & 0x01)!=0)  {
    z_button = 1;
  }
  else  {
    z_button = 0;
  }
  if ((buffer[5] & 0x02)!=0)  {
    c_button = 1;
  }
  else  {
    c_button = 0;
  }
  accel_x_axis += ((buffer[5]) >> 2) & 0x03;
  
  mydata.joy_x = (int)joy_x_axis;
  mydata.joy_y = (int)joy_y_axis;
  mydata.c = (int)c_button;
  mydata.z = (int)z_button;
  mydata.accel_x = accel_x_axis;

   ET.sendData();
   delay(50);
}

char nunchuk_decode_byte (char x) {
  x = (x ^ 0x17) + 0x17;
  return x;
}
