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
WiFiClient client;

void setup() {
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
    // wait 1 second for connection:
    delay(1000);
  }
  Serial.println("Connected to wifi");
  printWifiStatus();
  Serial.println("\nStarting connection to server...");
}

void loop() {
  if (counter < 1) {
    char postBody[] = "variable1=h&variable2=e&variable3=l&variable4=l&variable5=o";
    // format of postRoute: "POST /senderID/receiverID HTTP/1.1"
    char postRoute[] = "POST /F79721857DC5/F79721857DC5 HTTP/1.1";
    POSTServer(postRoute, postBody);

    // format of gettRoute: "GET /senderID/receiverID HTTP/1.1"
    char getRoute[] = "GET /F79721857DC5/F79721857DC5 HTTP/1.1";
    // message data array size plus one for null '\0' at end of C-string
    static char messageData[MSGDATA_SIZE + 1];
    // sender GETs messageData from receiver
    GETServer(getRoute, messageData);
    counter++;
  }
    if (!client.connected()) {
    Serial.println("disconnecting from server.");
    client.stop();
    while (true);
  }
}

void POSTServer(const char theRoute[], char *bodyMessage) {
  if (client.connect(server, portNumber)) {
    client.println(theRoute);
    client.print("Host: ");
    client.println(server);
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    int postBodyLength = strlen(bodyMessage);
    client.println(postBodyLength);
    client.println();
    client.println(bodyMessage);
  }
}

void GETServer(const char theRoute[], char *message) {
  if (client.connect(server, portNumber)) {
    // Make a HTTP GET request:
    client.println(theRoute);
    client.print("Host: ");
    client.println(server);
    client.println("Connection: close");
    client.println();
    delay(150);
    char* result = GetMessageBody(message); // get the message from the server
    Serial.println("result:");
    Serial.println(result);

    if (!client.connected()) {
      client.stop();
    }
  }
}

char *GetMessageBody(char *message) {
  String currentLine = "";
  while (client.available()) {
    char c = client.read();
    if (c == '\n') {
      if (currentLine.startsWith("senderID")) {
      char *buffer = (char*) malloc(sizeof(char)*(currentLine.length() + 1));
      buffer[currentLine.length()] = '\0';
      currentLine.toCharArray(buffer, currentLine.length() + 1);
      char * result = parseLine(buffer);
      return result;
    }
      currentLine = "";
    } else if (c != '\r') {
      currentLine += c;
    }
  }
  if (currentLine.startsWith("senderID")) {
    char *buffer = (char*) malloc(sizeof(char)*(currentLine.length()+1));
    buffer[currentLine.length()] = '\0';
    currentLine.toCharArray(buffer, currentLine.length() + 1);
    char * result = parseLine(buffer);
    message = result;
    return result;
  }
  return "";
}

char *parseLine(char* message) {
  // Define a struct to store the key-value pairs
  typedef struct {
    char* key;
    char* value;
  } KeyValuePair;

  // Define an array to store the key-value pairs
  KeyValuePair keyValuePairs[10];
  int numPairs = 0;

  // Split the input string by "&" to separate the key-value pairs
  char* pair = strtok(message, "&");
  while (pair != NULL && numPairs < 10) {
    // Split each key-value pair by "=" to separate the key and value
    int counter = 0;
    int seen_equal = false;
    for (int i = 0; i < strlen(pair); i++) {
      if (pair[i] == '=') {
        seen_equal = true;
      }
      if (!seen_equal) {
        counter++;
      }
    }
    char* key = (char*) malloc(sizeof(char));
    key[0] = '\0';
    for (int i = 0; i < counter; i++) {
      char* new_str = (char*) malloc(sizeof(char)*2);
      new_str[0] = pair[i];
      new_str[1] = '\0';      
      strcat(key, new_str);
    }

    char* value = (char*) malloc(sizeof(char));
    value[0] = '\0';
    for (int i = counter + 1; i < strlen(pair); i++) {
      char* new_str = (char*) malloc(sizeof(char)*2);
      new_str[0] = pair[i];
      new_str[1] = '\0';
      strcat(value, new_str);
    }

    // Add the key-value pair to the array
    keyValuePairs[numPairs].key = key;
    keyValuePairs[numPairs].value = value;
    numPairs++;

    // Move to the next key-value pair
    pair = strtok(NULL, "&");
  }
  char *result = (char*) malloc(sizeof(char));
  result[0] = '\0';
  // Print out the parsed values for testing
  for (int i = 3; i < numPairs; i++) {
    char* val = (char*) malloc(sizeof(char)*(strlen(keyValuePairs[i].value) + 1));
    val[(strlen(keyValuePairs[i].value))] = '\0';
    for (int j = 0; j < (strlen(keyValuePairs[i].value)); j++) {
      val[j] = keyValuePairs[i].value[j];
    }
    strcat(result, (char*)val);
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