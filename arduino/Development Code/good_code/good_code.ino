// bluetooth module schematic: http://www.martyncurrey.com/wp-content/uploads/2015/03/HC-06_01.jpg

#include <SoftwareSerial.h>

#define RX_PIN 50
#define TX_PIN 51

#define L_DRIVE_PWM 12
#define R_DRIVE_PWM 13
#define L_STEER_PWM 9
#define R_STEER_PWM 10
#define STEER_POT A0

#define BT_VCC 2
#define MOTORS_VCC 8

namespace JoystickData {
    int angle = 0;      // 0 <= angle < 360
    int strength = 0;   // 0 <= strength <= 100
}

namespace {
    const SoftwareSerial bt_serial(RX_PIN, TX_PIN);
    const int BAUD_RATE = 9600;
    const int MAX_DRIVE_STRENGTH = 50;
    const int STEER_REGISTER_OFFSET = 30;
    const int TURNING_RADIUS = 50;
    const int STEER_CENTER = 345;
}

void handleRearWheelDrive();
void handleSteering();
 
void setup() {
    Serial.begin(BAUD_RATE);
    Serial.println("Enter AT commands:");
    bt_serial.begin(BAUD_RATE);

    // I/O
    
    pinMode(L_DRIVE_PWM, OUTPUT);
    pinMode(R_DRIVE_PWM, OUTPUT);
    pinMode(L_STEER_PWM, OUTPUT);
    pinMode(R_STEER_PWM, OUTPUT);
    pinMode(STEER_POT, INPUT);

    // power
    
    pinMode(BT_VCC, OUTPUT);
    pinMode(MOTORS_VCC, OUTPUT);
    
    digitalWrite(BT_VCC, HIGH);
    digitalWrite(MOTORS_VCC, HIGH);
}

void loop() {
    // temp variables
    static String angle = "";
    static String strength = "";
    static boolean has_read_comma = false;
    
    handleRearWheelDrive();
    handleSteering();
  
    if (bt_serial.available()) { 
        char input_byte = (char)(bt_serial.read());

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
            // Serial.println(angle + ", " + strength);

            // set the JoystickData data
            JoystickData::angle = angle.toInt();
            JoystickData::strength = strength.toInt();

            // clear temp variables
            angle = "";
            strength = "";
            has_read_comma = false;
        }
    }
  
    if (Serial.available()) {
        bt_serial.write(Serial.read());
    }
}

void handleRearWheelDrive() {
    // map the raw strength value to a new strength between 0 and the maximum strength
    int strength = map(JoystickData::strength, 0, 100, 0, MAX_DRIVE_STRENGTH);
    
    if (strength == 0) {
        // stay still
        analogWrite(R_DRIVE_PWM, 0);
        analogWrite(L_DRIVE_PWM, 0);
    }
    else if (JoystickData::angle <= 180 - STEER_REGISTER_OFFSET 
        && JoystickData::angle >= 0 + STEER_REGISTER_OFFSET) 
    {
        // drive backwards
        analogWrite(R_DRIVE_PWM, 0);
        analogWrite(L_DRIVE_PWM, strength);
    }
    else if (JoystickData::angle >= 180 + STEER_REGISTER_OFFSET
        && JoystickData::angle <= 360 - STEER_REGISTER_OFFSET) {
        // drive forwards
        analogWrite(R_DRIVE_PWM, strength);
        analogWrite(L_DRIVE_PWM, 0);
    }
    else {
        // stay still because the car is only turning and not driving
        analogWrite(R_DRIVE_PWM, 0);
        analogWrite(L_DRIVE_PWM, 0);
    }
}

bool isTurningLeft() {
    return JoystickData::angle >= 90 + STEER_REGISTER_OFFSET 
      && JoystickData::angle <= 270 - STEER_REGISTER_OFFSET;
}

bool isTurningRight() {
    int angle = JoystickData::angle;
    return (angle > 0 && angle <= 90 - STEER_REGISTER_OFFSET) 
      || (angle >= 270 + STEER_REGISTER_OFFSET && angle < 360);
}

void handleSteering() {
    // Serial.println(JoystickData::angle);
    
    int steer_pot_val = analogRead(STEER_POT);
    Serial.println(steer_pot_val);
    
    if (isTurningLeft()) {
        if (steer_pot_val <= STEER_CENTER - TURNING_RADIUS) return;
        
        analogWrite(R_STEER_PWM, 0);
        analogWrite(L_STEER_PWM, 255);
    }
    else if (isTurningRight()) {
        if (steer_pot_val >= STEER_CENTER + TURNING_RADIUS) return;
        
        analogWrite(R_STEER_PWM, 255);
        analogWrite(L_STEER_PWM, 0);
    }
    else {
        analogWrite(R_STEER_PWM, 0);
        analogWrite(L_STEER_PWM, 0);
    }
}
