#include <Wire.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11);
#define nunchuk_ID 0x52
unsigned char buffer[6];
int count = 0;
unsigned char joy_x_axis;
unsigned char joy_y_axis;
unsigned char z_button;
unsigned char c_button;
int x_axis;
int y_axis;
int z;
int c;
int accel_x_axis;


void setup(){
  Serial.begin(9600);
  Wire.begin ();
  Wire.beginTransmission (nunchuk_ID);
  Wire.write (0x40);
  Wire.write (0x00);
  Wire.endTransmission ();
  Serial.begin(57600);
  while (!Serial) {
    ;
  }
  delay (100);
  Serial.println("Connected");

  mySerial.begin(9600);
  mySerial.println("Connected");
}

void loop(){
  Wire.requestFrom (nunchuk_ID, 6);
  while (Wire.available ()) {
    buffer[count] = nunchuk_decode_byte (Wire.read());
    count++;
  }
  if (count >= 5) {
    joy_x_axis = buffer[0];
    joy_y_axis = buffer[1];
    accel_x_axis = (buffer[2]) << 2;
    if ((buffer[5] & 0x01)!=0)
    { z_button = 1; }
    else
    { z_button = 0; }
    if ((buffer[5] & 0x02)!=0)
    { c_button = 1; }
    else
    { c_button = 0; }
    accel_x_axis += ((buffer[5]) >> 2) & 0x03;
  }
  count = 0;
  send_zero ();
  
  
  x_axis = int(joy_x_axis);
  y_axis = int(joy_y_axis);
  z = int(z_button);
  c = int(c_button);
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  if (Serial.available()) {
    mySerial.write("x: " + x_axis + "\ty: " + y_axis + "\tz: " + z + "\tc: " + c + "\tax: " + accel_x_axis + "\n");
  }
  delay(100);
}

void send_zero () {
  Wire.beginTransmission (nunchuk_ID);
  Wire.write (0x00);
  Wire.endTransmission ();
}

char nunchuk_decode_byte (char x) {
  x = (x ^ 0x17) + 0x17;
  return x;
}
