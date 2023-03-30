void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

void loop() {
  // reads the input on analog pin A0 (value between 0 and 1023)
  int analogValue = analogRead(A0);

  Serial.print("Analog reading: ");
  Serial.println(analogValue);   // the raw analog reading

  // We'll have a few threshholds, qualitatively determined
  if (analogValue > 500) {
    Serial.println("DAY");
    digitalWrite(8, LOW);
  } else if (analogValue < 500) {
    Serial.println("NIGHT");
    digitalWrite(8, HIGH);
  } 

  delay(500);
}
