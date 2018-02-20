//Needs to send signals to arduino micro vie digital pings 3-7 to tell it which lights to turn on and off
//needs to recieve bluetooth data from the joystick and the app
//needs to use that data to steer and change velocity

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

void Loop()	{
	if (ET.receiveData()) {
		x_value = mydata.joy_x;
  		y_value = mydata.joy_y;
    		c_button = mydata.z_button;
    		c_button = mydata.c_button;
    		accel_x = mydata.accel_x;
  	}
	checkLights();
	checkDrive();
	checkTurn();
	delay(50);
}

void checkDrive()	{
	right_pwn_drive_speed = map(y_value, 0, 255, -255, 255);
	analogWrite(RIGHT_PWM_DRIVE, right_pwn_drive_speed);
	analogWrite(LEFT_PWM_DRIVE, 0);
	Serial.print("Steer: " + right_pwm_drive_speed);
}

void checkTurn()	{
	steer_pot = analogRead(STEER_INPUT);
	right_pwn_steer_speed = map(x_value, 0, 255, -255, 255);
	if(!((steet_pot >= 950 && right_pwn_steer_speed > 0) || (steer_pot <= 75 && right_pwn_steer_speed < 0))	{
		analogWrite(RIGHT_PWM_STEER, right_pwn_steer_speed);
		analogWrite(LEFT_PWM_STEER, 0);
		Serial.print("Steer: " + right_pwm_steer_speed);
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
	if(!headLightsOn && z_button)	{
		digitalWrite(HEAD_LIGHT, HIGH);
		headLightsdOn = !headLightsdOn;
		Serial.print("Started Head Lights");
	}
	else if(headLightsdOn && !z_button)	{
		digitalWrite(HEAD_LIGHT, LOW);
		headLightsdOn = !headLightsdOn;
		Serial.print("Stopped Head Lights");
	}
}

void checkHazards()	{
	if(!hazardsOn && c_button)	{
		digitalWrite(HAZARD_LIGHT, HIGH);
		hazardsOn = !hazardsOn;
		Serial.print("Started Hazard Lights");
	}
	else if(hazardsOn && !c_button)	{
		digitalWrite(HAZARD_LIGHT, LOW);
		hazardsOn = !hazardsOn;
		Serial.print("Stopped Hazard Lights");
	}
}

void checkRightLights()	{
	if(!rightLightsOn && right_pwm_steer_speed > 50)	{
		digitalWrite(RIGHT_LIGHT, HIGH);
		rightLightsOn = !rightLightsOn;
		Serial.print("Started Right Lights");
	}
	else if(rightLightsOn && !(right_pwm_steer_speed > 50))	{
		digitalWrite(RIGHT_LIGHT, LOW);
		rightLightsOn = !rightLightsOn;
		Serial.print("Stoped Right Lights");
	}
}

void checkLeftLights()	{
	if(!leftLightsOn && right_pwm_steer_speed < 50)	{
		digitalWrite(LEFT_LIGHT, HIGH);
		leftLightsOn = !leftLightsOn;
		Serial.print("Started Left Lights");
	}
	else if(leftLightsOn && !(right_pwm_steer_speed < 50))	{
		digitalWrite(LEFT_LIGHT, LOW);
		leftLightsOn = !leftLightsOn;
		Serial.print("Stoped Left Lights");
	}
}

void checkBrakeLights()	{
	if(!brakeLightsOn && right_pwm_drive_speed < 50)	{
		digitalWrite(BRAKE_LIGHT, HIGH);
		brakeLightsOn = !brakeLightsOn;
		Serial.print("Started Brake Lights");
	}
	else if(brakeLightsdOn && !(right_pwm_drive_speed < 50))	{
		digitalWrite(BRAKE_LIGHT, LOW);
		brakeLightsOn = !brakeLightsOn;
		Serial.print("Stoped Brake Lights");
	}
}
