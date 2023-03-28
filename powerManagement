#include <Wire.h>


#define led_pin 13

void setup()
{
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, LOW);
  Serial.begin(9600);
}

void loop()
{
  int sensorValue = analogRead(A0); //read the A0 pin value
  float voltage = sensorValue * (5.00 / 1023.00) * 2; //convert the value to a true voltage.
  Serial.print("voltage = ");
  Serial.println(voltage); //print the voltage to LCD
  Serial.print(" V");
  if (voltage < 6.50) //set the voltage considered low battery here
  {
    digitalWrite(led_pin, HIGH);
  } else if (voltage > 6.51) {
    digitalWrite(led_pin, LOW);
    delay(500);
    digitalWrite(led_pin, HIGH);
  }

}
