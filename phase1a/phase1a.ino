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
      blink_LED(green_LED, 2, 3);
      break;
      
    case ON:
      blink_LED(red_LED, 2, 3);
      break;

    case RUN:
      blink_LED(blue_LED, 2, 3);
      break;

    case SLEEP:
      blink_LED(blue_LED, 20, 3);
      break;

    case DIAGNOSTIC:
      blink_LED(green_LED, 20, 3);
      break;
    
  } 
}

void blink_LED(int pin, int freq, int iterations){
  int delay_val = 0;
  int i;
  for(i = 0; i < iterations; i++){
    delay_val = 1000/(2*freq);
    digitalWrite(pin, HIGH);
    delay(delay_val);
    digitalWrite(pin, LOW);
    delay(delay_val);
  }
}

void fade_LED(int analog_pin, int time_constant){
  int brightness = 255;
  float time_val = 0;
  float delay_val = 5;
  while(1){
    analogWrite(analog_pin, brightness);
    delay(delay_val); //milliseconds
    time_val = time_val + delay_val/1000;
    brightness = 255*exp(-time_constant*time_val);
    Serial.println(brightness);
    if(brightness < 3){
      break;
    }
  }
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