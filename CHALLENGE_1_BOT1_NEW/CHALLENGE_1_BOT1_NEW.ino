// ********************************* CHALLENGE 1 BOT 1 *********************************
#include <SPI.h>
#include <WiFiNINA.h>

#define SECRET_SSID "junior"
#define SECRET_PASS "designdesign"
#define COMMAND_ARRAY_SIZE 20
#define PARAMETER_ARRAY_SIZE 40
#define CHAR_ARRAY_LENGTH 25
#define MSGDATA_SIZE 255
#define VALUEARRAY_SIZE 15
#define ERYTHAEAN F79721857DC5
#define GREIGE 89C87865077A

// *********************************** CALIBRATION **************************************
float calibrate_me = 1000;
int collision_threshold = 850;

// ************************************** WIFI ******************************************
static char messageData[MSGDATA_SIZE + 1];
static char valuesArray[VALUEARRAY_SIZE];

int index = 0;
boolean CRLF2 = false;

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

// separate commands into individual strings
int numberOfCommands; //number of commands
char commands[COMMAND_ARRAY_SIZE][CHAR_ARRAY_LENGTH];
char parameters[PARAMETER_ARRAY_SIZE][CHAR_ARRAY_LENGTH];
char delimiter_amper = '&';
char delimiter_equal = '=';

int status = WL_IDLE_STATUS;
char server[] = "ee31.ece.tufts.edu";
int portNumber = 80;

WiFiClient client;

// ************************************** COLOR ******************************************
enum State {YELLOW, RED, BLUE, DARK};
State color = DARK;
State prev = DARK;
bool tracking_bool = true;
int color_value = 0;


// ************************************** COLLISION **************************************
bool no_collision = true;
int collision_value = 0;

// ************************************** AMBIENT ****************************************
int pin8 = 8;

// ************************************** PIN SETUP **************************************
int horn_pin = 2; // horn
int headlight_pin = 4; // headlights
int brake_pin = 12; // brake lights
int white_LED_pin = 3; // power whitelight
int ambient_light_analog = 0;

// *************************************** MOTOR *****************************************
int turn_period = 1000;
int power = 80;

// motor pin setup
int pin9 = 9;
int pin10 = 10;
int pin5 = 5;
int pin6 = 6;

// enable pins
int pin13 = 13;
int pin7 = 7;

int counter = 0;

// *************************************** SETUP *****************************************
void setup() {
  Serial.begin(9600);
  pinMode(white_LED_pin, OUTPUT);
  digitalWrite(white_LED_pin, HIGH); // power white LED

  // MOTOR SETUP
  pinMode(pin13, OUTPUT);
  pinMode(pin9, OUTPUT);
  pinMode(pin10, OUTPUT);
  pinMode(pin5, OUTPUT);
  pinMode(pin6, OUTPUT);
  pinMode(pin7, OUTPUT);
  pinMode(pin8, OUTPUT); //ambient light sensor
  pinMode(headlight_pin, OUTPUT); //headlights
  digitalWrite(pin13, HIGH); // enable1 pin
  digitalWrite(pin7, HIGH); // enable2 pin
  delay(1000);

  pinMode(brake_pin, OUTPUT); //brakes

  pinMode(horn_pin, OUTPUT);//horn
  // start up cycle
  // check for the WiFi module:
  while (status != WL_CONNECTED) {
    Serial.print("Attemping to connect to Network named: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
  }
  // end start up cycle
  Serial.print("SSID: ");
  Serial.print(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  IPAddress gateway = WiFi.gatewayIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // Saying WAIT to the other bot
  char postBody[] = "command=W";
  char postRoute[] = "POST /F79721857DC5/89C87865077A HTTP/1.1";
  POSTServer(postRoute, postBody);
}

// *************************************** LOOP ****************************************
void loop() {
  if(counter == 0){
    // MOVE FORWARD UNTIL SEEING WALL
    // **********************************************************************
    while(no_collision){
      change_state();
      forward_motion(pin5, pin6, pin10, pin9, power, power, 100);
    }
    delay(1000);
    
    // PIVOT RIGHT TO TURN ABOUT 160 degrees
    // **********************************************************************
    pivotright(pin5, pin6, pin10, pin9, power, power, calibrate_me);
    delay(1000);

    // MOVE FORWARD UNTIL SEEING RED
    // **********************************************************************
    color = DARK;
    while(color != RED){
      change_state();
      forward_motion(pin5, pin6, pin10, pin9, power, power, 100);
    }
    delay(1000);

    // TRACK RED LINE UNTIL SEEING WALL
    // **********************************************************************
    // The stop function will be called when change_state sees a collision
    line_tracker(RED);
    delay(1000);

    // TURN LEFT AROUND 90 DEGREES
    // **********************************************************************
    pivotleft(pin5, pin6, pin10, pin9, power, power, calibrate_me * 0.40);
    delay(1000);

    // MOVE FORWARD UNTIL SEEING YELLOW
    // **********************************************************************
    start_forward_motion(pin5, pin6, pin10, pin9, power, power);
    color = DARK;
    while(color != YELLOW){
      change_state();
      forward_motion(pin5, pin6, pin10, pin9, power, power, 100);
    }
    delay(1000);


    // TRACK YELLOW LINE UNTIL SEEING WALL
    // **********************************************************************
    line_tracker(YELLOW);
    delay(1000);

    // TURN LEFT AROUND 90 DEGREES
    // **********************************************************************
    pivotleft(pin5, pin6, pin10, pin9, power, power, calibrate_me * 0.40);
    delay(1000);


    // MOVE FORWARD UNTIL SEEING WALL
    // **********************************************************************
    start_forward_motion(pin5, pin6, pin10, pin9, power, power);
    while(no_collision){
      change_state();
      forward_motion(pin5, pin6, pin10, pin9, power, power, 100);
    }
    delay(1000);
  }

  // if(counter >= 1){
  //   //send wifi;
  //   // ********************** POST ***********************
  //   Serial.println("Doing POST");
  //   char postBody[] = "command=M";
  //   // format of postRoute: "POST /senderID/receiverID HTTP/1.1"
  //   char postRoute[] = "POST /F79721857DC5/89C87865077A HTTP/1.1";
  //   POSTServer(postRoute, postBody);
  // }
}


// //makes the bot move in a straight line and stop whenever it sees a certain color.
// void color_avoider(State COLOR){
//   int power = 80;
//   while(color != COLOR){
//     forward_motion(pin5, pin6, pin10, pin9, power, power, 40);
//     change_state();
//   }
//   return;
// }
  



void line_tracker(State COLOR) {
  int power = 80;
  turn_period = 20;
  while(tracking_bool) {
    change_state();
    
    while ((color == COLOR) && tracking_bool && no_collision ) {
      turnright(pin5, pin6, pin10, pin9, power, power, turn_period);
      change_state();
    }
    
    while ((color != COLOR) && tracking_bool && no_collision) {
      turnleft(pin5, pin6, pin10, pin9, power, power, turn_period);
      change_state();
    }

    while ((color == COLOR) && tracking_bool && no_collision) {
      turnleft(pin5, pin6, pin10, pin9, power, power, turn_period);
      change_state();
    }

    while ((color != COLOR) && tracking_bool && no_collision) {
      turnright(pin5, pin6, pin10, pin9, power, power, turn_period);
      change_state();
    }
    if (!no_collision) {
      return;
    }
  }
}

void change_state() {
  no_collision = true;
  //color sensing
  prev = color;
  color_value = analogRead(A1);

  if((color_value < 65)) {
    color = DARK;
  } else if((65 < color_value) and (color_value < 80)) {
    color = BLUE;
  } else if((80 < color_value) and (color_value < 110)) {
    color = RED;
  } else if(110 < color_value) {
    color = YELLOW;
  }

  if (prev == YELLOW && color == BLUE) {
    color = YELLOW;
  }
  delay(3);
  // collision sensing while changing state
  collision_sense();
  ambient_light_sense();
}


void ambient_light_sense(){
  ambient_light_analog = analogRead(A0);
  Serial.print("Analog reading: ");
  Serial.println(ambient_light_analog);
  if (ambient_light_analog > 7) {
    digitalWrite(8, LOW);
  } else if (ambient_light_analog < 7) {
    digitalWrite(8, HIGH);
  }
}

void collision_sense(){
  collision_value = analogRead(A2);
  if (collision_value >= collision_threshold) {
    no_collision = false;
    digitalWrite(brake_pin, HIGH); // brake lights
  } else {
    no_collision = true;
    digitalWrite(brake_pin, LOW); // brake lights
  }
}

void start_forward_motion(int rightwheel1, int rightwheel2, int leftwheel1, int leftwheel2, int analogright, int analogleft){
  analogWrite(rightwheel2, analogright);
  analogWrite(rightwheel1, 0);
  analogWrite(leftwheel2, analogleft);
  analogWrite(leftwheel1, 0);
}

void start_pivotleft(int rightwheel1, int rightwheel2, int leftwheel1, int leftwheel2, int analogright, int analogleft){
  analogWrite(rightwheel2, 0);
  analogWrite(rightwheel1, analogright);
  analogWrite(leftwheel2, analogleft);
  analogWrite(leftwheel1, 0);
}

void stop(int rightwheel1, int rightwheel2, int leftwheel1, int leftwheel2, int analogright, int analogleft){
  analogWrite(rightwheel1, 0);
  analogWrite(rightwheel2, 0);
  analogWrite(leftwheel2, 0);
  analogWrite(leftwheel1, 0); 
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
  for(i=0; i<150; i++){
    digitalWrite(pin, HIGH);
    delay(5);
    digitalWrite(pin, LOW);
    delay(5); 
  }
}


void headlights(int pin){
  int i = 0;
  for(i=0; i<2; i++){
    digitalWrite(pin, HIGH);
    delay(300);
    digitalWrite(pin, LOW);
    delay(500);
    digitalWrite(pin, HIGH);
    delay(300);
    digitalWrite(pin, LOW);
    delay(300); 
  }
}

void brake_lights(int pin){
  digitalWrite(pin, HIGH);
  delay(800);
  digitalWrite(pin, LOW);
}

// POST function doing actual call
void POSTServer(const char theRoute[], char *bodyMessage) {  
 if (client.connect(server, portNumber)) {

      Serial.println("POST: Connected to server");

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
  while(firstReading != -1) {
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
