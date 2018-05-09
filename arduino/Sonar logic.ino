int f1;
int f2;
int f3;
int b1;
int b2;
int b3;

if(f2 > 20)  {
  if(f1 > 20 && f3 > 20)  {
    Straight
  }
  else if(f1 < 20 && f1 > 5 && f3 > 20) {
    turn right
  }
  else if(f1 > 20 && f3 < 20 && f3 > 5) {
    turn left
  }
  else if(f1 < 5 || f3 < 5) {
    backup true
  }
}
if(backup && b2 > 20)  {
  if((f1 - f3) >= 0 && b3 > 20 && b1 > 5) {
    backup right
  }
  else if((f3 - f1) >= 0 && b1 > 20 && b3 > 5)  {
    backup left
  }
}
else  {
  stop
}
