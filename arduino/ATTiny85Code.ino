#include <TinyWireM.h>                  // I2C Master lib for ATTinys which use USI

#define DS1621_ADDR   0x48              // 7 bit I2C address for DS1621 temperature sensor

void setup(){
  Serial.begin(9600);
  TinyWireM.begin();                    // initialize I2C lib
  Init_Temp();                          // Setup DS1621
  delay (3000);
}


void loop(){
  Get_Temp();
  delay (2000);
}


void Init_Temp(){ // Setup the DS1621 for one-shot mode
  TinyWireM.beginTransmission(DS1621_ADDR);
  TinyWireM.send(0xAC);                 // Access Command Register
  TinyWireM.send(B00000001);            // Using one-shot mode for battery savings
  //TinyWireM.send(B00000000);          // if setting continious mode for fast reads
  TinyWireM.endTransmission();          // Send to the slave
}


void Get_Temp(){  // Get the temperature from a DS1621
  TinyWireM.beginTransmission(DS1621_ADDR);
  TinyWireM.send(0xEE);                 // if one-shot, start conversions now
  TinyWireM.endTransmission();          // Send 1 byte to the slave
  delay(750);                           // if one-shot, must wait ~750 ms for conversion
  TinyWireM.beginTransmission(DS1621_ADDR);
  TinyWireM.send(0xAA);                 // read temperature (for either mode)
  TinyWireM.endTransmission();          // Send 1 byte to the slave
  TinyWireM.requestFrom(DS1621_ADDR,1); // Request 1 byte from slave
  Serial.print(TinyWireM.receive());          // get the temperature
}
