#define BUTTON_PIN1 6
#define BUTTON_PIN2 5

// hey
int state = 0;

void setup() {
  // put your setup code here, to run once:
  state = 1;
  pinMode(13, OUTPUT);
}

void loop() {


  switch state
  {
    
  case 1:
    if (digitREAD(BUTTON_PIN1) == HIGH) {
      digitalWrite(12, HIGH);
      delay(1000);   
      digitalWrite(12, LOW);
      delay(1000);   
    }

  case 2:
    if (digitREAD(BUTTON_PIN2) == HIGH) {
      digitalWrite(11, HIGH);
      delay(1000);   
      digitalWrite(11, LOW);
      delay(1000);   
    }
  }
  //THIS WORKS
  //digitalWrite(13, HIGH);  // turn the LED on (HIGH is the voltage level)
  //delay(1000);                      // wait for a second
  //digitalWrite(13, LOW);   // turn the LED off by making the voltage LOW
  //delay(1000);
}
