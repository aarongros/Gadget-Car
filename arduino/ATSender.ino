#include <TinyWireM.h>                  // I2C Master lib for ATTinys which use USI
#include <EasyTransfer.h>     //EasyTransfer library to facilitate sending the joystick values over bluetooth while making sure we get all the data
#include <SoftwareSerial.h>   //Software Serial to facilitate serial communication through bluetooth

#define RxD 1
#define TxD 2

SoftwareSerial mySerial(RxD, TxD);
EasyTransfer ET;

int count = 0;
unsigned char buffer[6];
unsigned char joy_x_axis;
unsigned char joy_y_axis;
unsigned char z_button;
unsigned char c_button;
unsigned int accel_x_axis;
#define nunchuk_ID 0x52

struct SEND_DATA_STRUCTURE{
  int16_t joy_x;
  int16_t joy_y;
  int16_t c;
  int16_t z;
  int16_t accel_x;
};
SEND_DATA_STRUCTURE mydata;

void setup(){
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);
  Serial.begin(9600);
  mySerial.begin(9600);
  ET.begin(details(mydata), &mySerial);
  TinyWireM.begin();
  i2c_setup();
  delay (3000);
}

void loop(){
  Get_Temp();
  delay (2000);
}

void i2c_setup(){
  TinyWireM.beginTransmission(nunchuk_ID);
  TinyWireM.write(0x40);
  TinyWireM.write(0x00);
  TinyWireM.endTransmission();
  delay(100);
}

void Get_Temp(){
  TinyWireM.requestFrom (nunchuk_ID, 6);
  while (TinyWireM.available ()) {
    buffer[count] = nunchuk_decode_byte (TinyWireM.read ());
    count++;
  }
  if (count >= 5) {
    filterJoyData();
    ET.sendData();
  }
  count = 0;
  send_zero ();
  delay (10);
}

void filterJoyData() {
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
}

void send_zero() {
  TinyWireM.beginTransmission (nunchuk_ID);
  TinyWireM.write (0x00);
  TinyWireM.endTransmission ();
}

char nunchuk_decode_byte(char x) {
  x = (x ^ 0x17) + 0x17;
  return x;
}
