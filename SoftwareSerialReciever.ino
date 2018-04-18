#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11);
int x_axis;
int y_axis;
int z_button;
int c_button;
int x_accel;
void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  Serial.println("Connected");
  mySerial.begin(9600);
  mySerial.println("Connected");
}

void loop() {
  if (mySerial.available()) {
    data = mySerial.read();
    Serial.write(data);
  }
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
  
  if(data.next().equals("x")) {
    int x_axis = data.next();
  }
  if(data.next().equals("y")) {
    int y_axis = data.next();
  }
  if(data.next().equals("z")) {
    int z_button = data.next();
  }
  if(data.next().equals("c")) {
    int c_button = data.next();
  }
  if(data.next().equals("ax")) {
    int x_accel = data.next();
  }
  Serial.write("x_axis: " + x_axis + "\ty_axis: " + y_axis + "\tz_button: " + z_button + "\tc_button: " + c_button + "\tx_accel: " + x_accel + "\n");
}
