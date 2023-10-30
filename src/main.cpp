#include <WiFi.h>
#include <SonosUPnP.h>    
#include "arduino_secrets.h" 

char ssid[] = SECRET_SSID;       
char pass[] = SECRET_PASS;    
int status = WL_IDLE_STATUS;  

// Setup Sonos Variables and settings
#define MAXSONOS 6
WiFiClient G_Sonosclient;                               // setup wifiClient for Sonos Control
SonosUPnP G_Sonos = SonosUPnP(G_Sonosclient);           // Setup SonosUpnp Device G_Sonos
IPAddress ACTIVE_sonosIP,G_SonosIPList[MAXSONOS]  ; 

String inputString = "";      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


    //Try to connect to Wifi network:
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, pass);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());


  // Setup Sonos connections
  //G_Sonos.CheckUPnP(G_SonosIPList,MAXSONOS);
   if ( G_Sonos.CheckUPnP(G_SonosIPList,MAXSONOS)!=0 )                            // check Sonos IP's on network , short scan, scan twice for large networks
   {
    ACTIVE_sonosIP = G_SonosIPList[0];
    Serial.print("\nActive Sonos ");Serial.println(ACTIVE_sonosIP);
   }
   else {
    Serial.println("\nNo UPnP found.");while(1);
   }
}

void loop() {

  char text[128];
  FullTrackInfo info;
  
   info= G_Sonos.getFullTrackInfo(ACTIVE_sonosIP); // Rest full track info
   Serial.println();Serial.print(info.album);
   Serial.print(" - ");Serial.println(info.creator);   Serial.print(info.number);
   Serial.print(". ");Serial.print(info.title);   Serial.print("  ");Serial.println(info.position);
   
  delay(5000);

}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;

    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}




