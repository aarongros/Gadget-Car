//Needs to send signals to arduino micro vie digital pings 3-7 to tell it which lights to turn on and off
//needs to recieve bluetooth data from the joystick and the app
//needs to use that data to steer and change velocity.
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
bool c_button = false;
bool z_button = false;
int accel_x = 0;
int accel_y = 0;
int accel_z = 0;
int steer_pot = 0;
bool headLightsOn = false;
bool brakeLightsOn = false;
bool leftLightsOn = false;
bool rightLightsOn = false;
bool hazardsOn = false;

5 outputs for light
2 output for steer
2 output for drive
1 input for steer pot


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
	analogRead();
	analogWrite();
}

void checkTurn()	{
	steer_pot = analogRead();
	if(!((steet_pot >= 950 && drive > 0) || (steer_pot <= 75 && drive < 0))	{
		analogWrite();
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
	if(!headLightsOn	&&	digitalRead())	{
		digitalWrite(,HIGH);
	}
	else if(headLightsdOn && !digitalRead())	{
		digitalWrite(,LOW);
	}
}

void checkHazards()	{
	if(!hazardsOn	&&	digitalRead())	{
		digitalWrite(,HIGH);
	}
	else if(hazardsOn && !digitalRead())	{
		digitalWrite(,LOW);
	}
}

void checkRightLights()	{
	if(!rightLightsOn	&&	digitalRead())	{
		digitalWrite(,HIGH);
	}
	else if(rightLightsOn && !digitalRead())	{
		digitalWrite(,LOW);
	}
}

void checkLeftLights()	{
	if(!leftLightsOn	&&	digitalRead())	{
		digitalWrite(,HIGH);
	}
	else if(leftLightsOn && !digitalRead())	{
		digitalWrite(,LOW);
	}
}

void checkBrakeLights()	{
	if(!brakeLightsOn	&&	digitalRead())	{
		digitalWrite(,HIGH);
	}
	else if(brakeLightsdOn && !digitalRead())	{
		digitalWrite(,LOW);
	}
}

