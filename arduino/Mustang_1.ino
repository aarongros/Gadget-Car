int steeringPinLeft = 9;    // forward PWM pin
int steeringPinRight = 10;   // reverse PWM pin
int motorPinF = 12;
int motorPinR = 13;
int motorEnable = 8;
int count = 0;
int steeringMotor = A0;
int steeringControl = A2;
int motorControl = A4;
int pos;
int spd = 0;
int desired = 650;
boolean turning = false;
//int turnDir = 0;
//int moveDir = 0;
String command = "z";
//ri = b
//le = c
//f = d
//r = e
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(steeringPinLeft, OUTPUT);
  pinMode(steeringPinRight, OUTPUT);
  pinMode(motorPinF, OUTPUT);
  pinMode(motorPinR, OUTPUT);
  pinMode(steeringMotor, INPUT);
  pinMode(steeringControl,INPUT);
  pinMode(motorControl, INPUT);
  pinMode(motorEnable, OUTPUT);
  digitalWrite(motorEnable,LOW);
  delay(500);
  digitalWrite(motorEnable,HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()) {
    command = Serial.readString();
    command = command.substring(command.length()-1);
    Serial.println(command);
    if (command.equals("D")) {
      desired += 100;
    } else if(command.equals("A")) {
      desired -= 100;
    }
    if (command.equals("W")) {
      spd += 30;
    } else if(command.equals("S")) {
      spd -= 30;
    }
    if(command.equals(" ")) {
      spd = 0;
      //turnDir = 0;
      desired = 650;
    }
  }
  delay(1);
  //desired += turnDir * 1;
  int actual = analogRead(steeringMotor);
  //Serial.print(actual);
  //Serial.print("   ");
  //Serial.println(desired);
  if(desired <= 350) desired = 350; 
  if(desired >= 950) desired = 950; 
  if(actual >= desired - 5 && actual <= desired + 5) {
    analogWrite(steeringPinLeft,0);
    analogWrite(steeringPinRight,0);
    //Serial.print("Desired: ");
    //Serial.print(desired);
    //Serial.print(", Actual: ");
    //Serial.println(actual);
  }else if(actual < desired - 5) {
    analogWrite(steeringPinLeft,0);
    analogWrite(steeringPinRight,150);
  }else if(actual > desired + 5) {
    analogWrite(steeringPinLeft,150);
    analogWrite(steeringPinRight,0);
  }

  if(spd >= 240) {
    spd = 240;
  } else if(spd <= -240) {
    spd = -240;
  } else if(spd >= 15) {
    analogWrite(motorPinF, spd);
    analogWrite(motorPinR, 0);
  } else if(spd <= -15) {
    analogWrite(motorPinR, spd*(-1));
    analogWrite(motorPinF, 0);
  } else {
    analogWrite(motorPinR, 0);
    analogWrite(motorPinF, 0);
  }
  
//  int vel = analogRead(motorControl);
//  if(vel >= 512) {
//    analogWrite(motorPinF, (vel-512)/2);
//    analogWrite(motorPinR, 0);
//  } else {
//    analogWrite(motorPinR, (512-vel)/2);
//    analogWrite(motorPinF, 0);
//  }
  
}
