//RECIPIENT BOT
#include <SPI.h>
#include <WiFiNINA.h>

#define SECRET_SSID "junior"
#define SECRET_PASS "designdesign"
#define COMMAND_ARRAY_SIZE 20
#define PARAMETER_ARRAY_SIZE 40
#define CHAR_ARRAY_LENGTH 25
#define MSGDATA_SIZE 255
#define VALUEARRAY_SIZE 15

char senderID[] = "UUID 1";
char receiverID[] = "UUID 2";
char ERYTHAEAN[] = "F79721857DC5";
char GREIGE[] = "89C87865077A";
char sender[] = "senderID";
char receiver[] = "receiverID";
static char messageData[MSGDATA_SIZE + 1];
static char valuesArray[VALUEARRAY_SIZE]; // 15 bc probably won't do more

int index = 0;
boolean CRLF2 = false;

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

char postBody[] =  "warninglight=0&headlights=1&variable3=3&variable4=4&variable5=5&variable6=6";
int sendBody = 0;

// separate commands into individual strings
int numberOfCommands; //number of commands
char commands[COMMAND_ARRAY_SIZE][CHAR_ARRAY_LENGTH];
char parameters[PARAMETER_ARRAY_SIZE][CHAR_ARRAY_LENGTH];
char delimiter_amper = '&';
char delimiter_equal = '=';

int status = WL_IDLE_STATUS;
char server[] = "ee31.ece.tufts.edu";  // for Tufts
int portNumber = 80;   // for Tufts

WiFiClient client;

enum State {YELLOW, RED, BLUE, DARK};
State color = DARK;
State prev = DARK;

bool tracking_bool = true;
bool collision_absence = true;

int pin2 = 2; //horn
int pin4 = 4;
int pin8 = 8;
int pin3 = 3; //power white light
int pin12 = 12; //brake lights
int ambient_light_analog = 0;
int value = 0;

int val1 = 0;

// motor declarations
int pivot_period = 1000;
int turn_period = 1000;
int power = 80;

// motor pins
int pin9 = 9;
int pin10 = 10;
int pin5 = 5;
int pin6 = 6;

// enable pins
int pin13 = 13;
int pin7 = 7;

int pin1 = 1; //light-based communication

int value1 = 0;
int value2 = 0;
int value3 = 0;
int value4 = 0;

int color1 = 0;

int light = 0; //light-based communication

int counter = -1; //start at 0 if doing light-based communication

void setup() {
  Serial.begin(9600);
  pinMode(pin3, OUTPUT);
  digitalWrite(pin3, HIGH); // power white LED

  //light-based comms
  pinMode(pin1, OUTPUT);

  // motor setup
  pinMode(pin13, OUTPUT);
  pinMode(pin9, OUTPUT);
  pinMode(pin10, OUTPUT);
  pinMode(pin5, OUTPUT);
  pinMode(pin6, OUTPUT);
  pinMode(pin7, OUTPUT);
  pinMode(pin8, OUTPUT); //ambient light sensor
  pinMode(pin4, OUTPUT); //headlights
  digitalWrite(pin13, HIGH); // enable1 pin
  digitalWrite(pin7, HIGH); // enable2 pin
  delay(1000);

  //digitalWrite(pin4, HIGH); //headlights
  pinMode(pin12, OUTPUT); //brakes
  //digitalWrite(pin12, HIGH); // power brakes test

  pinMode(pin2, OUTPUT);//horn

  //light_based_comm(pin1);
  //horn(pin2);
  delay(1000);
}

void loop() {
  //SET counter to -1 if want light based comms recipient
  if(counter == -1){ // will start moving when light based communication starts
    while(light < 25){
      light = analogRead(A5);
    }
    //"flash headlights and brake
    flash_brake_and_head(pin4, pin12);
    delay(500);
    horn(pin2);
    delay(500);
    flash_brake_and_head(pin4, pin12);
    delay(500);
    horn(pin2);
    delay(500);

    delay(1000);
    
    light_based_comm(pin1); //receipt acknowledgement.

    delay(7000);
    
    counter += 1;  
    
    
  }
  
  if(counter == 0){
    while(collision_absence){
      change_state();
      forward_motion(pin5, pin6, pin10, pin9, power, power, 100);
    }
  }

  if(counter == 1){
    delay(100);
    pivotleft(pin5, pin6, pin10, pin9, power, power, 1500); //180 degree pivot
    //forward_motion(pin5, pin6, pin10, pin9, power, power, 1100);
    counter += 1;
    loop();
  }

  if(counter == 2){
    delay(100);
    State what_to_avoid = BLUE;
    color_avoider(what_to_avoid);
    digitalWrite(pin12, HIGH);
    digitalWrite(pin12, LOW);
    counter += 1;
    loop();
  }

  if(counter == 3){
//    forward_motion(pin5, pin6, pin10, pin9, power, power, 200);
    pivotright(pin5, pin6, pin10, pin9, power, power, 200);
    delay(400);
    State what_to_track = BLUE;
    line_tracker(what_to_track);

    // ********************** GET ***********************
    Serial.println("Doing GET");
    char getRoute[] = "GET /89C87865077A/F79721857DC5 HTTP/1.1";
    GETServer(getRoute);
    Serial.print("Message: ");
    Serial.println(messageData);
    int parsedLength = parseMessage(valuesArray, messageData);
    valuesArray[parsedLength] = '\0';
    Serial.print("Values: ");
    Serial.println(valuesArray);
    
    while (valuesArray[3] != 'F') {
      Serial.println("WAIT");
    }

    counter += 1;
    delay(300);
    loop();
  }

  if(counter == 4){
    delay(100);
    pivotleft(pin5, pin6, pin10, pin9, power, power, 1100);
    counter += 1;
    loop();
  }

  if(counter == 5){
    delay(100);
    State what_to_avoid = YELLOW;
    color_avoider(what_to_avoid);
    digitalWrite(pin12, HIGH);
    delay(800);
    digitalWrite(pin12, LOW);
    counter += 1;
    loop();
  }

  if(counter == 6){
    forward_motion(pin5, pin6, pin10, pin9, power, power, 200);
    pivotleft(pin5, pin6, pin10, pin9, power, power, 200);
    State what_to_track = YELLOW;
    line_tracker(what_to_track);
    counter += 1;
    delay(300);
    loop();
  }

  if(counter == 7){
    pivotleft(pin5, pin6, pin10, pin9, power, power,900);
//    while(collision_absence){
//      change_state();
//      forward_motion(pin5, pin6, pin10, pin9, power, power, 100);
//    }
    forward_motion(pin5, pin6, pin10, pin9, power, power, 1200);
    delay(50000);
  }


  
  change_state();
}


//makes the bot move in a straight line and stop whenever it sees a certain color.
void color_avoider(State COLOR){
  int counting = 0;
  int power = 100;
  while(color != COLOR){
    forward_motion(pin5, pin6, pin10, pin9, power, power, 40);
    if (counting % 8 == 0){
      change_state();
    }
    counting += 1;
  }
  return;
}
  

bool check_signal() {
  // ********************** GET ***********************
  Serial.println("Doing GET");
  char getRoute[] = "GET /89C87865077A/F79721857DC5 HTTP/1.1";
  GETServer(getRoute);
  Serial.print("Message: ");
  Serial.println(messageData);
  int parsedLength = parseMessage(valuesArray, messageData);
  valuesArray[parsedLength] = '\0';
  Serial.print("Values: ");
  Serial.println(valuesArray);
  
  if (valuesArray[3] == 'W') {
    Serial.println("WAIT");
    // ********************** POST ***********************
    Serial.println("Doing POST");
    char postBody[] = "command=W";
    // format of postRoute: "POST /senderID/receiverID HTTP/1.1"
    char postRoute[] = "POST /89C87865077A/F79721857DC5 HTTP/1.1";
    POSTServer(postRoute, postBody);
  } else if (valuesArray[3] == 'S') {
    Serial.println("MOVE");
    // Blink RED LED
    brake_lights(pin12);
    // Send POST to bot 1
    // ********************** POST ***********************
    Serial.println("Doing POST");
    char postBody[] = "command=S";
    // format of postRoute: "POST /senderID/receiverID HTTP/1.1"
    char postRoute[] = "POST /89C87865077A/F79721857DC5 HTTP/1.1";
    POSTServer(postRoute, postBody);
  }
}

void brake_lights(int pin){
  digitalWrite(pin, HIGH);
  delay(1000);
  digitalWrite(pin, LOW);
  delay(1000);
}

void line_tracker(State COLOR) {
  int power = 140;
  turn_period = 20;
  while(tracking_bool) {
    change_state();
    
    while ((color == COLOR) && tracking_bool && collision_absence ) {
      turnright(pin5, pin6, pin10, pin9, power, power, turn_period);
      change_state();
    }
    
    while ((color != COLOR) && tracking_bool && collision_absence) {
      turnleft(pin5, pin6, pin10, pin9, power, power, turn_period);
      change_state();
    }

    while ((color == COLOR) && tracking_bool && collision_absence) {
      turnleft(pin5, pin6, pin10, pin9, power, power, turn_period);
      change_state();
    }

    while ((color != COLOR) && tracking_bool && collision_absence) {
      turnright(pin5, pin6, pin10, pin9, power, power, turn_period);
      change_state();
    }
  }
}

void change_state() {
  if (counter == 2 || counter == 3) {
    check_signal();
  }
  collision_absence = true;
  
  //color sensing
  prev = color;
  value = analogRead(A1);

  delay(5);
  if((value < 65)) {
    color = DARK;
  } else if((65 < value) and (value < 76)) {
    color = BLUE;
  } else if((76 < value) and (value < 110)) {
    color = RED;
  } else if(110 < value) {
    color = YELLOW;
  }
  delay(10);
  
//  if (prev == BLUE && color == RED) {
//    color = DARK;
//  }
//
//  if (prev == YELLOW && color == RED) {
//    color = DARK;
//  }


  Serial.print("COLOR = ");
  Serial.print(value);
  Serial.println(color);
  //collision sys
  collision_sense();
  if(collision_absence == false){
    counter += 1;
    loop();
  }
//  Serial.println(counter);
  //ambient light sensing
  ambient_light_sense();

//  if(collision_absence == false){
//    digitalWrite(pin12, HIGH);
//  }
//  if(collision_absence == true){
//    digitalWrite(pin12, LOW);
//  }

}


void ambient_light_sense(){
  ambient_light_analog = analogRead(A0);
//  Serial.print("Analog reading: ");
//  Serial.println(ambient_light_analog);
  if (ambient_light_analog > 7) {
    digitalWrite(8, LOW);
  } else if (ambient_light_analog < 7) {
    digitalWrite(8, HIGH);
    //tracking_bool = false;
  } 
}

void collision_sense(){
//  Serial.println(collision_absence);
  //val1 = 0;
  val1 = analogRead(A2);
  Serial.println(val1);

  
  if (val1 <= 670) {
    val1 = analogRead(A2);
    //Serial.print("LOW");    // sets the output pin initially to LOW
    //Serial.println(val1); 
    collision_absence = false;
    digitalWrite(pin12, HIGH);
    //horn(pin2);
    //counter = 5;
//  Serial.println(val1);
  }
  if (val1 >= 670) {
    val1 = analogRead(A2);
    //Serial.println(val1);
    //Serial.print("HIGH");    // sets output pin HIGH to activate special effects
   // delay(1000);  // waits for a second
    collision_absence = true;
    digitalWrite(pin12, LOW);
//    Serial.println(val1);
  }
  Serial.println(val1);
  
  
}
// motor control functions
void backward_motion(int rightwheel1, int rightwheel2, int leftwheel1, int leftwheel2, int analogright, int analogleft, int period){
  analogWrite(rightwheel2, 0);
  analogWrite(rightwheel1, analogright);
  analogWrite(leftwheel2, 0);
  analogWrite(leftwheel1, analogleft);
  delay(period);
  analogWrite(rightwheel1, 0);
  analogWrite(rightwheel2, 0);
  analogWrite(leftwheel2, 0);
  analogWrite(leftwheel1, 0);
}

void forward_motion(int rightwheel1, int rightwheel2, int leftwheel1, int leftwheel2, int analogright, int analogleft, int period){
  analogWrite(rightwheel2, analogright);
  analogWrite(rightwheel1, 0);
  analogWrite(leftwheel2, analogleft);
  analogWrite(leftwheel1, 0);
  delay(period);
  analogWrite(rightwheel1, 0);
  analogWrite(rightwheel2, 0);
  analogWrite(leftwheel1, 0);
  analogWrite(leftwheel2, 0);
}

void pivotright(int rightwheel1, int rightwheel2, int leftwheel1, int leftwheel2, int analogright, int analogleft, int period){
  analogWrite(rightwheel2, analogright);
  analogWrite(rightwheel1, 0);
  analogWrite(leftwheel2, 0);
  analogWrite(leftwheel1, analogleft);
  delay(period);
  analogWrite(rightwheel1, 0);
  analogWrite(rightwheel2, 0);
  analogWrite(leftwheel1, 0);
  analogWrite(leftwheel2, 0);
}

void pivotleft(int rightwheel1, int rightwheel2, int leftwheel1, int leftwheel2, int analogright, int analogleft, int period){
  analogWrite(rightwheel2, 0);
  analogWrite(rightwheel1, analogright);
  analogWrite(leftwheel2, analogleft);
  analogWrite(leftwheel1, 0);
  delay(period);
  analogWrite(rightwheel1, 0);
  analogWrite(rightwheel2, 0);
  analogWrite(leftwheel2, 0);
  analogWrite(leftwheel1, 0); 
}

void turnright(int rightwheel1, int rightwheel2, int leftwheel1, int leftwheel2, int analogright, int analogleft, int period){
  analogWrite(rightwheel2, analogright);
  analogWrite(rightwheel1, 0);
  analogWrite(leftwheel2, analogleft/4);
  analogWrite(leftwheel1, 0);
  delay(period);
  analogWrite(rightwheel1, 0);
  analogWrite(rightwheel2, 0);
  analogWrite(leftwheel2, 0);
  analogWrite(leftwheel1, 0);
}


void turnleft(int rightwheel1, int rightwheel2, int leftwheel1, int leftwheel2, int analogright, int analogleft, int period){
  analogWrite(rightwheel2, analogright/4);
  analogWrite(rightwheel1, 0);
  analogWrite(leftwheel2, analogleft);
  analogWrite(leftwheel1, 0);
  delay(period);
  analogWrite(rightwheel1, 0);
  analogWrite(rightwheel2, 0);
  analogWrite(leftwheel1, 0);
  analogWrite(leftwheel2, 0);
}

void horn(int pin){
  int i = 0;
  for(i=0; i<200; i++){
    digitalWrite(pin, HIGH);
    delay(5);
    digitalWrite(pin, LOW);
    delay(5); 
  }
  
}

void light_based_comm(int pin){
  int i = 0;
  for(i=0; i<50000; i++){
    digitalWrite(pin, HIGH);
    delayMicroseconds(168);
    digitalWrite(pin, LOW);
    delayMicroseconds(168);
  }
}

void flash_brake_and_head(int pin4, int pin12){
    digitalWrite(pin4, HIGH); //headlights
    digitalWrite(pin12, HIGH);
    delay(500);
    digitalWrite(pin4, LOW);
    digitalWrite(pin12, LOW);
    delay(500);
    digitalWrite(pin4, HIGH); 
    digitalWrite(pin12, HIGH);
    delay(500);
    digitalWrite(pin4, LOW);
    digitalWrite(pin12, LOW);
}
// POST function doing actual call
void POSTServer(const char theRoute[], char *bodyMessage) {  
 if (client.connect(server, portNumber)) {
    // Make a HTTP POST request:
    client.println(theRoute);
    client.print("Host: ");
    client.println(server);
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    int postBodyLength = strlen(bodyMessage);
    client.println(postBodyLength);
    client.println();
    client.print(bodyMessage);
 }
}

// GET function doing actual call
void GETServer(const char theRoute[]) {
      
      if (client.connect(server, portNumber)) {
      Serial.println("GET: Connected to server");

      // connected to server, turn on the LED
      Serial.println("Server connected");
      
      // Make a HTTP GET request:
      client.println(theRoute);
      client.print("Host: ");
      client.println(server);
      client.println("Connection: close");
      client.println();
 
      delay(150); // need a delay to wait for the server to respond
      
      GetMessageBody();  // get the message from the server into an array
      // if the server's disconnected, stop the client:
      if (!client.connected()) {
      Serial.println("disconnecting from server.");
      Serial.println();
      client.stop();
      CRLF2 = false;
      }
      
   }
}

// read in from the client to the global variable
void  GetMessageBody() {
  // the actual message/information starts after the empty line
  int firstReading = client.read();
  int secondReading = 0;
  int index = 0;
  // client.read() returns -1 when there is no more data
  while(firstReading != -1) {
    // 13 = carriage return, 10 = line feed
    // one after the other means an empty line 
    //(time to start reading the actual message)
    if (firstReading == 13 && secondReading == 10) {
      break; 
    }
    secondReading = firstReading;
    firstReading = client.read();
  }
  // reading the actual message
  while(firstReading != -1 && index < MSGDATA_SIZE) {
    messageData[index] = firstReading;
    index++;
    firstReading = client.read();
  }
  messageData[index] = '\0';
}

// returns the int number of values including initial 3
int parseMessage(char buff[], char message[]) {
  // first = senderID, second = receiverID, third = message (Succ/Fail)
  // following are the values
  int values = 0;
  for (int i = 0; i < MSGDATA_SIZE; i++) {
    // end of the message
    if (message[i] == '\0') {
      break;
    }
    // start of a value (this assumes each value is just one character)
    if (message[i] == '=') {
      buff[values] = message[i+1];
      values++;
    }
  }
  numberOfCommands = values - 3;
  return values;
}

