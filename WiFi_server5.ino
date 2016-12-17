#include <SPI.h>
#include <WiFi101.h>

char ssid[] = "";     //  your network SSID (name)
char pass[] = "";  // your network password
int status = WL_IDLE_STATUS;
int command = -1;
int times_sent = 0;
boolean alreadyConnected = false; // whether or not the client was connected previously

IPAddress ip(192, 168, 0, 177);
WiFiServer server(80);

void setup() {
  while(!Serial){;}
  
  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("Launch!");
  
  while ( status != WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    WiFi.config(ip);
    status = WiFi.begin(ssid, pass);

    // wait 1 second for connection:
    delay(1000);
  }
  Serial.println("Wifi connected succesfully!");
  printWifi();
  digitalWrite(LED_BUILTIN, HIGH);

  server.begin();
  Serial.print("Server is up and running! My address:");
  IPAddress myAddress = WiFi.localIP();
  Serial.println(myAddress);
}

void loop() {  
  // wait for a new client:
  WiFiClient client = server.available();

  if (client) {
    // Serial.println("connected! :)");
    // Read serial.
    if (Serial.available()>0 && command==-1){
      command = Serial.read();
      // Serial.println("Serial command recieved!");
    }
    // Send command if I have one.
    if (command>-1){
      server.write(command);
      // Serial.print("Command sent to someone:");
      // Serial.println(command);
      times_sent++;
      if (times_sent >= 2){
        command = -1;
        times_sent = 0;
      }
    }

    delay(100);
    
    // Read something if she has something to say.
    if (client.available() > 0) {
      // read the bytes incoming from the client:
      char thisChar = client.read();
      Serial.print("Recieved from someone: ");
      Serial.write(thisChar);
      command = int(thisChar);
      times_sent = 1;
      Serial.println("");
    }
    client.stop();
  }
}



void loop_old() {
  int message = -1;
  int counter = 0;
  while (server.available() != true) {
    //server.write(client.read());
    if (counter == 0){
      Serial.println("Waiting for client...");
      counter = 1;
    }
  }
  WiFiClient client = server.available();
  client.flush();
  Serial.println("Client found!");
  //server.write("Hello client!");
  while(message<0){
    message = client.read();
  }
  Serial.println(message);
  //delay(1000);

  Serial.println("Cutting off client");
  client.stop();
  /*
  if (client == true) {
    //server.write(client.read());
    server.write("Hello client!");
  }
  */
  //delay(5);
  printWifi();
}

void printWifi(){
  IPAddress ip = WiFi.localIP();
  Serial.println("");
  Serial.print("IP: ");
  Serial.println(ip);
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);
}

