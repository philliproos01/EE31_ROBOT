//input pins
int pin0 = 0;
int pin1 = 1;
int pin2 = 2;

//analog out pins for fading
int pin9 = 9;
int pin10 = 10;
int pin11 = 11;

int blue_LED = pin9;
int green_LED = pin10;
int red_LED = pin11;

//interrupt pins
int pin4 = 4;
int pin5 = 5;
int pin6 = 6;
int pin7 = 7;
int pin8 = 8;

enum State {OFF, ON, RUN, SLEEP, DIAGNOSTIC};
State state = OFF;

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
  
}

void loop() {
  switch(state){
    case OFF:
      break;
      
    case ON:
      blink_LED(red_LED, 10, 1);
      break;

    case RUN:
      fade_LED(green_LED, 6);
      blink_LED(green_LED, 1, 2);
      state = OFF;
      break;

    case SLEEP:
      blink_LED(blue_LED, 4, 3);
      fade_LED(blue_LED, 1);
      state = OFF;
      break;

    case DIAGNOSTIC:
      blink_LED(red_LED, 2, N_diag);
      state = OFF;
      break;
    
  } 
}

void blink_LED(int pin, int freq, int iterations){
  int delay_val = 0;
  int i;
  for(i = 0; i < iterations; i++){
    delay_val = 1000 / (2 * freq);
    digitalWrite(pin, HIGH);
    delay(delay_val);
    digitalWrite(pin, LOW);
    delay(delay_val);
  }
}

void fade_LED(int analog_pin, int time_constant){
  int brightness = 255;
  int step_time = (int) (time_constant * 1000 / 254);
  while (brightness > 3) {
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
