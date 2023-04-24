int LED = 7;
int LED2 = 6;
int val = 0;
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(9, INPUT_PULLUP);
  digitalWrite(LED, HIGH);
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);
}

// the loop function runs over and over again forever
void loop() {
  
    
  val = digitalRead(9);
  Serial.println(val);// wait for a second
  //if (val =)
  if (digitalRead(9) == HIGH) {
    digitalWrite(LED2, HIGH);
    //WRITE CODE TO USE WIFI TO MAKE OTHER ARDUINO MOVE FORWARD FOR 5 SECONDS


    
  } else if (digitalRead(9) == LOW) {
    digitalWrite(LED2, LOW);
  }
}
