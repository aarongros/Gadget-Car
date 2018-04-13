static int r_pwm_drive = 13;
static int l_pwm_drive = 12;
static int driveEnable = 9;

void setup() {
  Serial.begin(9600);
  pinMode(r_pwm_drive, OUTPUT);
  pinMode(l_pwm_drive, OUTPUT);
  pinMode(driveEnable, OUTPUT);
}

void loop() {
  analogWrite(r_pwm_drive, r_pwm_drive_speed);
  analogWrite(l_pwm_drive, l_pwm_drive_speed);
}
