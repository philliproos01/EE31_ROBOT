int pin3 = 3; //power white light

int value = 0;

// motor declarations
int pivot_period = 1000;
int turn_period = 1000;
int power = 80;

// motor pins
int pin9 = 9;
int pin10 = 10;
int pin5 = 5;
int pin6 = 6;

// enable pins
int pin13 = 13;
int pin7 = 7;

enum State {YELLOW, RED, BLUE, DARK};
State color = DARK;

void setup() {
  Serial.begin(9600);
  pinMode(pin3, OUTPUT);
  digitalWrite(pin3, HIGH); // power white LED

  // motor setup
  pinMode(pin13, OUTPUT);
  pinMode(pin9, OUTPUT);
  pinMode(pin10, OUTPUT);
  pinMode(pin5, OUTPUT);
  pinMode(pin6, OUTPUT);
  pinMode(pin7, OUTPUT);

  digitalWrite(pin13, HIGH); // enable1 pin
  digitalWrite(pin7, HIGH); // enable2 pin
  delay(1000);
}

void loop() {
  value = analogRead(A1) ;
  // change_state();
  line_tracker(BLUE);
}

void line_tracker(State COLOR) {
  int power = 50;
  turn_period = 20;
  while(true) {
    change_state();
    while (color == COLOR) {
      turnright(pin5, pin6, pin10, pin9, power, power, turn_period);
      change_state();
    }
    
    while (color != COLOR) {
      turnleft(pin5, pin6, pin10, pin9, power, power, turn_period);
      change_state();
    }

    while (color == COLOR) {
      turnleft(pin5, pin6, pin10, pin9, power, power, turn_period);
      change_state();
    }

    while (color != COLOR) {
      turnright(pin5, pin6, pin10, pin9, power, power, turn_period);
      change_state();
    }
  }
}

void change_state() {
  value = analogRead(A1) ;
  if((value < 500)) {
    color = DARK;
    Serial.println("d");
  } else if((500 < value) and (value < 540)) {
    color = BLUE;
    Serial.println("b");
  } else if((540 < value) and (value < 650)) {
    color = RED;
    Serial.println("r");
  } else if(650 < value) {
    color = YELLOW;
    Serial.println("y");
  }
}

// motor control functions
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

void turnright(int rightwheel1, int rightwheel2, int leftwheel1, int leftwheel2, int analogright, int analogleft, int period){
  Serial.println("inside turning func");
  analogWrite(rightwheel1, analogright);
  analogWrite(rightwheel2, 0);
  analogWrite(leftwheel1, analogleft/2);
  analogWrite(leftwheel2, 0);
  delay(period);
  analogWrite(rightwheel1, 0);
  analogWrite(rightwheel2, 0);
  analogWrite(leftwheel1, 0);
  analogWrite(leftwheel2, 0);
  
}


void turnleft(int rightwheel1, int rightwheel2, int leftwheel1, int leftwheel2, int analogright, int analogleft, int period){
  analogWrite(rightwheel1, analogright/2);
  analogWrite(rightwheel2, 0);
  analogWrite(leftwheel1, analogleft);
  analogWrite(leftwheel2, 0);
  delay(period);
  analogWrite(rightwheel1, 0);
  analogWrite(rightwheel2, 0);
  analogWrite(leftwheel1, 0);
  analogWrite(leftwheel2, 0);
  
}
