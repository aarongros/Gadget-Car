#include <Wire.h>
#define nunchuk_ID 0x52
unsigned char buffer[6];
int count = 0;
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
unsigned char joy_x_axis;
unsigned char joy_y_axis;
unsigned char z_button;
unsigned char c_button;
int accel_x_axis;

void setup () {
	Serial.begin (9600);
	Wire.begin ();
	Wire.beginTransmission (nunchuk_ID);
	Wire.send (0x40);
	Wire.send (0x00);
	Wire.endTransmission ();
	delay (100);
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

void loop ()	{
	Wire.requestFrom (nunchuk_ID, 6);
	while (Wire.available ())	{
		buffer[count] = nunchuk_decode_byte (Wire.receive ());
		count++;
	}
	if (count >= 5)	{
		updateJoy ();
	}
	count = 0;
	send_zero ();
	delay (5);
	
	checkLights();
	checkDrive();
	checkTurn();
}

void send_zero ()	{
	Wire.beginTransmission (nunchuk_ID);
	Wire.send (0x00);
	Wire.endTransmission ();
}

void updateJoy ()	{
	joy_x_axis = buffer[0];
	joy_y_axis = buffer[1];
	accel_x_axis = (buffer[2]) << 2;
	if ((buffer[5] & 0x01)!=0)
	{ z_button = 1; }
	else
	{ z_button = 0; }
	if ((buffer[5] & 0x02)!=0)
	{ c_button = 1; }
	else
	{ c_button = 0; }
	accel_x_axis += ((buffer[5]) >> 2) & 0x03;
}

char nunchuk_decode_byte (char x)	{
	x = (x ^ 0x17) + 0x17;
	return x;
}

void checkDrive()	{
	right_pwn_drive_speed = joy_y_axis - 255 / 2;
	analogWrite(RIGHT_PWM_DRIVE, right_pwn_drive_speed);
	analogWrite(LEFT_PWM_DRIVE, 0);
}

void checkTurn()	{
	steer_pot = analogRead(STEER_INPUT);
	right_pwn_steer_speed = joy_x_axis - 255 / 2;
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
	if(!headLightsOn && c_button == 0)	{
		digitalWrite(HEAD_LIGHT, HIGH);
		headLightsdOn = !headLightsdOn;
	}
	else if(headLightsdOn && c_button == 1)	{
		digitalWrite(HEAD_LIGHT, LOW);
		headLightsdOn = !headLightsdOn;
	}
}

void checkHazards()	{
	if(!hazardsOn && z_button == 0)	{
		digitalWrite(HAZARD_LIGHT, HIGH);
		hazardsOn = !hazardsOn;
	}
	else if(hazardsOn && z_button == 1)	{
		digitalWrite(HAZARD_LIGHT, LOW);
		hazardsOn = !hazardsOn;
	}
}

void checkRightLights()	{
	if(!rightLightsOn && accel_x_axis > 800)	{
		digitalWrite(RIGHT_LIGHT, HIGH);
		rightLightsOn = !rightLightsOn;
	}
	else if(rightLightsOn && !digitalRead(RIGHT_LIGHT))	{
		digitalWrite(RIGHT_LIGHT, LOW);
		rightLightsOn = !rightLightsOn;
	}
}

void checkLeftLights()	{
	if(!leftLightsOn && accel_x_axis < 400)	{
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
