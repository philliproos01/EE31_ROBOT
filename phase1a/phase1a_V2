//potentiometer pin
#define POTENTIOMETER_PIN A1
int outValuePot = 0;

//input pins
int pin0 = 0;
int pin1 = 1;


//analog out pins for fading
int pin9 = 9;
int pin10 = 10;
int pin11 = 11;

int blue_LED = pin9;
int green_LED = pin10;
int red_LED = pin11;

//interrupt pins
int pin2 = 2; //switch 2
int pin3 = 3; //switch 1
int pin4 = 4;
int pin5 = 5;
int pin6 = 6;
int pin7 = 7;
int pin8 = 8;

enum State {OFF, ON, RUN, SLEEP, DIAGNOSTIC};
//enum subState {SWITCH1, SWITCH2}; //ALSO BROKEN

State state = OFF;
State state_previous = OFF;

int N_diag = 5;

//LED pattern functions
void blink_LED(int analog_pin, int freq, int iterations);
void fade_LED(int pin, int time_constant);

//interrupt functions
void off_interrupt();
void on_interrupt();
void run_interrupt();
void sleep_interrupt();
void diagnostic_interrupt();

/* THESE ARE BROKEN
void switch1_interrupt();
void switch2_interrupt();
*/

void setup() {
  Serial.begin(9600);
  pinMode(pin9, OUTPUT);
  pinMode(pin10, OUTPUT);
  pinMode(pin11, OUTPUT);

  //interrupts
  pinMode(pin4, INPUT_PULLUP);
  pinMode(pin5, INPUT_PULLUP);
  pinMode(pin6, INPUT_PULLUP);
  pinMode(pin7, INPUT_PULLUP);
  pinMode(pin8, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pin8), off_interrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pin7), on_interrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pin6), run_interrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pin5), sleep_interrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pin4), diagnostic_interrupt, CHANGE);

  //dipswitches
  pinMode(pin3, INPUT_PULLUP);
  pinMode(pin2, INPUT_PULLUP);
}

void loop() {
  // 20 < x and x < 28
  int potentiometerValue = (analogRead(POTENTIOMETER_PIN)); //  * 4) - 1700;
  outValuePot = potentiometerValue;
  //outValuePot = map(potentiometerValue, 0, 1023, 0, 255);
  Serial.println(outValuePot);
  switch(state){
    case OFF:
      Serial.println("ENTER OFF STATE");
      state_previous = OFF;
      break;
      
    case ON:
      Serial.println("ENTER ON STATE");
      state_previous = ON;
      blink_LED(red_LED, 10, 1);
      break;

    case RUN:
      if (digitalRead(pin3) == LOW && digitalRead(pin2) == HIGH) {
        Serial.println("pin3 low and pin2 high");
        Serial.println("This is the default state, switch #1 is open and switch #2 is closed");
        if (outValuePot <= 107) { 
          Serial.println("ENTER RUN STATE");
          state_previous = RUN;
          fade_LED(green_LED, 6);
          blink_LED(green_LED, 1, 2); 
        }

        if (outValuePot >= 108) {
          Serial.println("ENTER RUN STATE");
          state_previous = RUN;
          fade_LED(green_LED, 3);
          blink_LED(green_LED, outValuePot, 2); 
        } 
      } else if (digitalRead(pin3) == HIGH && digitalRead(pin2) == HIGH) {
        Serial.println("pin3 high and pin2 is high");
        Serial.println("Switch #1 moved from OPEN to CLOSED. Switch #2 is closed");
        if (outValuePot <= 107) { 
          Serial.println("ENTER RUN STATE");
          state_previous = RUN;
          blink_LED(blue_LED, 1, 2); 
          fade_LED(green_LED, 6);
          blink_LED(green_LED, 1, 2); 
        }

        if (outValuePot >= 108) {
          Serial.println("ENTER RUN STATE");
          state_previous = RUN;
          blink_LED(blue_LED, outValuePot, 2);
          fade_LED(green_LED, 3);
          blink_LED(green_LED, outValuePot, 2); 
        } 
      } else if (digitalRead(pin3) == HIGH && digitalRead(pin2) == LOW) {
        Serial.println("pin3 high pin2 low");
        Serial.println("Switch #1 moved from OPEN to CLOSED. Switch #2 is OPEN. RED AND BLUE SHOULD BE ON");
        if (outValuePot <= 107) { 
          Serial.println("ENTER RUN STATE");
          state_previous = RUN;
          blink_LED(blue_LED, 1, 2);
          blink_LED(red_LED, 1, 2);  
          fade_LED(green_LED, 6);
          blink_LED(green_LED, 1, 2); 
        }

        if (outValuePot >= 108) {
          Serial.println("ENTER RUN STATE");
          state_previous = RUN;
          blink_LED(blue_LED, outValuePot, 2);
          blink_LED(red_LED, outValuePot, 2);  
          fade_LED(green_LED, 3);
          blink_LED(green_LED, outValuePot, 2); 
        } 
      }
      break;

    case SLEEP:
      Serial.println("ENTER SLEEP STATE");
      state_previous = SLEEP;
      blink_LED(blue_LED, 4, 3);
      fade_LED(blue_LED, 1);
      state = OFF;
      break;

    case DIAGNOSTIC:
      Serial.println("ENTER DIAGNOSTIC STATE");
      state_previous = DIAGNOSTIC;
      blink_LED(red_LED, 2, N_diag);
      state = OFF;
      break;
    
  } 
}

void blink_LED(int pin, int freq, int iterations){
  int delay_val = 0;
  int i;
  for(i = 0; i < iterations; i++){
    if (state != state_previous) {
      Serial.println(state);
      break;  
    }
    delay_val = 1000 / (2 * freq);
    digitalWrite(pin, HIGH);
    delay(delay_val);
    digitalWrite(pin, LOW);
    delay(delay_val);
  } 
  return;
}

void fade_LED(int analog_pin, int time_constant){
  int brightness = 255;
  int step_time = (int) (time_constant * 1000 / 254);
  while (brightness > 3) {
    if (state != state_previous) {
      analogWrite(analog_pin, 0);
      return;  
    }
    analogWrite(analog_pin, brightness);
    brightness -= 1;
    delay(step_time);
  }
  analogWrite(analog_pin, 0);
}

//interrupt functions

void off_interrupt(){
  state = OFF;
}

void on_interrupt(){
  state = ON;
}

void run_interrupt(){
  state = RUN;
}

void sleep_interrupt(){
  state = SLEEP;
}

void diagnostic_interrupt(){
  state = DIAGNOSTIC;
}
/*
void switch1_interrupt(){
  subState = SWITCH1;
}

void switch2_interrupt(){
  subState = SWITCH2;
}
*/
