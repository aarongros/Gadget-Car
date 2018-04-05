#include <SoftwareServo.h> 
int potentiometer=A1;
int button1=3;
int button2=4;
int potval;
int curval;
SoftwareServo ESC;


void setup() {
  
  pinMode(potentiometer, INPUT);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  ESC.attach(1);
  curval=0;
  ESC.setMinimumPulse(800);
  ESC.setMaximumPulse(2000);
}

void loop() {
  
  while (digitalRead(button1)==HIGH){
  potval=analogRead(potentiometer);
  potval=map(potval,0,1023,0,180);
  
  while(curval<potval){
    potval=analogRead(potentiometer);
    potval=map(potval,0,1023,0,180);
    curval=curval+1;
    ESC.write(curval);
    SoftwareServo::refresh();
    delay(50);
    while(digitalRead(button2)==HIGH){
      ESC.write(curval);
      SoftwareServo::refresh();}}

  while(curval>potval){
    potval=analogRead(potentiometer);
    potval=map(potval,0,1023,0,180);
    curval=curval-1;
    ESC.write(curval);
    SoftwareServo::refresh();
    delay(50);
    while(digitalRead(button2)==HIGH){
      ESC.write(curval);
      SoftwareServo::refresh();}}
      
  ESC.write(curval);
  SoftwareServo::refresh();}

  potval=0; 
  while(curval>potval){
      curval=curval-1;
      ESC.write(curval);
      SoftwareServo::refresh();
      delay(50);}

  ESC.write(curval);
  SoftwareServo::refresh();}
    

  




