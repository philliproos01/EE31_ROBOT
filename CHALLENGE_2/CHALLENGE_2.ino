enum State {YELLOW, RED, BLUE, DARK};
State color = DARK;
State prev = DARK;

bool tracking_bool = true;
bool collision_absence = true;

int pin2 = 2; //horn
int pin4 = 4; //headlights
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

float calibrate_me = 1400;


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

  pinMode(pin12, OUTPUT); //brakes

  pinMode(pin2, OUTPUT);//horn
  
}

void loop() {
  if(counter == 0){
    forward_motion(pin5, pin6, pin10, pin9, power, power, calibrate_me);
    delay(500);
    pivotleft(pin5, pin6, pin10, pin9, power, power, calibrate_me * 0.70);
    delay(500);
    backward_motion(pin5, pin6, pin10, pin9, power, power, calibrate_me * 0.25);
    delay(500);
    pivotleft(pin5, pin6, pin10, pin9, power, power, calibrate_me * 0.35);
    delay(500);
    forward_motion(pin5, pin6, pin10, pin9, power, power, calibrate_me * 0.25);
    delay(500);
    pivotright(pin5, pin6, pin10, pin9, power, power, calibrate_me * 0.35);
    delay(500);
    forward_motion(pin5, pin6, pin10, pin9, power, power, calibrate_me * 1.4);
    delay(500);
    pivotright(pin5, pin6, pin10, pin9, power, power, calibrate_me * 0.35);
    delay(500);
    forward_motion(pin5, pin6, pin10, pin9, power, power, calibrate_me * 0.25);
    delay(500);
    pivotright(pin5, pin6, pin10, pin9, power, power, calibrate_me * 0.35);
    delay(500);
    forward_motion(pin5, pin6, pin10, pin9, power, power, calibrate_me * 0.25);
    delay(500);
    counter += 1;    
  }

  if(counter >= 1){
    //send wifi;
  }
}


// //makes the bot move in a straight line and stop whenever it sees a certain color.
// void color_avoider(State COLOR){
//   int power = 80;
//   while(color != COLOR){
//     forward_motion(pin5, pin6, pin10, pin9, power, power, 40);
//     change_state();
//   }
//   return;
// }
  



// void line_tracker(State COLOR) {
//   int power = 80;
//   turn_period = 20;
//   while(tracking_bool) {
//     change_state();
    
//     while ((color == COLOR) && tracking_bool && collision_absence ) {
//       turnright(pin5, pin6, pin10, pin9, power, power, turn_period);
//       change_state();
//     }
    
//     while ((color != COLOR) && tracking_bool && collision_absence) {
//       turnleft(pin5, pin6, pin10, pin9, power, power, turn_period);
//       change_state();
//     }

//     while ((color == COLOR) && tracking_bool && collision_absence) {
//       turnleft(pin5, pin6, pin10, pin9, power, power, turn_period);
//       change_state();
//     }

//     while ((color != COLOR) && tracking_bool && collision_absence) {
//       turnright(pin5, pin6, pin10, pin9, power, power, turn_period);
//       change_state();
//     }
//   }
// }

// void change_state() {
//   collision_absence = true;
//   //color sensing
//   prev = color;
//   value1 = analogRead(A1);
//   value2 = analogRead(A1);
//   value3 = analogRead(A1);
//   value4 = analogRead(A1);

// //  if(value1 == value2 and value2 == value3 and value3 == value4){
// //    value = value1;
// //  }
//   value = value1;
//  //3rd color sensor Greige
//   //delay(300);
//   if((value < 65)) {
//     color = DARK;
//   } else if((65 < value) and (value < 80)) {
//     color = BLUE;
//   } else if((80 < value) and (value < 110)) {
//     color = RED;
//   } else if(110 < value) {
//     color = YELLOW;
//   }
// //  
//   if (prev == YELLOW && color == BLUE) {
//     color = YELLOW;
//   }
//   delay(3);
//   Serial.print("COLOR = ");
// //  Serial.print(value);
//   Serial.println(color);
//   //collision sys
//   collision_sense();
//   if(collision_absence == false){
//     counter += 1;
//     loop();
//   }
// //  Serial.println(counter);
//   //ambient light sensing
//   ambient_light_sense();

// //  if(collision_absence == false){
// //    digitalWrite(pin12, HIGH);
// //  }
// //  if(collision_absence == true){
// //    digitalWrite(pin12, LOW);
// //  }

// }


// void ambient_light_sense(){
//   ambient_light_analog = analogRead(A0);
//   Serial.print("Analog reading: ");
//   Serial.println(ambient_light_analog);
//   if (ambient_light_analog > 7) {
//     digitalWrite(8, LOW);
//   } else if (ambient_light_analog < 7) {
//     digitalWrite(8, HIGH);
//     //tracking_bool = false;
//   } 
// }

// void collision_sense(){
// //  Serial.println(collision_absence);
//   //val1 = 0;
//   val1 = analogRead(A2);
//   Serial.println(val1);

  
//   if (val1 <= 670) {
//     val1 = analogRead(A2);
//     //Serial.print("LOW");    // sets the output pin initially to LOW
//     //Serial.println(val1); 
//     collision_absence = false;
//     digitalWrite(pin12, HIGH); //brake lights
      
//     //if(counter == 0){  
//       //flash_headlights(pin4);
//       //horn(pin2);
//     //}
//     //counter = 5;
// //  Serial.println(val1);
//   }
//   if (val1 >= 670) {
//     val1 = analogRead(A2);
//     //Serial.println(val1);
//     //Serial.print("HIGH");    // sets output pin HIGH to activate special effects
//    // delay(1000);  // waits for a second
//     collision_absence = true;
//     digitalWrite(pin12, LOW); //brake lights
// //    Serial.println(val1);
//   }
//   Serial.println(val1);
// }

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

// void horn(int pin){
//   int i = 0;
//   for(i=0; i<150; i++){
//     digitalWrite(pin, HIGH);
//     delay(5);
//     digitalWrite(pin, LOW);
//     delay(5); 
//   }
// }


// void flash_headlights(int pin){
//   int i = 0;
//   for(i=0; i<2; i++){
//     digitalWrite(pin, HIGH);
//     delay(300);
//     digitalWrite(pin, LOW);
//     delay(500); 
  
//     digitalWrite(pin, HIGH);
//     delay(300);
//     digitalWrite(pin, LOW);
//     delay(300); 
//   }
 
// }

// void brake_lights(int pin){
//   digitalWrite(pin, HIGH);
//   delay(800);
//   digitalWrite(pin, LOW);
// }
