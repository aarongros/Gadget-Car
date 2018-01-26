//Needs to send signals to arduino micro vie digital pings 3-7 to tell it which lights to turn on and off
//needs to recieve bluetooth data from the joystick and the app
//needs to use that data to steer and change velocity
const int RIGHT_LIGHT = 3;
const int LEFT_LIGHT = 4;
const int BRAKE_LIGHT = 5;
const int HAZARD_LIGHT = 6;
const int HEAD_LIGHT = 7;
const int RIGHT_PWM_DRIVE = 13;
const int LEFT_PWM_DRIVE = 12;
const int RIGHT_PWM_STEER = 10;
const int LEFT_PWM_STEER = 9;
#define STEER_INPUT A0;
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

void Setup()	{
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

void Loop()	{
	checkLights();
	checkDrive();
	checkTurn();
}

void checkDrive()	{
	right_pwn_drive_speed = y_value * 255 / 1023;
	analogWrite(RIGHT_PWM_DRIVE, right_pwn_drive_speed);
	analogWrite(LEFT_PWM_DRIVE, 0);
}

void checkTurn()	{
	steer_pot = analogRead(STEER_INPUT);
	right_pwn_steer_speed = x_value * 255 / 1023;
	if(!((steet_pot >= 950 && right_pwn_steer_speed > 0) || (steer_pot <= 75 && right_pwn_steer_speed < 0))	{
		analogWrite(RIGHT_PWM_STEER, right_pwn_steer_speed);
		analogWrite(LEFT_PWM_STEER, 0);
	}
}

void checkLights()	{
	checkHazards();
	checkRightLights();
	checkLeftLights();
	checkBrakeLights();
	checkHeadLights();
}

void checkHeadLights()	{
	if(!headLightsOn && digitalRead(HEAD_LIGHT))	{
		digitalWrite(HEAD_LIGHT, HIGH);
		headLightsdOn = !headLightsdOn;
	}
	else if(headLightsdOn && !digitalRead(HEAD_LIGHT))	{
		digitalWrite(HEAD_LIGHT, LOW);
		headLightsdOn = !headLightsdOn;
	}
}

void checkHazards()	{
	if(!hazardsOn && digitalRead(HAZARD_LIGHT))	{
		digitalWrite(HAZARD_LIGHT, HIGH);
		hazardsOn = !hazardsOn;
	}
	else if(hazardsOn && !digitalRead(HAZARD_LIGHT))	{
		digitalWrite(HAZARD_LIGHT, LOW);
		hazardsOn = !hazardsOn;
	}
}

void checkRightLights()	{
	if(!rightLightsOn && digitalRead(RIGHT_LIGHT))	{
		digitalWrite(RIGHT_LIGHT, HIGH);
		rightLightsOn = !rightLightsOn;
	}
	else if(rightLightsOn && !digitalRead(RIGHT_LIGHT))	{
		digitalWrite(RIGHT_LIGHT, LOW);
		rightLightsOn = !rightLightsOn;
	}
}

void checkLeftLights()	{
	if(!leftLightsOn && digitalRead(LEFT_LIGHT))	{
		digitalWrite(LEFT_LIGHT, HIGH);
		leftLightsOn = !leftLightsOn;
	}
	else if(leftLightsOn && !digitalRead(LEFT_LIGHT))	{
		digitalWrite(LEFT_LIGHT, LOW);
		leftLightsOn = !leftLightsOn;
	}
}

void checkBrakeLights()	{
	if(!brakeLightsOn && digitalRead(BRAKE_LIGHT))	{
		digitalWrite(BRAKE_LIGHT, HIGH);
		brakeLightsOn = !brakeLightsOn;
	}
	else if(brakeLightsdOn && !digitalRead(BRAKE_LIGHT))	{
		digitalWrite(BRAKE_LIGHT, LOW);
		brakeLightsOn = !brakeLightsOn;
	}
}

