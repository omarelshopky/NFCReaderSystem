#include <ESP8266WiFi.h>      // Connect to a WiFi
#include <ESP8266WebServer.h> // Establish a webserver
#include <SPI.h>              // Handle the SPI communication
#include <MFRC522.h>          // For the MFRC522 NFC Reader Model
#include <cppQueue.h>


/***    MFRC522 Pins Definetions    ***/
#define SS  D8
#define RST D4

#define QUEUE_SIZE 10



/***    Constant    ***/

//Set the WiFi SSID & Password
const char* WIFI_SSID = "WIFI_NAME";  
const char* WIFI_PASS = "WIFI_PASSWORD"; 


/***    Global Variables    ***/
cppQueue idsQue(sizeof(String), QUEUE_SIZE);

// A server object listen to port 80
ESP8266WebServer server(80);

// Create MFRC522 object
MFRC522 mfrc522(SS, RST);   

// Configure the network connection
IPAddress local_IP(192, 168, 1, 145);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);


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

  String currentId = readTagID();
  
  if (currentId != "" && !inIdsQueue(currentId)) {
      idsQue.push(&currentId);
  }
  
  inIdsQueue(""); // Print queue values
  delay(200);
}


void connectToWiFi(const char* ssid, const char* password) {

  // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }
  
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
  String id = "";
  idsQue.pop(&id);
  server.send(200, "text/plain", id);
}


String readTagID() {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) return "";
  
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) return "";

  // Read tag UID
  String uid = "";
  
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     uid += mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""; 
     uid += String(mfrc522.uid.uidByte[i], HEX);
  }
  
  Serial.println("The Tag UID: " + uid);
  return uid;
}


bool inIdsQueue(String id) {
  Serial.println("---------------");
  for (int i = 0; i < QUEUE_SIZE; i++) {
    String tmpId = "";
    
    if(idsQue.peekIdx(&tmpId, i)) {
      
      Serial.println(tmpId);
      
      if (id == tmpId)
        return true; 
    } 
  }

  return false;
}
