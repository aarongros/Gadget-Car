#include <SoftwareSerial.h>
#include <EasyTransfer.h>
char input;
SoftwareSerial mySerial(7, 8);
EasyTransfer ET; 

struct RECEIVE_DATA_STRUCTURE{
  int16_t joy_x;
  int16_t joy_y;
  int16_t c;
  int16_t z;
  int16_t accel_x;
};
RECEIVE_DATA_STRUCTURE mydata;
void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  ET.begin(details(mydata), &mySerial);
}

void loop() {
  if(ET.receiveData()){
    Serial.print(mydata.joy_x);
    Serial.print("\t");
    Serial.print(mydata.joy_y);
    Serial.print("\t");
    Serial.print(mydata.c);
    Serial.print("\t");
    Serial.print(mydata.z);
    Serial.print("\t");
    Serial.println(mydata.accel_x);
  }
}
