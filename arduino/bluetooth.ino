// //Skematic: http://www.martyncurrey.com/wp-content/uploads/2015/03/HC-06_01.jpg

#include <SoftwareSerial.h>

#define RX_PIN 7
#define TX_PIN 8

namespace {
  const SoftwareSerial bt_serial(RX_PIN, TX_PIN);
  const int BAUD_RATE = 9600;
}
 
void setup() {
    Serial.begin(BAUD_RATE);
    Serial.println("Enter AT commands:");
    bt_serial.begin(BAUD_RATE);
}
 
void loop() {
    if (bt_serial.available()) {  
        Serial.write(bt_serial.read());
    }
  
    if (Serial.available()) {
        bt_serial.write(Serial.read());
    }
}
