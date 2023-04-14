int reciever_pin = 7;
int trans_pin = 2;
int indicator_led = 9;
unsigned long rememberTime = 0;
int detect_pulse_count = 0;
int pulse_count = 0;
bool on = true;
void setup()
{   
  Serial.begin(9600);    
  pinMode(reciever_pin, INPUT);
  pinMode(trans_pin, OUTPUT);    
  digitalWrite(trans_pin, LOW);

}
 
//does the specified amount of pulses
void send_pulse(int count){
//for some reason i have to add 2 idk why
  while(pulse_count < count+2){
    
    if((micros()- rememberTime) >= 333.33 and on ){   
      digitalWrite(trans_pin, HIGH);
      Serial.println("sending pulse");
      rememberTime=micros();
      on = false;
  
    }else if((micros()- rememberTime) >= 333.33 and not on){   
        digitalWrite(trans_pin, LOW);
        Serial.println("stopped pulse");
        pulse_count++;
        on = true;
        rememberTime=micros();
      }
   
  }
}

void detect_pulse(float prev_pulse){
  float pulse = digitalRead(reciever_pin);     
  if(pulse > prev_pulse){
    Serial.println("pulse recieved");
    detect_pulse_count++;
    Serial.println(detect_pulse_count);
  } 
  prev_pulse = pulse;

}

void loop() 
{ 
  send_pulse(4); // pulse 4 times
  delay(1000);

  //uncomment both for detection
  //float prev_pulse = digitalRead(reciever_pin);  
 // detect_pulse(prev_pulse);

}
