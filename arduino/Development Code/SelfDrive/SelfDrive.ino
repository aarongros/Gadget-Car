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
const int TRIG_PIN_BACK_LEFT = 28;
const int ECHO_PIN_BACK_LEFT = 29;
const int TRIG_PIN_BACK_RIGHT = 30;
const int ECHO_PIN_BACK_RIGHT = 31;
#define STEER_INPUT A0

//Instantiating all the variables needed for the logic
int steer_pot = 0;
long durationFront;
int distanceFront;
long durationFrontLeft;
int distanceFrontLeft;
long durationFrontRight;
int distanceFrontRight;
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
      allOff();
    }
    else if(distanceFrontLeft < 20 && distanceFrontLeft > 5 && distanceFrontRight > 20) {
      analogWrite(RIGHT_PWM_DRIVE, 175);
      analogWrite(LEFT_PWM_DRIVE, 0);
      if(!(steer_pot < 1023 && steer_pot > 800)) {
        analogWrite(RIGHT_PWM_STEER, 255);
        analogWrite(LEFT_PWM_STEER, 0);
      }
      allOff();
      digitalWrite(RIGHT_LIGHT, HIGH);
    }
    else if(distanceFrontLeft > 20 && distanceFrontRight < 20 && distanceFrontRight > 5) {
      analogWrite(RIGHT_PWM_DRIVE, 175);
      analogWrite(LEFT_PWM_DRIVE, 0);
      if(!(steer_pot < 200 && steer_pot > 0)) {
        analogWrite(RIGHT_PWM_STEER, 0);
        analogWrite(LEFT_PWM_STEER, 255);
      }
      allOff();
      digitalWrite(LEFT_LIGHT, HIGH);
    }
    else if(distanceFrontLeft < 5 || distanceFrontRight < 5) {
      backup = true;
    }
  }
  if(backup)  {
    if((distanceFrontLeft - distanceFrontRight) >= 0 && distanceBackRight > 20 && distanceBackLeft > 5) {
      analogWrite(RIGHT_PWM_DRIVE, 0);
      analogWrite(LEFT_PWM_DRIVE, 175);
      if(!(steer_pot < 1023 && steer_pot > 800)) {
        analogWrite(RIGHT_PWM_STEER, 255);
        analogWrite(LEFT_PWM_STEER, 0);
      }
      allOff();
      digitalWrite(RIGHT_LIGHT, HIGH);
      digitalWrite(BRAKE_LIGHT, HIGH);
    }
    else if((distanceFrontRight - distanceFrontLeft) >= 0 && distanceBackLeft > 20 && distanceBackRight > 5)  {
      analogWrite(RIGHT_PWM_DRIVE, 0);
      analogWrite(LEFT_PWM_DRIVE, 175);
      if(!(steer_pot < 200 && steer_pot > 0)) {
        analogWrite(RIGHT_PWM_STEER, 0);
        analogWrite(LEFT_PWM_STEER, 255);
      }
      allOff();
      digitalWrite(LEFT_LIGHT, HIGH);
      digitalWrite(BRAKE_LIGHT, HIGH);
    }
  }
  else  {
    analogWrite(RIGHT_PWM_DRIVE, 0);
    analogWrite(LEFT_PWM_DRIVE, 0);
    allOff();
    digitalWrite(HAZARD_LIGHT, HIGH);
  }
}

void allOff() {
  digitalWrite(BRAKE_LIGHT, LOW);
  digitalWrite(LEFT_LIGHT, LOW);
  digitalWrite(RIGHT_LIGHT, LOW);
  digitalWrite(HAZARD_LIGHT, LOW);
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
  distanceBackLeft = durationBackLeft*0.034/2;
  distanceBackRight = durationBackRight*0.034/2;
}
