#include <EasyTransfer.h>

EasyTransfer ET;

struct RECIEVE_DATA_STRUCTURE {
  int joy_x;
  int joy_y;
  int z_button;
  int c_button;
  int accel_x;
};

RECIEVE_DATA_STRUCTURE mydata;

void setup() 
{
    Serial.begin(9600);
    ET.begin(details(mydata), &Serial);
}
 
void loop()
{
  if (ET.receiveData()) {
    Serial.println(mydata.joy_x);
    Serial.println(mydata.joy_y);
    Serial.println(mydata.z_button);
    Serial.println(mydata.c_button);
    Serial.println(mydata.accel_x);
  }
  delay(50);
}
