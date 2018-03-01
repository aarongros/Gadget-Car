const int LEFT_LIGHTS[3] = {3,4,5};
const int RIGHT_LIGHTS[3] = {6,7,8};
const int NUM_LIGHTS = sizeof(LEFT_LIGHTS) / sizeof(int);
const int HEADLIGHTS = 9;
const int LEFT_INPUT = 10;
const int RIGHT_INPUT = 16;
const int HAZARDS_INPUT = 14;
const int BREAKING_INPUT = 15;
const int HEADLIGHTS_INPUT = 2;
const int WAIT_TIME = 100;
boolean leftAndBraking = false;
boolean rightAndBraking = false;
boolean brakingOn = false;
boolean headlightsOn = false;

// Setup the headlights, back lights, and all the inputs describing the state of each function
void setup()  {
  for(int s = 0; s < NUM_LIGHTS; s++)  {
    pinMode(LEFT_LIGHTS[s], OUTPUT);
    pinMode(RIGHT_LIGHTS[s], OUTPUT);
  }
  pinMode(HEADLIGHTS, OUTPUT);
  pinMode(LEFT_INPUT, INPUT);
  pinMode(RIGHT_INPUT, INPUT);
  pinMode(HAZARDS_INPUT, INPUT);
  pinMode(BREAKING_INPUT, INPUT);
  pinMode(HEADLIGHTS_INPUT, INPUT);
}

// Loops cheching if any on the functions should run
void loop() {
  checkBraking();
  checkTurning();
  checkTurningAndBraking();
  checkHazards();
  checkHeadLights();
}

// Checks if headlights should change state and changes it if necessary
void checkHeadLights()  {
  if(digitalRead(HEADLIGHTS_INPUT).equals(HIGH) && !headlightsOn)  {
    digitalWrite(HEADLIGHTS,HIGH);
    headlightsOn = !headlightsOn;
  }
  else if(digitalRead(HEADLIGHTS_INPUT).equals(LOW) && headlightsOn) {
    digitalWrite(HEADLIGHTS,LOW);
    headlightsOn = !headlightsOn;
  }
}

// Checks if the hazards should change state and changes it if necessary
void checkHazards() {
  if(digitalRead(HAZARDS_INPUT).equals(HIGH)) {
    hazardLights();
  }
}

// Checks if the light signals should change state and changes it if necessary
void checkTurning() {
  if(digitalRead(LEFT_INPUT))  {
      turnLeftBlincker();
      if(brakingOn) {
       leftAndBraking = true;     
      }
  }
  if(digitalRead(RIGHT_INPUT))  {
      turnRightBlincker();
      if(brakingOn) {
       rightAndBraking = true;     
      }
  }
}

// Checks if the brake lights should change state and changes it if necessary
void checkBraking() {
  if(digitalRead(BREAKING_INPUT).equals(HIGH) && !brakingOn)  {
    brakeLights();
    brakingOn = !brakingOn;
  }
  else if(digitalRead(BREAKING_INPUT).equals(LOW) && brakingOn) {
    allOff();
    brakingOn = !brakingOn;
  }
}

// Makes sure there are no errors when turning and braking
checkTurningAndBraking()  {
  if(digitalRead(LEFT_INPUT).equals(LOW) && leftAndBraking)  {
    for(int tb = 0; tb < NUM_LIGHTS; tb++)  {
      digitalWrite(LEFT_LIGHTS[tb], HIGH);
      delay(WAIT_TIME);
    }
    leftAndBraking = false;     
  } 
  if(digitalRead(RIGHT_INPUT).equals(LOW) && rightAndBraking)  {
    for(int rb = 0; rb < NUM_LIGHTS; rb++)  {
      digitalWrite(RIGHT_LIGHTS[rb], HIGH);
      delay(WAIT_TIME);
    }
    rightAndBraking = false;
  }
}

// Turns on the left blincker
void turnLeftBlincker()  {
  for(int lt = 0; lt < NUM_LIGHTS; lt++)  {
    digitalWrite(LEFT_LIGHTS[lt], HIGH);
    delay(WAIT_TIME);
  }
  leftOff();
  delay(WAIT_TIME);
}

// Turns on the right blincker
void turnRightBlincker()  {
  for(int rt = 0; rt < NUM_LIGHTS; rt++)  {
    digitalWrite(RIGHT_LIGHTS[rt], HIGH);
    delay(WAIT_TIME);
  }
  rightOff();
  delay(WAIT_TIME);
}

// Turns on the hazard lights
void hazardLights()  {
  brakeLights();
  allOff();
  delay(WAIT_TIME);
}

// Turns on the brake lights
void brakeLights() {
  for(int b = 0; b < NUM_LIGHTS; b++)  {
    digitalWrite(LEFT_LIGHTS[b], HIGH);
    digitalWrite(RIGHT_LIGHTS[b], HIGH);
    delay(WAIT_TIME);
  }
}

// Turns off all the back lights
void allOff()  {
  for(int a = 0; a < NUM_LIGHTS; a++)  {
    digitalWrite(LEFT_LIGHTS[a], LOW);
    digitalWrite(RIGHT_LIGHTS[a], LOW);
  }
}

// Turns off all the back left lights
void leftOff()  {
  for(int l = 0; l < NUM_LIGHTS; l++)  {
    digitalWrite(LEFT_LIGHTS[l], LOW);
  }
}

// Turns off all the back right lights
void rightOff()  {
  for(int r = 0; r < NUM_LIGHTS; r++)  {
    digitalWrite(RIGHT_LIGHTS[r], LOW);
  }
}
