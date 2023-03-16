int pin3 = 3; //power white light

int red_val = 0;
int yel_val = 0;
int blue_val = 0;


void setup() {
  Serial.begin(9600);
  pinMode(pin3, OUTPUT);
}

void loop() {
  digitalWrite(pin3, HIGH); #power white LED
  yel_val = analogRead(A1) ;
  
  Serial.print("YELLOW = ");
  Serial.print(yel_val);
  Serial.println("  ");
  
  if((yel_val > 500)and(yel_val < 540)){
    Serial.println("     BLUEEEEEEE");
  }
  if((yel_val > 540)and(yel_val < 650)){
    Serial.println("     REDDDD");
  }
  if((yel_val > 650)){
    Serial.println("     YELLOWWWWW");
  }
  if((yel_val < 500)){
    Serial.println("     DARKKKKKK");
  }
  delay(500);
  
}
