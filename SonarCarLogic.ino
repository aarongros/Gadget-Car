#include <EasyTransfer.h> //EasyTransfer Library to recieve a data structure containing all the joystick values
EasyTransfer ET;

struct RECIEVE_DATA_STRUCTURE { //Instantiating the data structure with the same data types as the sender
  int joy_x;
  int joy_y;
  int z_button;
  int c_button;
  int accel_x;
};
RECIEVE_DATA_STRUCTURE mydata;

//Instantiate all variables representing inputs and outputs on the arduino
const int RIGHT_LIGHT = 3;
const int LEFT_LIGHT = 4;
const int BRAKE_LIGHT = 5;
const int HAZARD_LIGHT = 6;
const int HEAD_LIGHT = 7;
const int RIGHT_PWM_DRIVE = 13;
const int LEFT_PWM_DRIVE = 12;
const int RIGHT_PWM_STEER = 10;
const int LEFT_PWM_STEER = 9;
const int DRIVE_ENABLE = 8;
const int BLUETOOTH_VCC = 2;
const int TRIG_PIN_FRONT = 22;
const int ECHO_PIN_FRONT = 23;
const int TRIG_PIN_FRONT_LEFT = 24;
const int ECHO_PIN_FRONT_LEFT = 25;
const int TRIG_PIN_FRONT_RIGHT = 26;
const int ECHO_PIN_FRONT_RIGHT = 27;
const int TRIG_PIN_BACK = 28;
const int ECHO_PIN_BACK = 29;
const int TRIG_PIN_BACK_LEFT = 30;
const int ECHO_PIN_BACK_LEFT = 31;
const int TRIG_PIN_BACK_RIGHT = 32;
const int ECHO_PIN_BACK_RIGHT = 33;
#define STEER_INPUT A0

//Instantiating all the variables needed for the logic
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
long durationFront;
int distanceFront;
long durationFrontLeft;
int distanceFrontLeft;
long durationFrontRight;
int distanceFrontRight;
long durationBack;
int distanceBack;
long durationBackLeft;
int distanceBackLeft;
long durationBackRight;
int distanceBackRight;

void setup()  {
  Serial.begin(9600); //Begin Serial to recieve data
  ET.begin(details(mydata), &Serial); //Begin EasyTransfer to easily recieve data structures
  
  //Set all necesary inputs and outputs
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
  pinMode(DRIVE_ENABLE, OUTPUT);
  pinMode(BLUETOOTH_VCC, OUTPUT);
  pinMode(TRIG_PIN_FRONT, OUTPUT);
  pinMode(ECHO_PIN_FRONT, INPUT);
  pinMode(TRIG_PIN_FRONT_LEFT, OUTPUT);
  pinMode(ECHO_PIN_FRONT_LEFT, INPUT);
  pinMode(TRIG_PIN_FRONT_RIGHT, OUTPUT);
  pinMode(ECHO_PIN_FRONT_RIGHT, INPUT);
  pinMode(TRIG_PIN_BACK, OUTPUT);
  pinMode(ECHO_PIN_BACK, INPUT);
  pinMode(TRIG_PIN_BACK_LEFT, OUTPUT);
  pinMode(ECHO_PIN_BACK_LEFT, INPUT);
  pinMode(TRIG_PIN_BACK_RIGHT, OUTPUT);
  pinMode(ECHO_PIN_BACK_RIGHT, INPUT);
    
  digitalWrite(BLUETOOTH_VCC, HIGH);  //Set bluetooth high in code to facilitate uploading code since TX and RX won't intervene this way
  digitalWrite(DRIVE_ENABLE, HIGH); //Set Drive enable to the motor driver for the back wheels will activate
}

void loop() {
  if (ET.receiveData()) { //When EasyTransfer recieves data we assign all the data structure's values to the class variables
      x_value = mydata.joy_x;
      y_value = mydata.joy_y;
      z_button = mydata.z_button;
      c_button = mydata.c_button;
      accel_x = mydata.accel_x;
  }
  checkLights();  //Checks all the light logic
  checkDrive();  //Checks the back motor logic
  checkTurn();  //Checks the turning logic
  delay(50);
}

void checkDrive() {
  right_pwm_drive_speed = map(y_value, 33, 225, -255, 255); //Map the joystick y value with range 0 - 255 to a range of -255 - 255
  if(right_pwm_drive_speed >= 0) {  //If the maped speed is positive we move forward
    analogWrite(RIGHT_PWM_DRIVE, right_pwm_drive_speed);
    analogWrite(LEFT_PWM_DRIVE, 0);
  }
  else if(right_pwm_drive_speed < 0)  { //If the maped speed is negative we move backwards
    analogWrite(RIGHT_PWM_DRIVE, 0);
    analogWrite(LEFT_PWM_DRIVE, (-1 * right_pwm_drive_speed));
  }
}

void checkTurn()  { //Assigns specific joy value to specific pot value and turns the wheel until it is at the position
  steer_pot = analogRead(STEER_INPUT);
  right_pwm_steer_speed = map(x_value, 25 , 223, 0, 1023);
  if(((steer_pot >= 900 && right_pwm_steer_speed < 900) || (steer_pot <= 100 && right_pwm_steer_speed > 100) || (steer_pot < 900 && steer_pot > 100)) && ((right_pwm_steer_speed < steer_pot - 20) || (right_pwm_steer_speed > steer_pot + 20))) {
  //If the pot value is greater than 900 and the turn value is less that 900, or the pot value is less than 100 and the turn value is greater than 100, or the pot value in between 100 and 900, and the turn value is further than 20 from the pot value the conditions will be true
        if(right_pwm_steer_speed < steer_pot) { //Turn right
            analogWrite(RIGHT_PWM_STEER, 175);
            analogWrite(LEFT_PWM_STEER, 0);
        }
        else if(right_pwm_steer_speed > steer_pot)  { //Turn left
            analogWrite(RIGHT_PWM_STEER, 0);
            analogWrite(LEFT_PWM_STEER, 175);
        }
  }
  else  { //Turn of steering when it doesn't meet the conditions so it doesn't keep turning
    analogWrite(RIGHT_PWM_STEER, 0);
    analogWrite(LEFT_PWM_STEER, 0);
  }
}

void checkLights()  { //Checks all the liferent light events
  checkHazards();
  checkRightLights();
  checkLeftLights();
  checkBrakeLights();
  checkHeadLights();
}

void checkHeadLights()  {
  if(!headLightsOn && !z_button) {  //If headlights are not on and the z button is pressed we turn the headlights on
    digitalWrite(HEAD_LIGHT, HIGH);
    headLightsOn = !headLightsOn;
  }
  else if(headLightsOn && z_button) { //If headlights are on and the z button is not pressed we turn the headlights off
    digitalWrite(HEAD_LIGHT, LOW);
    headLightsOn = !headLightsOn;
  }
}

void checkHazards() {
  if(!hazardsOn && !c_button)  {  //If hazards are not on and the c button is pressed we turn the hazards on
    digitalWrite(HAZARD_LIGHT, HIGH);
    hazardsOn = !hazardsOn;
  }
  else if(hazardsOn && c_button) {  //If hazards are on and the c button is not pressed we turn the hazards off
    digitalWrite(HAZARD_LIGHT, LOW);
    hazardsOn = !hazardsOn;
  }
}

void checkRightLights() {
  if(!rightLightsOn && right_pwm_steer_speed > 50)  { //If the right signal is not on and the turn value is greater than 50 we turn the right blinker on
    digitalWrite(RIGHT_LIGHT, HIGH);
    rightLightsOn = !rightLightsOn;
  }
  else if(rightLightsOn && !(right_pwm_steer_speed > 50)) { //If the right signal is on and the turn value is not greater than 50 we turn the right blinker off
    digitalWrite(RIGHT_LIGHT, LOW);
    rightLightsOn = !rightLightsOn;
  }
}

void checkLeftLights()  {
  if(!leftLightsOn && right_pwm_steer_speed < -50) {  //If the left signal is not on and the turn value is less than -50 we turn the left blinker on
    digitalWrite(LEFT_LIGHT, HIGH);
    leftLightsOn = !leftLightsOn;
  }
  else if(leftLightsOn && !(right_pwm_steer_speed < -50))  {  //If the left signal is on and the turn value is not less than -50 we turn the left blinker off
    digitalWrite(LEFT_LIGHT, LOW);
    leftLightsOn = !leftLightsOn;
  }
}

void checkBrakeLights() {
  if(!brakeLightsOn && right_pwm_drive_speed < -50)  {  //If the brake lights are not on and the drive value is less than -50 we turn the brake lights on
    digitalWrite(BRAKE_LIGHT, HIGH);
    brakeLightsOn = !brakeLightsOn;
  }
  else if(brakeLightsOn && !(right_pwm_drive_speed < -50))  { //If the brake lights are on and the drive value is not less than -50 we turn the breake lights off
    digitalWrite(BRAKE_LIGHT, LOW);
    brakeLightsOn = !brakeLightsOn;
  }
}

void scanSurroundings() { //Reads all the ultrasonic values and finds their distances
  //Clears the TRIN_PINs
  digitalWrite(TRIG_PIN_FRONT, LOW);
  digitalWrite(TRIG_PIN_FRONT_LEFT, LOW);
  digitalWrite(TRIG_PIN_FRONT_RIGHT, LOW);
  digitalWrite(TRIG_PIN_BACK, LOW);
  digitalWrite(TRIG_PIN_BACK_LEFT, LOW);
  digitalWrite(TRIG_PIN_BACK_RIGHT, LOW);
  delayMicroseconds(2);
  
  //Sets the TRIG_PINs HIGH for 10 micro seconds
  digitalWrite(TRIG_PIN_FRONT, HIGH);
  digitalWrite(TRIG_PIN_FRONT_LEFT, HIGH);
  digitalWrite(TRIG_PIN_FRONT_RIGHT, HIGH);
  digitalWrite(TRIG_PIN_BACK, HIGH);
  digitalWrite(TRIG_PIN_BACK_LEFT, HIGH);
  digitalWrite(TRIG_PIN_BACK_RIGHT, HIGH);
  delayMicroseconds(10);
  
  digitalWrite(TRIG_PIN_FRONT, LOW);
  digitalWrite(TRIG_PIN_FRONT_LEFT, LOW);
  digitalWrite(TRIG_PIN_FRONT_RIGHT, LOW);
  digitalWrite(TRIG_PIN_BACK, LOW);
  digitalWrite(TRIG_PIN_BACK_LEFT, LOW);
  digitalWrite(TRIG_PIN_BACK_RIGHT, LOW);
  
  //Reads the echoPin, returns the sound wave travel time in microseconds
  durationFront = pulseIn(ECHO_PIN_FRONT, HIGH);
  durationFrontLeft = pulseIn(ECHO_PIN_FRONT_LEFT, HIGH);
  durationFrontRight = pulseIn(ECHO_PIN_FRONT_RIGHT, HIGH);
  durationBack = pulseIn(ECHO_PIN_BACK, HIGH);
  durationBackLeft = pulseIn(ECHO_PIN_BACK_LEFT, HIGH);
  durationBackRight = pulseIn(ECHO_PIN_BACK_RIGHT, HIGH);
  
  // Calculating the distance
  distanceFront = durationFront*0.034/2;
  distanceFrontLeft = durationFrontLeft*0.034/2;
  distanceFrontRight = durationFrontRight*0.034/2;
  distanceBack = durationBack*0.034/2;
  distanceBackLeft = durationBackLeft*0.034/2;
  distanceBackRight = durationBackRight*0.034/2;
}
