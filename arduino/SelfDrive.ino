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
int steer_pot = 0;
int right_pwm_steer_speed = 0;
int right_pwm_drive_speed = 0;
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
  
  digitalWrite(DRIVE_ENABLE, HIGH); //Set Drive enable to the motor driver for the back wheels will activate
}

void loop() {
  scanSurroundings();
  bool backup = false;
  steer_pot = analogRead(STEER_INPUT);
  if(distanceFront > 20)  {
    if(distanceFrontLeft > 20 && distanceFrontRight > 20)  {
      analogWrite(RIGHT_PWM_DRIVE, 175);
      analogWrite(LEFT_PWM_DRIVE, 0);
    }
    else if(distanceFrontLeft < 20 && distanceFrontLeft > 5 && distanceFrontRight > 20) {
      analogWrite(RIGHT_PWM_DRIVE, 175);
      analogWrite(LEFT_PWM_DRIVE, 0);
      if(!(steer_pot < 1023 && steer_pot > 800)) {
        analogWrite(RIGHT_PWM_STEER, 255);
        analogWrite(LEFT_PWM_STEER, 0);
      }
    }
    else if(distanceFrontLeft > 20 && distanceFrontRight < 20 && distanceFrontRight > 5) {
      analogWrite(RIGHT_PWM_DRIVE, 175);
      analogWrite(LEFT_PWM_DRIVE, 0);
      if(!(steer_pot < 200 && steer_pot > 0)) {
        analogWrite(RIGHT_PWM_STEER, 0);
        analogWrite(LEFT_PWM_STEER, 255);
      }
    }
    else if(distanceFrontLeft < 5 || distanceFrontRight < 5) {
      backup = true;
    }
  }
  if(backup && distanceBack > 20)  {
    if((distanceFrontLeft - distanceFrontRight) >= 0 && distanceBackRight > 20 && distanceBackLeft > 5) {
      analogWrite(RIGHT_PWM_DRIVE, 0);
      analogWrite(LEFT_PWM_DRIVE, 175);
      if(!(steer_pot < 1023 && steer_pot > 800)) {
        analogWrite(RIGHT_PWM_STEER, 255);
        analogWrite(LEFT_PWM_STEER, 0);
      }
    }
    else if((distanceFrontRight - distanceFrontLeft) >= 0 && distanceBackLeft > 20 && distanceBackRight > 5)  {
      analogWrite(RIGHT_PWM_DRIVE, 0);
      analogWrite(LEFT_PWM_DRIVE, 175);
      if(!(steer_pot < 200 && steer_pot > 0)) {
        analogWrite(RIGHT_PWM_STEER, 0);
        analogWrite(LEFT_PWM_STEER, 255);
      }
    }
  }
  else  {
    analogWrite(RIGHT_PWM_DRIVE, 0);
    analogWrite(LEFT_PWM_DRIVE, 0);
  }
}

void headLights()  {
  if(!headLightsOn && !z_button) {  //If headlights are not on and the z button is pressed we turn the headlights on
    digitalWrite(HEAD_LIGHT, HIGH);
    headLightsOn = !headLightsOn;
  }
  else if(headLightsOn && z_button) { //If headlights are on and the z button is not pressed we turn the headlights off
    digitalWrite(HEAD_LIGHT, LOW);
    headLightsOn = !headLightsOn;
  }
}

void hazards() {
  if(!hazardsOn && !c_button)  {  //If hazards are not on and the c button is pressed we turn the hazards on
    digitalWrite(HAZARD_LIGHT, HIGH);
    hazardsOn = !hazardsOn;
  }
  else if(hazardsOn && c_button) {  //If hazards are on and the c button is not pressed we turn the hazards off
    digitalWrite(HAZARD_LIGHT, LOW);
    hazardsOn = !hazardsOn;
  }
}

void rightLights() {
  if(!rightLightsOn && right_pwm_steer_speed > 50)  { //If the right signal is not on and the turn value is greater than 50 we turn the right blinker on
    digitalWrite(RIGHT_LIGHT, HIGH);
    rightLightsOn = !rightLightsOn;
  }
  else if(rightLightsOn && !(right_pwm_steer_speed > 50)) { //If the right signal is on and the turn value is not greater than 50 we turn the right blinker off
    digitalWrite(RIGHT_LIGHT, LOW);
    rightLightsOn = !rightLightsOn;
  }
}

void leftLights()  {
  if(!leftLightsOn && right_pwm_steer_speed < -50) {  //If the left signal is not on and the turn value is less than -50 we turn the left blinker on
    digitalWrite(LEFT_LIGHT, HIGH);
    leftLightsOn = !leftLightsOn;
  }
  else if(leftLightsOn && !(right_pwm_steer_speed < -50))  {  //If the left signal is on and the turn value is not less than -50 we turn the left blinker off
    digitalWrite(LEFT_LIGHT, LOW);
    leftLightsOn = !leftLightsOn;
  }
}

void brakeLight() {
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
  digitalWrite(TRIG_PIN_FRONT, LOW);  //Clears the TRIN_PIN
  delayMicroseconds(2); 
  digitalWrite(TRIG_PIN_FRONT, HIGH); //Sets the TRIG_PIN HIGH for 10 micro seconds
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_FRONT, LOW);
  durationFront = pulseIn(ECHO_PIN_FRONT, HIGH);  //Reads the ECHO_PIN, returns the sound wave travel time in microseconds

  digitalWrite(TRIG_PIN_FRONT_LEFT, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN_FRONT_LEFT, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_FRONT_LEFT, LOW);
  durationFrontLeft = pulseIn(ECHO_PIN_FRONT_LEFT, HIGH);

  digitalWrite(TRIG_PIN_FRONT_LEFT, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN_FRONT_RIGHT, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_FRONT_RIGHT, LOW);
  durationFrontRight = pulseIn(ECHO_PIN_FRONT_RIGHT, HIGH);

  digitalWrite(TRIG_PIN_BACK, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN_BACK, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_BACK, LOW);
  durationBack = pulseIn(ECHO_PIN_BACK, HIGH);
  
  digitalWrite(TRIG_PIN_BACK_LEFT, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN_BACK_LEFT, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_BACK_LEFT, LOW);
  durationBack = pulseIn(ECHO_PIN_BACK_LEFT, HIGH);
  
  digitalWrite(TRIG_PIN_BACK_RIGHT, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN_BACK_RIGHT, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_BACK_RIGHT, LOW);
  durationBack = pulseIn(ECHO_PIN_BACK_RIGHT, HIGH);
  
  // Calculating the distance
  distanceFront = durationFront*0.034/2;
  distanceFrontLeft = durationFrontLeft*0.034/2;
  distanceFrontRight = durationFrontRight*0.034/2;
  distanceBack = durationBack*0.034/2;
  distanceBackLeft = durationBackLeft*0.034/2;
  distanceBackRight = durationBackRight*0.034/2;
}
