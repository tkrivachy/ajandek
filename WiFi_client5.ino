#include <SPI.h>
#include <WiFi101.h>

char ssid[] = "";     //  your network SSID (name)
char pass[] = "";  // your network password
int status = WL_IDLE_STATUS;

int command = -1;

IPAddress server(192, 168, 0, 177);

//IPAddress ip(192, 168, 0, 177);
//WiFiServer server(80);
WiFiClient client;


void setup() {
  while(!Serial){;}
  
  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("Launch!");
  
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
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.print("My client address:");
  IPAddress myAddress = WiFi.localIP();
  Serial.println(myAddress);
}


void loop() {
  delay(3000);
  // Read my input.
  if (Serial.available()>0 && command==-1){
    command = Serial.read();
    // Serial.println("Serial command recieved!");
  }
  // Connect to server.
  if (!client.connected()){
    // Serial.println("Trying to connect");
    client.connect(server, 80);
    client.flush();
  }
  if (client.connected()) {
    // Serial.println("connected! :)");

    // If I have command to send, send it.
    // SZTEM AZ A BAJ HOGY ELKULDOM, DE UTANA ELVAG A SZERVER, ES CSAK KOVI CIKLUSBAN OLVASNA KI...
    if (command>-1) {
      client.write(command);
      // Serial.print("Command sent to server: ");
      Serial.println(command);
      command = -1;
    }

    // If she has something to say, read it.
    if (client.available() > 0){
      char thisChar = client.read();
      Serial.print("Recieved this from server: ");
      Serial.write(thisChar);
      Serial.println("");
    }
  }
}


void loop_old() {
  // SOME ERROR HANDELING STILL NEEDED! e.g. if server restarts...
  // SERIAL
  if (Serial.available()>0){
    command = Serial.read();
    if (command>-1){
      if (client.connect(server, 80)) {
        Serial.println("connected! :)");
        client.write(command);
      }
    }
  }
  
// CLIENT - only connect on demand and transmit information!
      /* THIS PART TO READ FROM SERVER
       if (client.available()) {
        //char c = client.read();
        //Serial.print(c);
  */
  delay(5); 
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

