#include <ESP8266WiFi.h>      // Connect to a WiFi
#include <ESP8266WebServer.h> // Establish a webserver
#include <SPI.h>              // Handle the SPI communication
#include <MFRC522.h>          // For the MFRC522 NFC Reader Model



/***    MFRC522 Pins Definetions    ***/
#define SS  D8
#define RST D4


/***    Constant    ***/

//Set the WiFi SSID & Password
const char* WIFI_SSID = "Hesham";  
const char* WIFI_PASS = "01010606783"; 



/***    Global Variables    ***/
String id = "";

// A server object listen to port 80
ESP8266WebServer server(80);

// Create MFRC522 object
MFRC522 mfrc522(SS, RST);   



void setup() {
  /***    Set The Initial Configuration    ***/
  Serial.begin(9600); 
  SPI.begin(); 
  mfrc522.PCD_Init(); 

  // Connet to the WiFi
  connectToWiFi(WIFI_SSID, WIFI_PASS);

  // Print ESP8266 Local IP Address
  Serial.print("Local IP address: ");
  Serial.println(WiFi.localIP());
  delay(100);

  // Set the server routes
  server.on("/", handleOnConnect);
  server.on("/getCurrentID", sendCurrentID);
  server.onNotFound(handleNotFound);

  // Establish the server
  server.begin();
  Serial.println("HTTP server started");
}


void loop() {
  // Start the server listening
  server.handleClient();

  readTagID();
}


void connectToWiFi(const char* ssid, const char* password) {
  Serial.printf("\nConnecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n*************CONNECTED*************");
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


void readTagID(){
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();
}
