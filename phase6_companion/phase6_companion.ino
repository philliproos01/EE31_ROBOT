#include <SPI.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"

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

void setup() {
  Serial.begin(9600);
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
}

void loop() {
  // clears the message data
  memset(messageData, 0, sizeof(messageData));
  memset(valuesArray, 0, 15);
  
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
  
  if (valuesArray[3] == '0') {
    Serial.println("WAIT");
  } else if (valuesArray[3] == '5') {
    Serial.println("MOVE");
  }
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