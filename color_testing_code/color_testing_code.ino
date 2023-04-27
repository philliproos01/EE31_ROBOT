enum State {YELLOW, RED, BLUE, DARK};
State color = DARK;
State prev = DARK;

bool tracking_bool = true;
bool collision_absence = true;

int pin2 = 2; //horn
int pin4 = 4;
int pin8 = 8;
int pin3 = 3; //power white light
int pin12 = 12; //brake lights
int ambient_light_analog = 0;
int value = 0;

int val1 = 0;

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

int counter = 0;

int value1 = 0;
int value2 = 0;
int value3 = 0;
int value4 = 0;


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
  pinMode(pin8, OUTPUT); //ambient light sensor
  pinMode(pin4, OUTPUT); //headlights
  digitalWrite(pin13, HIGH); // enable1 pin
  digitalWrite(pin7, HIGH); // enable2 pin
  delay(1000);

  digitalWrite(pin4, HIGH); //headlights
  pinMode(pin12, OUTPUT); //brakes
  //digitalWrite(pin12, HIGH); // power brakes test

  pinMode(pin2, OUTPUT);//horn

  //horn(pin2);
  delay(1000);
}

void loop() {
  
  
//  Serial.print("counter = ");
//  Serial.println(counter);
  if(counter == 0){
    State what_to_track = RED;
    line_tracker(what_to_track);
  }

  if(counter == 1){
    delay(100);
    pivotleft(pin5, pin6, pin10, pin9, power, power, 600);
    counter += 1;
    loop();
//    forward_motion(pin5, pin6, pin10, pin9, power, power, 5000);
//    backward_motion(pin5, pin6, pin10, pin9, power, power, 5000);
//    turnright(pin5, pin6, pin10, pin9, power, power, turn_period);
//    turnleft(pin5, pin6, pin10, pin9, power, power, turn_period);
//    pivotright(pin5, pin6, pin10, pin9, power, power, 500);
//    pivotleft(pin5, pin6, pin10, pin9, power, power, 500);
  }

  if(counter == 2){
    delay(100);
    State what_to_avoid = YELLOW;
    color_avoider(what_to_avoid);
    digitalWrite(pin12, HIGH);
    delay(800);
    digitalWrite(pin12, LOW);
    counter += 1;
    loop();
  }

  if(counter == 3){
    delay(100);
    State what_to_avoid = BLUE;
    color_avoider(what_to_avoid);
    digitalWrite(pin12, HIGH);
    delay(800);
    digitalWrite(pin12, LOW);
    forward_motion(pin5, pin6, pin10, pin9, power, power, 300);
    pivotleft(pin5, pin6, pin10, pin9, power, power, 200);
//    forward_motion(pin5, pin6, pin10, pin9, power, power, 400);
    counter += 1;
    
    delay(300);
    loop();
  }

  if(counter == 4){
    //backward_motion(pin5, pin6, pin10, pin9, power, power, 1000);
    
    State what_to_track = BLUE;
    line_tracker(what_to_track);
  }
   

  
  change_state();
}


//makes the bot move in a straight line and stop whenever it sees a certain color.
void color_avoider(State COLOR){
  int power = 80;
  while(color != COLOR){
    forward_motion(pin5, pin6, pin10, pin9, power, power, 40);
    change_state();
  }
  return;
}
  



void line_tracker(State COLOR) {
  int power = 80;
  turn_period = 20;
  while(tracking_bool) {
    change_state();
    
    while ((color == COLOR) && tracking_bool && collision_absence ) {
      turnright(pin5, pin6, pin10, pin9, power, power, turn_period);
      change_state();
    }
    
    while ((color != COLOR) && tracking_bool && collision_absence) {
      turnleft(pin5, pin6, pin10, pin9, power, power, turn_period);
      change_state();
    }

    while ((color == COLOR) && tracking_bool && collision_absence) {
      turnleft(pin5, pin6, pin10, pin9, power, power, turn_period);
      change_state();
    }

    while ((color != COLOR) && tracking_bool && collision_absence) {
      turnright(pin5, pin6, pin10, pin9, power, power, turn_period);
      change_state();
    }
  }
}

void change_state() {
  collision_absence = true;
  //color sensing
  prev = color;
  value = analogRead(A1);
  delay(5);
  if((value < 65)) {
    color = DARK;
  } else if((65 < value) and (value < 85)) {
    color = BLUE;
  } else if((85 < value) and (value < 100)) {
    color = RED;
  } else if(100 < value) {
    color = YELLOW;
  }
//  
//  if (prev == YELLOW && color == BLUE) {
//    color = YELLOW;a
//  }
  delay(10);
  Serial.print("COLOR = ");
  Serial.print(value + "  :  ");
  Serial.println(color);
  //collision sys
  collision_sense();
  if(collision_absence == false){
    counter += 1;
    loop();
  }

  ambient_light_sense();

//  if(collision_absence == false){
//    digitalWrite(pin12, HIGH);
//  }
//  if(collision_absence == true){
//    digitalWrite(pin12, LOW);
//  }

}


void ambient_light_sense(){
  ambient_light_analog = analogRead(A0);
//  Serial.print("Analog reading: ");
//  Serial.println(ambient_light_analog);
  if (ambient_light_analog > 7) {
    digitalWrite(8, LOW);
  } else if (ambient_light_analog < 7) {
    digitalWrite(8, HIGH);
    //tracking_bool = false;
  } 
}

void collision_sense(){
//  Serial.println(collision_absence);
  //val1 = 0;
  val1 = analogRead(A2);
  Serial.println(val1);

  
  if (val1 <= 670) {
    val1 = analogRead(A2);
    //Serial.print("LOW");    // sets the output pin initially to LOW
    //Serial.println(val1); 
    collision_absence = false;
    digitalWrite(pin12, HIGH);
    //horn(pin2);
    //counter = 5;
//  Serial.println(val1);
  }
  if (val1 >= 670) {
    val1 = analogRead(A2);
    //Serial.println(val1);
    //Serial.print("HIGH");    // sets output pin HIGH to activate special effects
   // delay(1000);  // waits for a second
    collision_absence = true;
    digitalWrite(pin12, LOW);
//    Serial.println(val1);
  }
  Serial.println(val1);
  
  
}
// motor control functions
void backward_motion(int rightwheel1, int rightwheel2, int leftwheel1, int leftwheel2, int analogright, int analogleft, int period){
  analogWrite(rightwheel2, 0);
  analogWrite(rightwheel1, analogright);
  analogWrite(leftwheel2, 0);
  analogWrite(leftwheel1, analogleft);
  delay(period);
  analogWrite(rightwheel1, 0);
  analogWrite(rightwheel2, 0);
  analogWrite(leftwheel2, 0);
  analogWrite(leftwheel1, 0);
}

void forward_motion(int rightwheel1, int rightwheel2, int leftwheel1, int leftwheel2, int analogright, int analogleft, int period){
  analogWrite(rightwheel2, analogright);
  analogWrite(rightwheel1, 0);
  analogWrite(leftwheel2, analogleft);
  analogWrite(leftwheel1, 0);
  delay(period);
  analogWrite(rightwheel1, 0);
  analogWrite(rightwheel2, 0);
  analogWrite(leftwheel1, 0);
  analogWrite(leftwheel2, 0);
}

void pivotright(int rightwheel1, int rightwheel2, int leftwheel1, int leftwheel2, int analogright, int analogleft, int period){
  analogWrite(rightwheel2, analogright);
  analogWrite(rightwheel1, 0);
  analogWrite(leftwheel2, 0);
  analogWrite(leftwheel1, analogleft);
  delay(period);
  analogWrite(rightwheel1, 0);
  analogWrite(rightwheel2, 0);
  analogWrite(leftwheel1, 0);
  analogWrite(leftwheel2, 0);
}

void pivotleft(int rightwheel1, int rightwheel2, int leftwheel1, int leftwheel2, int analogright, int analogleft, int period){
  analogWrite(rightwheel2, 0);
  analogWrite(rightwheel1, analogright);
  analogWrite(leftwheel2, analogleft);
  analogWrite(leftwheel1, 0);
  delay(period);
  analogWrite(rightwheel1, 0);
  analogWrite(rightwheel2, 0);
  analogWrite(leftwheel2, 0);
  analogWrite(leftwheel1, 0); 
}

void turnright(int rightwheel1, int rightwheel2, int leftwheel1, int leftwheel2, int analogright, int analogleft, int period){
  analogWrite(rightwheel2, analogright);
  analogWrite(rightwheel1, 0);
  analogWrite(leftwheel2, analogleft/4);
  analogWrite(leftwheel1, 0);
  delay(period);
  analogWrite(rightwheel1, 0);
  analogWrite(rightwheel2, 0);
  analogWrite(leftwheel2, 0);
  analogWrite(leftwheel1, 0);
}


void turnleft(int rightwheel1, int rightwheel2, int leftwheel1, int leftwheel2, int analogright, int analogleft, int period){
  analogWrite(rightwheel2, analogright/4);
  analogWrite(rightwheel1, 0);
  analogWrite(leftwheel2, analogleft);
  analogWrite(leftwheel1, 0);
  delay(period);
  analogWrite(rightwheel1, 0);
  analogWrite(rightwheel2, 0);
  analogWrite(leftwheel1, 0);
  analogWrite(leftwheel2, 0);
}

void horn(int pin){
  int i = 0;
  for(i=0; i<200; i++){
    digitalWrite(pin, HIGH);
    delay(5);
    digitalWrite(pin, LOW);
    delay(5); 
  }
  
}
