// //Skematic: http://www.martyncurrey.com/wp-content/uploads/2015/03/HC-06_01.jpg

#include <SoftwareSerial.h>

#define RX_PIN 50
#define TX_PIN 51

#define L_DRIVE_PWM 12
#define R_DRIVE_PWM 13

#define BT_VCC 2
#define MOTORS_VCC 8

namespace {
  const SoftwareSerial bt_serial(RX_PIN, TX_PIN);
  const int BAUD_RATE = 9600;
}
 
void setup() {
    Serial.begin(BAUD_RATE);
    Serial.println("Enter AT commands:");
    bt_serial.begin(BAUD_RATE);
  
    pinMode(L_DRIVE_PWM, OUTPUT);
    pinMode(R_DRIVE_PWM, OUTPUT);

    pinMode(BT_VCC, OUTPUT);
    pinMode(MOTORS_VCC, OUTPUT);
    
    digitalWrite(BT_VCC, HIGH);
    digitalWrite(MOTORS_VCC, HIGH);
}

void loop() {
    static String angle = "";
    static String strength = "";
    static boolean has_read_comma = false;

    static int angle_val = 0;

    if (angle_val <= 180) {
        analogWrite(R_DRIVE_PWM, 100);
        analogWrite(L_DRIVE_PWM, 0);
    }
    else {
        analogWrite(R_DRIVE_PWM, 0);
        analogWrite(L_DRIVE_PWM, 100);
    }
  
    if (bt_serial.available()) { 
        char input_byte = (char)(bt_serial.read());
        // Serial.println((char)input_byte);

        if (isDigit(input_byte)) {
            if (!has_read_comma) {
                angle += input_byte;
            }
            else {
                strength += input_byte;
            }
        }
        else if (input_byte == ',') {
            has_read_comma = true;
        }
        else if (input_byte == '\n') {
            Serial.println(angle + ", " + strength);
            angle_val = angle.toInt();
            
            angle = "";
            strength = "";
            has_read_comma = false;
        }
    }
  
    if (Serial.available()) {
        bt_serial.write(Serial.read());
    }
}
