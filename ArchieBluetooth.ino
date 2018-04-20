/*
//Skematic: http://www.martyncurrey.com/wp-content/uploads/2015/03/HC-06_01.jpg
Arduino              Bluetooth Module
5V   ----------------->   VCC
GND  ----------------->   GND
3   ---> 1KΩ --> 2KΩ --> GND (Voltage Divider)
               ------->   RX
2   ----------------->   TX
*/

#include <SoftwareSerial.h>
SoftwareSerial BTserial(2, 3);
 
void setup() 
{
    Serial.begin(9600);
    Serial.println("Enter AT commands:");
    BTserial.begin(9600);  
}
 
void loop()
{
    if (BTserial.available())
    {  
        Serial.write(BTserial.read());
    }
  
    if (Serial.available())
    {
        BTserial.write(Serial.read());
    }
}
