#include <SPI.h>
#include <Time.h>
// From https://github.com/brandenhall/Arduino-Websocket

#define DEBUGGING 1

// Here we define a maximum framelength to 64 bytes. Default is 256.
#define MAX_FRAME_LENGTH 64

// Define how many callback functions you have. Default is 1.
#define CALLBACK_FUNCTIONS 1

#include <WebSocketClient.h>
// ISN'T THIS A PROBLEM THAT IT'S THE SAME WEBSOCKETCLIENT.H AS THE PREVIOUS ONE???
// Nope. changed the old one to WebSocketClient.h-old.h

WebSocketClient webSocketClient;

#include <WiFi101.h>
char ssid[] = "ssid";     //  your network SSID (name)
char pass[] = "pass";  // your network password
int status = WL_IDLE_STATUS;
const int LED = 6;
const int BUTTON = 7;
String sendData = "turn_on";
int last_time_pinged = now();

WiFiClient client;

void setup() {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT);

  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);

  setup_wifi();
  setup_websocket();
}

void loop() {
  String data;
  int gotping = 1;
  int message_recieved = 0;
  int button = 0;
  int minutes = 5;

  if (client.connected()) {
    
    if (gotping) {
      
      // get data from server and read button
      message_recieved = webSocketClient.getData(data);
      if (message_recieved && data == "ping") {
        gotping = 1;
        last_time_pinged = now();
        message_recieved = 0;
      } else {
        ;//extract_time(data);
      }
      if (message_recieved){
        last_time_pinged = now();
      }

      // Get button data
      button = digitalRead(BUTTON);
      
      if (button == HIGH || message_recieved) {
        if (button == HIGH) {
          webSocketClient.sendData(sendData);
        }
        Serial.println(data);
        digitalWrite(LED, HIGH);
        delay(3000);
        digitalWrite(LED, LOW);
      }
    } else {
      setup_websocket();
      /*
        while (!setup_websocket()|| !gotping) {
        ;
        }
        gotping = 1;
      */
      gotping = 1;
    }
  } else {
    setup_wifi();
    gotping = 0;
    gotping = setup_websocket();
  }

  // Check how long ago I was pinged last: If more than 2 minutes then reset in next round
  if (last_time_pinged-now()>120){
    gotping = 0;
  }
}

/*
int extract_time(char data) {
  auto const pos = data.find_last_of('|');
  const auto leaf = data.substr(pos + 1);
  return atoi(leaf);
}
*/

void printWifi() {
  IPAddress ip = WiFi.localIP();
  Serial.println("");
  Serial.print("IP: ");
  Serial.println(ip);
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);
}

bool setup_wifi() {
  while ( status != WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 1 second for connection:
    delay(1000);
  }
  Serial.println("Wifi connected succesfully!");
  printWifi();
  return 1;
}

bool setup_websocket() {
  // Connect to the websocket server
  // if (client.connect("echo.websocket.org", 80)) {
  // if (client.connect("lightsense-api.herokuapp.com", 80)) {

  if (client.connect("192.168.2.135", 3000)) {
    Serial.println("Connected");
  } else {
    Serial.println("Connection failed.");
    return 0;
    /* 
     while (1) {
      // Hang on failure
    }
    */
  }

  // Handshake with the server
  //webSocketClient.path = "/qZ9ye7JhQgeZCDQ9tQWTgXPGwy7EQaGsjSSC";
  //webSocketClient.host = "lightsense-api.herokuapp.com";
  webSocketClient.path = "/secret";
  webSocketClient.host = "192.168.2.105";
  //webSocketClient.protocol = "wss";

  if (webSocketClient.handshake(client)) {
    Serial.println("Handshake successful");
    return 1;
  } else {
    Serial.println("Handshake failed.");
    return 0;
    /*while (1) {
      // Hang on failure
      }*/
  }
  /*
    if (client.connected()) {
    webSocketClient.sendData(sendData);
    Serial.println("Data sent!");
    }
  */
}


void trash() {
  String data;
  if (digitalRead(BUTTON) == HIGH) {
    if (webSocketClient.handshake(client)) {
      webSocketClient.sendData(sendData);
    }
    digitalWrite(LED, HIGH);
    delay(3000);
    digitalWrite(LED, LOW);
  }

  if (client.connected()) {

    //data = webSocketClient.getData();
    int resulting = webSocketClient.getData(data);

    if (data.length() > 0) {
      Serial.print("Received data: ");
      Serial.println(data);
      Serial.println(resulting);
    }

  } else {

    Serial.println("Client disconnected.");
    while (1) {
      // Hang on disconnect.
    }
  }

  // wait to fully let the client disconnect
  delay(3000);
}
