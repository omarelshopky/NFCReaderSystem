#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>


/* Set the WiFi SSID & Password */
const char* ssid = "";  
const char* password = ""; 

// Set a server listen to port 80
ESP8266WebServer server(80);



void setup() {
  Serial.begin(115200);
  
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");

  // Print ESP8266 Local IP Address
  Serial.print("Local IP address: ");
  Serial.println(WiFi.localIP());
  delay(100);
  
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}
void loop() {

  // Start the server listening
  server.handleClient();
}

void handle_OnConnect() {
  server.send(200, "text/html", "Connected"); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}
