#include <SPI.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"

#define COMMAND_ARRAY_SIZE 20
#define PARAMETER_ARRAY_SIZE 40
#define CHAR_ARRAY_LENGTH 25
#define MSGDATA_SIZE 255
#define VALUEARRAY_SIZE 15

enum State {YELLOW, RED, BLUE, DARK};
State color = DARK;
State prev = DARK;

bool tracking_bool = true;
bool collision_absence = true;

int pin2 = 2; //horn
int pin4 = 4; //headlights
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

int counter = 0;

int value1 = 0;
int value2 = 0;
int value3 = 0;
int value4 = 0;

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


// int LED = 7;
// int LED2 = 6;
int val = 0;
int pin11 = 11;

bool is_on = false;

void setup() {
  Serial.begin(9600);
  pinMode(pin3, OUTPUT);
  digitalWrite(pin3, HIGH); // power white LED

  

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
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  // pinMode(LED, OUTPUT);
  // pinMode(LED2, OUTPUT);
  pinMode(pin11, INPUT_PULLUP);
  // digitalWrite(LED, HIGH);
  // digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW


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
  char postBody[] = "duration=0";
  char postRoute[] = "POST /F79721857DC5/89C87865077A HTTP/1.1";
  POSTServer(postRoute, postBody);
}

void loop() {
  val = digitalRead(pin11);
  Serial.println(val);// wait for a second
  if (digitalRead(pin11) == LOW) {
    if (!is_on) {
      pivotleft(pin5, pin6, pin10, pin9, power, power);
      horn(pin2);
      // ********************** POST ***********************
      Serial.println("Doing POST");
      char postBody[] = "duration=5";
      // format of postRoute: "POST /senderID/receiverID HTTP/1.1"
      char postRoute[] = "POST /F79721857DC5/89C87865077A HTTP/1.1";
      POSTServer(postRoute, postBody);      
      is_on = true;
    }
    

  } else if (digitalRead(pin11) == HIGH) {
      stop(pin5, pin6, pin10, pin9, power, power, 1000);
      // ********************** POST ***********************
      Serial.println("Doing POST");
      char postBody[] = "duration=0";
      // format of postRoute: "POST /senderID/receiverID HTTP/1.1"
      char postRoute[] = "POST /F79721857DC5/89C87865077A HTTP/1.1";
      POSTServer(postRoute, postBody);
      is_on = false;
    Serial.println("Switch is OFF");
  }
  
  if (!client.connected()) {
    Serial.println("disconnecting from server.");
    client.stop();
    while (true);
  }
  //delay(150);
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

void pivotleft(int rightwheel1, int rightwheel2, int leftwheel1, int leftwheel2, int analogright, int analogleft){
  
  analogWrite(rightwheel2, 0);
  analogWrite(rightwheel1, analogright);
  analogWrite(leftwheel2, analogleft);
  analogWrite(leftwheel1, 0);
}

void stop(int rightwheel1, int rightwheel2, int leftwheel1, int leftwheel2, int analogright, int analogleft, int period){
  analogWrite(rightwheel1, 0);
  analogWrite(rightwheel2, 0);
  analogWrite(leftwheel2, 0);
  analogWrite(leftwheel1, 0); 
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
