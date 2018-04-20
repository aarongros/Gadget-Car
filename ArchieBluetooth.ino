Arduino              Bluetooth Module
5V   ----------------->   VCC
GND  ----------------->   GND
TX   ---> 1KΩ --> 2KΩ --> GND (Voltage Divider)
               ------->   RX
RX   ----------------->   TX

void setup()  {
  Serial.begin(9600);
}

void loop() {
  if(Serial.available() > 0)  {
    Serial.print(Serial.read());
  }
}
