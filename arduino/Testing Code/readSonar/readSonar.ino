const int TRIG_PIN_FRONT = 13;
const int ECHO_PIN_FRONT = 12;
const int TRIG_PIN_FRONT_LEFT = 11;
const int ECHO_PIN_FRONT_LEFT = 10;
const int TRIG_PIN_FRONT_RIGHT = 9;
const int ECHO_PIN_FRONT_RIGHT = 8;
const int TRIG_PIN_BACK = 7;
const int ECHO_PIN_BACK = 6;
long durationFront;
int distanceFront;
long durationFrontLeft;
int distanceFrontLeft;
long durationFrontRight;
int distanceFrontRight;
long durationBack;
int distanceBack;
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(TRIG_PIN_FRONT, OUTPUT);
  pinMode(ECHO_PIN_FRONT, INPUT);
  pinMode(TRIG_PIN_FRONT_LEFT, OUTPUT);
  pinMode(ECHO_PIN_FRONT_LEFT, INPUT);
  pinMode(TRIG_PIN_FRONT_RIGHT, OUTPUT);
  pinMode(ECHO_PIN_FRONT_RIGHT, INPUT);
  pinMode(TRIG_PIN_BACK, OUTPUT);
  pinMode(ECHO_PIN_BACK, INPUT);
   Serial.print("Ready");
}

void loop() {
  digitalWrite(TRIG_PIN_FRONT, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN_FRONT, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_FRONT, LOW);
  durationFront = pulseIn(ECHO_PIN_FRONT, HIGH);

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
  
  // Calculating the distance
  distanceFront = durationFront*0.034/2;
  distanceFrontLeft = durationFrontLeft*0.034/2;
  distanceFrontRight = durationFrontRight*0.034/2;
  distanceBack = durationBack*0.034/2;

  Serial.print(distanceFront);
  Serial.print("\t");
  Serial.print(distanceFrontLeft);
  Serial.print("\t");
  Serial.print(distanceFrontRight);
  Serial.print("\t");
  Serial.println(distanceBack);

}
