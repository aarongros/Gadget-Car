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
    Serial.print(mydata.joy_x);
    Serial.print("\t");
    Serial.print(mydata.joy_y);
    Serial.print("\t");
    Serial.print(mydata.z_button);
    Serial.print("\t");
    Serial.print(mydata.c_button);
    Serial.print("\t");
    Serial.println(mydata.accel_x);
  }
  delay(50);
}
