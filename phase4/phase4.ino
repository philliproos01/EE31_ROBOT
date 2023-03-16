#include <SPI.h>
#include <WiFiNINA.h>

#include "arduino_secrets.h"
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
int keyIndex = 0;
char ERYTHAEAN[] = "F79721857DC5";
char GREIGE[] = "89C87865077A";
const int MSGDATA_SIZE = 64;

int status = WL_IDLE_STATUS;

char server[] = "ee31.ece.tufts.edu"; // for Tufts
int portNumber = 80; // for Tufts
int counter = 0;
// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):

WiFiClient client;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }

  Serial.println("Connected to wifi");
  printWifiStatus();
  Serial.println("\nStarting connection to server...");
  // format of post parameters to be sent
  

  
  // Serial.println(result);
}

void loop() {
  if (counter < 1) {
    char postBody[] = "variable1=h&variable2=e&variable3=l&variable4=l&variable5=o";
    // format of postRoute: "POST /senderID/receiverID HTTP/1.1"
    char postRoute[] = "POST /F79721857DC5/F79721857DC5 HTTP/1.1";
    // function call to make a post command
    POSTServer(postRoute, postBody);
    // if there are incoming bytes available
    // from the server, read them and print them:
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }    

    // message data array size plus one for null '\0' at end of C-string
    static char messageData[MSGDATA_SIZE + 1];
    // format of gettRoute: "GET /senderID/receiverID HTTP/1.1"
    char getRoute[] = "GET /F79721857DC5/F79721857DC5 HTTP/1.1"; //ERYTHREAEN/GREIGE
    // sender GETs messageData from receiver
    GETServer(getRoute, messageData);
    // if there are incoming bytes available
    // from the server, read them and print them:
    
    counter++;
  }

  
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }    
  
  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println("disconnecting from server.");
    client.stop();
    // do nothing forevermore:
    while (true);

  }
}

// POST function call -- post message from sender to the receiver
void POSTServer(const char theRoute[], char *bodyMessage) {
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

// GET function call -- get message to sender from the receiver
void GETServer(const char theRoute[], char *message) {
  if (client.connect(server, portNumber)) {
    // Make a HTTP GET request:
    client.println(theRoute);
    client.print("Host: ");
    client.println(server);
    client.println("Connection: close");
    client.println();

    client.read(message, MSGDATA_SIZE);
    char* result = GetMessageBody(message); // get the message from the server
    client.println(result);    
    
    // place in appropriate form and variable
    // if the server's disconnected, stop the client:
    if (!client.connected()) {
      client.stop();
    }
  }
}

char *GetMessageBody(char *message) {
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  String currentLine = "";                // make a String to hold incoming data from the client
  while (client.connected()) {            // loop while the client's connected
    if (client.available()) {             // if there's bytes to read from the client,
      char c = client.read();             // read a byte, then
      // Serial.println(c);                    // print it out the serial monitor
      if (c == '\n') {                    // if the byte is a newline character
        // if the current line is blank, you got two newline characters in a row.
        // that's the end of the client HTTP request, so send a response:
        if (currentLine.length() == 0) {
          // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
          // and a content-type so the client knows what's coming, then a blank line:
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          // client.println();
          // the content of the HTTP response follows the header:
          // client.print("Click <a href=\"/H\">here</a> turn the LED on<br>");
          // client.print("Click <a href=\"/L\">here</a> turn the LED off<br>");
          // The HTTP response ends with another blank line:
          client.println();
          // break out of the while loop:
          break;
        } else {      // if you got a newline, then clear currentLine:
          currentLine.toCharArray(message, MSGDATA_SIZE);
          Serial.println(currentLine);
          char *result = parseLine(message);
          currentLine = "";
        }
      } else if (c != '\r') {    // if you got anything else but a carriage return character,
        currentLine += c;      // add it to the end of the currentLine
      }
    }
  }
  return message;
}

char *parseLine(char* message) {
  char *senderID;
  char *receiverID;
  char *message_returned;
  char *result = (char*) malloc(sizeof(char));
  result[0] = '\0';

  char *splitted = strtok(message, "&");
  while (splitted != NULL) {
    char *name = strtok(splitted, "=");
    char *value = strtok(NULL, "=");
    if (strcmp(name, "senderID") == 0) {
      senderID = value;
    } else if (strcmp(name, "receiverID") == 0) {
      receiverID = value;
    } else if (strcmp(name, "message") == 0) {
      message_returned = value;
    } else if (strcmp(name, "senderID") != 0 && strcmp(name, "receiverID") != 0 && strcmp(name, "message") != 0) {
      strcat(result, value);
    }
    splitted = strtok(NULL, "&");
  }
  return result;
}

void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  long rssi = WiFi.RSSI();

  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}