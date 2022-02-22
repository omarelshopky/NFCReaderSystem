#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>


/*** Set the WiFi SSID & Password ***/
const char* ssid = "";  
const char* password = ""; 

/*** Set a server listen to port 80 ***/
ESP8266WebServer server(80);


/***    Global Variables    ***/
String id = "";


void setup() {
  Serial.begin(115200);
  
  connectToWiFi(ssid, password);
  
  // Print ESP8266 Local IP Address
  Serial.print("Local IP address: ");
  Serial.println(WiFi.localIP());
  delay(100);
  
  server.on("/", handleOnConnect);
  server.on("/getCurrentID", sendCurrentID);
  server.onNotFound(handleNotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}


void loop() {
  // Start the server listening
  server.handleClient();
}


void connectToWiFi(const char* ssid, const char* password) {
  Serial.printf("\nConnecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("*************CONNECTED*************");
}


void handleOnConnect() {
  server.send(200, "text/plain", "Connected"); 
}


void handleNotFound(){
  server.send(404, "text/plain", "Not found");
}


void sendCurrentID() {
  server.send(200, "text/plain", id);
}
