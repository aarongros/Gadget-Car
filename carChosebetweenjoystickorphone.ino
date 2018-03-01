bool connectedToPhone = false;
void setup()  {
  Serial.begin(9600);
  Serial.print("AT+");
  if(Serial.read()
    connectedToPhone = true;
}
void loop() {
  if(connectedToPhone)  {
  
  }
  else
}
