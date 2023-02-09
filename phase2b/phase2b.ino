void forward_motion(int rightwheel1, int rightwheel2, int leftwheel1, int leftwheel2, int analogright, int analogleft, int period);
void backward_motion(int rightwheel1, int rightwheel2, int leftwheel1, int leftwheel2, int analogright, int analogleft, int period);
void pivotright(int rightwheel1, int rightwheel2, int leftwheel1, int leftwheel2, int analogright, int analogleft, int period);
void pivotleft(int rightwheel1, int rightwheel2, int leftwheel1, int leftwheel2, int analogright, int analogleft, int period);
void turnleft(int rightwheel1, int rightwheel2, int leftwheel1, int leftwheel2, int analogright, int analogleft, int period);
void turnright(int rightwheel1, int rightwheel2, int leftwheel1, int leftwheel2, int analogright, int analogleft, int period);

int pivot_period = 1000;
int turn_period = 1000;
int power = 0;

//motor pins
int pin9 = 9;
int pin10 = 10;
int pin5 = 5;
int pin6 = 6;

//enable pins
int pin13 = 13;
int pin7 = 7;

void setup() {
  pinMode(pin13, OUTPUT);
  pinMode(pin9, OUTPUT);
  pinMode(pin10, OUTPUT);
  pinMode(pin5, OUTPUT);
  pinMode(pin6, OUTPUT);
  pinMode(pin7, OUTPUT);

}

void loop() {
  delay(2000);
  
  digitalWrite(pin13, HIGH); //enable1 pin
  digitalWrite(pin7, HIGH); //enable2 pin

  forward_motion(pin6, pin5, pin10, pin9, 255, 255, 1500);
  delay(300);
  pivot_period = 580; //180deg turn
  pivotright(pin6, pin5, pin10, pin9, 255, 255, pivot_period);
  delay(2000);

  turn_period = 950;
  turnright(pin6, pin5, pin10, pin9, 255, 255, turn_period);
  delay(2000);

  turnleft(pin6, pin5, pin10, pin9, 255, 255, turn_period);
  delay(2000);

}


void backward_motion(int rightwheel1, int rightwheel2, int leftwheel1, int leftwheel2, int analogright, int analogleft, int period){
  analogWrite(rightwheel1, 0);
  analogWrite(rightwheel2, analogright);
  analogWrite(leftwheel1, 0);
  analogWrite(leftwheel2, analogleft);
  delay(period);
  analogWrite(rightwheel1, 0);
  analogWrite(rightwheel2, 0);
  analogWrite(leftwheel1, 0);
  analogWrite(leftwheel2, 0);
}

void forward_motion(int rightwheel1, int rightwheel2, int leftwheel1, int leftwheel2, int analogright, int analogleft, int period){
  analogWrite(rightwheel1, analogright);
  analogWrite(rightwheel2, 0);
  analogWrite(leftwheel1, analogleft);
  analogWrite(leftwheel2, 0);
  delay(period);
  analogWrite(rightwheel1, 0);
  analogWrite(rightwheel2, 0);
  analogWrite(leftwheel1, 0);
  analogWrite(leftwheel2, 0);
}

void pivotright(int rightwheel1, int rightwheel2, int leftwheel1, int leftwheel2, int analogright, int analogleft, int period){
  analogWrite(rightwheel1, analogright);
  analogWrite(rightwheel2, 0);
  analogWrite(leftwheel1, 0);
  analogWrite(leftwheel2, analogleft);
  delay(period);
  analogWrite(rightwheel1, 0);
  analogWrite(rightwheel2, 0);
  analogWrite(leftwheel1, 0);
  analogWrite(leftwheel2, 0);
}

void pivotleft(int rightwheel1, int rightwheel2, int leftwheel1, int leftwheel2, int analogright, int analogleft, int period){
  analogWrite(rightwheel1, 0);
  analogWrite(rightwheel2, analogright);
  analogWrite(leftwheel1, analogleft);
  analogWrite(leftwheel2, 0);
  delay(period);
  analogWrite(rightwheel1, 0);
  analogWrite(rightwheel2, 0);
  analogWrite(leftwheel1, 0);
  analogWrite(leftwheel2, 0); 
}

void turnleft(int rightwheel1, int rightwheel2, int leftwheel1, int leftwheel2, int analogright, int analogleft, int period){
  analogWrite(rightwheel1, analogright);
  analogWrite(rightwheel2, 0);
  analogWrite(leftwheel1, analogleft/4);
  analogWrite(leftwheel2, 0);
  delay(period);
  analogWrite(rightwheel1, 0);
  analogWrite(rightwheel2, 0);
  analogWrite(leftwheel1, 0);
  analogWrite(leftwheel2, 0);
  
}


void turnright(int rightwheel1, int rightwheel2, int leftwheel1, int leftwheel2, int analogright, int analogleft, int period){
  analogWrite(rightwheel1, analogright/4);
  analogWrite(rightwheel2, 0);
  analogWrite(leftwheel1, analogleft);
  analogWrite(leftwheel2, 0);
  delay(period);
  analogWrite(rightwheel1, 0);
  analogWrite(rightwheel2, 0);
  analogWrite(leftwheel1, 0);
  analogWrite(leftwheel2, 0);
  
}