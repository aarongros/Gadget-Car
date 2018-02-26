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

const int RIGHT_LIGHT = 3;
const int LEFT_LIGHT = 4;
const int BRAKE_LIGHT = 5;
const int HAZARD_LIGHT = 6;
const int HEAD_LIGHT = 7;
const int RIGHT_PWM_DRIVE = 13;
const int LEFT_PWM_DRIVE = 12;
const int RIGHT_PWM_STEER = 10;
const int LEFT_PWM_STEER = 9;
#define STEER_INPUT A0

int y_value = 0;
int x_value = 0;
int accel_x = 0;
int accel_y = 0;
int accel_z = 0;
int steer_pot = 0;
int right_pwm_steer_speed = 0;
int right_pwm_drive_speed = 0;
bool c_button = false;
bool z_button = false;
bool headLightsOn = false;
bool brakeLightsOn = false;
bool leftLightsOn = false;
bool rightLightsOn = false;
bool hazardsOn = false;

void setup()  {
  Serial.begin(9600);
  ET.begin(details(mydata), &Serial);
  pinMode(RIGHT_LIGHT, OUTPUT);
  pinMode(LEFT_LIGHT, OUTPUT);
  pinMode(BRAKE_LIGHT, OUTPUT);
  pinMode(HAZARD_LIGHT, OUTPUT);
  pinMode(HEAD_LIGHT, OUTPUT);
  pinMode(RIGHT_PWM_DRIVE, OUTPUT);
  pinMode(LEFT_PWM_DRIVE, OUTPUT);
  pinMode(RIGHT_PWM_STEER, OUTPUT);
  pinMode(LEFT_PWM_STEER, OUTPUT);
  pinMode(STEER_INPUT, INPUT);
}

void loop() {
  if (ET.receiveData()) {
      x_value = mydata.joy_x;
      y_value = mydata.joy_y;
      z_button = mydata.z_button;
      c_button = mydata.c_button;
      accel_x = mydata.accel_x;
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
  checkLights();
  checkDrive();
  checkTurn();
  delay(1000);
}

void checkDrive() {
  right_pwm_drive_speed = map(y_value, 33, 225, -255, 255);
  int d = right_pwm_drive_speed;
  if(right_pwm_drive_speed >= 0) {
    analogWrite(RIGHT_PWM_DRIVE, right_pwm_drive_speed);
    analogWrite(LEFT_PWM_DRIVE, 0);
    Serial.print("Drive F: ");
      Serial.println(d);
  }
  else if(right_pwm_drive_speed < 0)  {
    analogWrite(RIGHT_PWM_DRIVE, 0);
    analogWrite(LEFT_PWM_DRIVE, (-1 * right_pwm_drive_speed));
    Serial.print("Drive B: ");
      Serial.println(d);
  }
}

void checkTurn()  {
  steer_pot = analogRead(STEER_INPUT);
  right_pwm_steer_speed = map(x_value, 25 , 223, -255, 255);
  int prints = right_pwm_steer_speed;
  if(!((steer_pot >= 950 && right_pwm_steer_speed > 0) || (steer_pot <= 75 && right_pwm_steer_speed < 0))) {
    if(right_pwm_steer_speed >= 0) {
      Serial.print("Steer R: ");
      Serial.println(prints);
      analogWrite(RIGHT_PWM_STEER, right_pwm_steer_speed);
      analogWrite(LEFT_PWM_STEER, 0);
    }
    else if(right_pwm_steer_speed < 0)  {
      Serial.print("Steer L: ");
      Serial.println(prints);
      analogWrite(RIGHT_PWM_STEER, 0);
      analogWrite(LEFT_PWM_STEER, (-1 * right_pwm_steer_speed));
    }
  }
}

void checkLights()  {
  checkHazards();
  checkRightLights();
  checkLeftLights();
  checkBrakeLights();
  checkHeadLights();
}

void checkHeadLights()  {
  if(!headLightsOn && !z_button) {
    digitalWrite(HEAD_LIGHT, HIGH);
    headLightsOn = !headLightsOn;
  }
  else if(headLightsOn && z_button) {
    digitalWrite(HEAD_LIGHT, LOW);
    headLightsOn = !headLightsOn;
  }
}

void checkHazards() {
  if(!hazardsOn && !c_button)  {
    digitalWrite(HAZARD_LIGHT, HIGH);
    hazardsOn = !hazardsOn;
  }
  else if(hazardsOn && c_button) {
    digitalWrite(HAZARD_LIGHT, LOW);
    hazardsOn = !hazardsOn;
  }
}

void checkRightLights() {
  if(!rightLightsOn && right_pwm_steer_speed > 50)  {
    digitalWrite(RIGHT_LIGHT, HIGH);
    rightLightsOn = !rightLightsOn;
  }
  else if(rightLightsOn && !(right_pwm_steer_speed > 50)) {
    digitalWrite(RIGHT_LIGHT, LOW);
    rightLightsOn = !rightLightsOn;
  }
}

void checkLeftLights()  {
  if(!leftLightsOn && right_pwm_steer_speed < -50) {
    digitalWrite(LEFT_LIGHT, HIGH);
    leftLightsOn = !leftLightsOn;
  }
  else if(leftLightsOn && !(right_pwm_steer_speed < -50))  {
    digitalWrite(LEFT_LIGHT, LOW);
    leftLightsOn = !leftLightsOn;
  }
}

void checkBrakeLights() {
  if(!brakeLightsOn && right_pwm_drive_speed < -50)  {
    digitalWrite(BRAKE_LIGHT, HIGH);
    brakeLightsOn = !brakeLightsOn;
  }
  else if(brakeLightsOn && !(right_pwm_drive_speed < -50))  {
    digitalWrite(BRAKE_LIGHT, LOW);
    brakeLightsOn = !brakeLightsOn;
  }
}
