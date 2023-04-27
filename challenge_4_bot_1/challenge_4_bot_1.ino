//Challenge 4 — Night conditions: 


// The swarmbot must drive at night (dim room lighting
// conditions) using only headlights (maximum two white LEDs) toward each other and
// avoid a collision


// The test will be to place the bots at opposite ends of the longer
// dimension of the track (the yellow path)


//The two white diodes representing the headlights are allowed as sources of illumination for this challenge


// A photodiode,
// phototransistor, or photo resistor is to be used as the detector. When the two bots become
// close enough to detect each other’s presence, they must stop, communicate, flash their
// headlights twice, and illuminate their rear yellow turn signals


//------ Drive towards a bot!
//------ Turn on the headlights(two white diodes)
//------ Use collision detection system to detect obstacle and the photo resistor to make sure it is another bot.
//------ Stop
//------ Communicate (wifi???)
//------ Flash headlights twice
//------ Eliminate rear yellow turn signals
//------ Turn aside (figure out what direction to turn)

//modules needed

//------  access to motors
//------  access to headlights
//------  access to collision detection system
//------  access to communication system
//------  access yellow turn signals


const int M1_EN = 13;
const int M1_lead1 = 9;
const int M1_lead2 = 10;
const int M2_EN = 7;
const int M2_lead1 = 5;
const int M2_lead2 = 6;

// motor pins
int pin9 = 9;
int pin10 = 10;
int pin5 = 5;
int pin6 = 6;

// enable pins
int pin13 = 13;
int pin7 = 7;


float calibrate_me = 1400;
int collision_threshold = 400;

// Initialize collision detection pins
const int collision_pin = A2;
bool no_collision = false;
int collision_value = 0;


// Initialize headlight and turn signal pins
const int headlight_pin = 4;
const int turn_signal_pin = 12;

// Initialize motor speeds
const int motor_speed = 200;
const int pivot_speed = 200;
const int turn_speed = 200;
int power = 30;

int val1 = 0;

int pin12 = 12;


// Function to initialize pins and Wi-Fi connection
void setup() {
  pinMode(M1_EN, OUTPUT);
  pinMode(M1_lead1, OUTPUT);
  pinMode(M1_lead2, OUTPUT);
  pinMode(M2_EN, OUTPUT);
  pinMode(M2_lead1, OUTPUT);
  pinMode(M2_lead2, OUTPUT);
  pinMode(collision_pin, INPUT);
  pinMode(headlight_pin, OUTPUT);
  pinMode(turn_signal_pin, OUTPUT);
  // motor setup
  pinMode(pin13, OUTPUT);
  pinMode(pin9, OUTPUT);
  pinMode(pin10, OUTPUT);
  pinMode(pin5, OUTPUT);
  pinMode(pin6, OUTPUT);
  pinMode(pin7, OUTPUT);
  

  Serial.begin(9600);
}

// Function to move the robot forward
void forward() {
  digitalWrite(M1_EN, HIGH);
  digitalWrite(M2_EN, HIGH);
  analogWrite(M1_lead2, 0);
  analogWrite(M2_lead2, 65);
  analogWrite(M1_lead1, 65);
  analogWrite(M2_lead1, 0);
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

// Function to blink the headlights twice
void blink_headlights() {
  for (int i = 0; i < 2; i++) {
    digitalWrite(headlight_pin, HIGH);
    delay(500);
    digitalWrite(headlight_pin, LOW);
    delay(500);
  }
}

// Function to blink the turn signal once
void blink_turn_signal() {
    digitalWrite(turn_signal_pin, HIGH);
    delay(1000);
    digitalWrite(turn_signal_pin, LOW);
    delay(1000);
  
  
}


// Function to move the robot backward
void backward() {
  digitalWrite(M1_EN, HIGH);
  digitalWrite(M2_EN, HIGH);
  analogWrite(M1_lead1, motor_speed);
  analogWrite(M2_lead1, motor_speed);
  analogWrite(M1_lead2, 0);
  analogWrite(M2_lead2, 0);
}

  // Function to turn the robot
void turn(int direction) {
  if (direction == 1) { // turn left
    digitalWrite(M1_EN, HIGH);
    digitalWrite(M2_EN, HIGH);
    analogWrite(M1_lead2, turn_speed);
    analogWrite(M2_lead1, turn_speed);
    analogWrite(M1_lead1, 0);
    analogWrite(M2_lead2, 0);
  }
  else if (direction == 2) { // turn right
    digitalWrite(M1_EN, HIGH);
    digitalWrite(M2_EN, HIGH);
    analogWrite(M1_lead1, turn_speed);
    analogWrite(M2_lead2, turn_speed);
    analogWrite(M1_lead2, 0);
    analogWrite(M2_lead1, 0);
  }
}

// Function to stop the robot
void stop() {
  digitalWrite(M1_EN, LOW);
  digitalWrite(M2_EN, LOW);
  analogWrite(M1_lead1, 0);
  analogWrite(M1_lead2, 0);
  analogWrite(M2_lead1, 0);
  analogWrite(M2_lead2, 0);
}

// // Function to detect collisions using the collision detection system
// void collision_sense(){

//   val1 = analogRead(A2);
  
//   Serial.println(val1);

//   if (val1 <= 40) { 
//     no_collision = true;
//     Serial.println("no collision ");
//   }
//   else if (val1 >= 40) {
//     no_collision = false;
    
//     Serial.println("collision");
//   }
//   Serial.println(val1);
  
  
// }

// Function to communicate with the other bot using Wi-Fi
void communicate() {

}

void loop() {

  digitalWrite(headlight_pin, HIGH);
  delay(500);

  val1 = analogRead(A2);
  
  Serial.println(val1);
  
  collision_sense();

  while(no_collision == false){

    Serial.println("stop");

    stop();
    communicate(); // send a message to bot 2 to stop

    //--- blink the headlights twice and turn signal once --//
    blink_headlights();
    delay(500);
    blink_turn_signal();
    delay(500);

    no_collision = true;
     delay(500);

     stop();

  }

  Serial.println("moving forward!!");
  //Move the robot forward by default
  forward();

}

void collision_sense(){
  collision_value = analogRead(A2);
  if (collision_value >= collision_threshold) {
    no_collision = false;
  } else {
    no_collision = true;
  }
}
