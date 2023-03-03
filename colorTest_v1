//Define Pins
int redled = 3;
int greenled = 11;
int blueled = 2;

int photores = A4;

//Define Time each LED is on
int pulsetime = 100;

//Define Photoresistor's value depended on which color is active
int redValue = 0;
int greenValue = 0;
int blueValue = 0;

void setup() {
 
 //enable serial output to serial monitor 
  Serial.begin(9600);
  
 //Set RGB LED Pins as outputs 
  pinMode(redled, OUTPUT);
  pinMode(greenled, OUTPUT);
  pinMode(blueled, OUTPUT);
 

}

void loop() {
 
  //Turn on Red LED 
  digitalWrite(redled, LOW);
  digitalWrite(greenled, HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(blueled, HIGH);  
 
 //measure the Photoresistors value 
  redValue = analogRead(photores);
 
 //send RED photoresistor value to serial monitor
  Serial.print("Red: ");
  Serial.println(redValue);
 // wait for a 2milliseconds 
  delay(pulsetime);               
  

  digitalWrite(redled, HIGH);
  digitalWrite(greenled, LOW);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(blueled, HIGH);     // turn the LED off by making the voltage LOW

  greenValue = analogRead(photores);
 

  Serial.print("Green: ");
  Serial.println(greenValue);
 

  delay(pulsetime);
 
 //Turn on Blue LED 
  digitalWrite(redled, HIGH);
  digitalWrite(greenled, HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(blueled, LOW);     // turn the LED off by making the voltage LOW
  
 
  blueValue = analogRead(photores);
 
 
  Serial.print("Blue: ");
  Serial.println(blueValue);
 //Wait for 2 milliseconds 
  delay(pulsetime);
 
 
  if(blueValue < greenValue && blueValue < redValue && greenValue < redValue){
 
  Serial.println("_");
  Serial.println("BLUE!");
  Serial.println("_");}
 
  if(greenValue < blueValue && greenValue < redValue){
 
  Serial.println("_");
  Serial.println("GREEN!");
  Serial.println("_");}
  
  if(redValue < greenValue){
 
  Serial.println("_");
  Serial.println("RED!");
  Serial.println("_");}   
}
