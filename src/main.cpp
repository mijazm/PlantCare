#include <Arduino.h>
#include <plantcare.h>
#include "WiFi.h"
#include "ESPmDNS.h"
#include<PubSubClient.h>
#include<ArduinoJson.h>

#define MQTT_VERSION MQTT_VERSION_3_1_1


//Define your plants
Plant p1("Curry Leaves",39,36);


// WiFi credentials.
const char* WIFI_SSID = "MuAlpha";
const char* WIFI_PASS = ".hack//197228";


// MQTT: ID, server IP, port, username and password
const PROGMEM char* MQTT_CLIENT_ID = "PlantCare";
const PROGMEM uint16_t MQTT_SERVER_PORT = 1883;
const PROGMEM char* MQTT_USER = "homeassistant";
const PROGMEM char* MQTT_PASSWORD = "eiC3gier7ooMai9uWab0xah5choong5eeXohyiechaegahlohteengohguReis0Y";

const char *topic_telemetry_data = "plantcare/telemetry_data";

WiFiClient espClient;
PubSubClient client(espClient);
IPAddress HAserverIp;



void setup() {
  // put your setup code here, to run once:
  p1.setup_sensors();
  // pinMode(39,INPUT);
  Serial.begin(115200);


  // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.println("Connecting...");

    while (WiFi.status() != WL_CONNECTED) {
      // Check to see if connecting failed.
      // This is due to incorrect credentials
      if (WiFi.status() == WL_CONNECT_FAILED) {
        Serial.println("Failed to connect to WIFI. Please verify credentials");
      }
      delay(5000);
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());


//The folloing code finds the ip address of local HA installation using mDNS
  while(mdns_init()!= ESP_OK){
    delay(1000);
    Serial.println("Starting MDNS...");
  }
 
  Serial.println("MDNS started");
 
 
  while (HAserverIp.toString() == "0.0.0.0") {
    Serial.println("Resolving host...");
    delay(250);
    HAserverIp = MDNS.queryHost("homeassistant");
  }
 
  Serial.println("Home Assistant address resolved:");
  Serial.println(HAserverIp.toString()); 

  const char* MQTT_SERVER_IP = HAserverIp.toString().c_str();

   //Connecting to MQTT broker
   client.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);
   //TODO:Callback
   while(!client.connected()){
      if ( client.connect(MQTT_CLIENT_ID,MQTT_USER,MQTT_PASSWORD) ){
        Serial.println("Connected to MQTT broker on HA....");
      }
      else{
        Serial.println("Failed with state ");
        Serial.println (client.state());
        delay(2000);
      }
   }



}

void reconnect_wifi(){

    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.println("Trying to reconnect WiFi...");
    delay(10000);

    if (WiFi.status() != WL_CONNECTED) {
      // Check to see if connecting failed.
      // This is due to incorrect credentials
      if (WiFi.status() == WL_CONNECT_FAILED) {
        Serial.println("Failed to reconnect to WIFI. Please verify credentials");
      }
      delay(5000);
    }else{
      Serial.println("");
      Serial.println("WiFi reconnected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
    }

    

}


void reconnect_mqtt(){
  Serial.println("here/........");
  //The folloing code finds the ip address of local HA installation using mDNS
  while(mdns_init()!= ESP_OK){
    delay(1000);
    Serial.println("Starting MDNS...");
  }
 
  Serial.println("MDNS started");
 
 
  while (HAserverIp.toString() == "0.0.0.0") {
    Serial.println("Resolving host...");
    delay(250);
    HAserverIp = MDNS.queryHost("homeassistant");
  }
 
  Serial.println("Home Assistant address resolved:");
  Serial.println(HAserverIp.toString()); 

  const char* MQTT_SERVER_IP = HAserverIp.toString().c_str();

  //  Connecting to MQTT broker
   client.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);
  //  TODO:Callback

 

  if (client.connect(MQTT_CLIENT_ID,MQTT_USER,MQTT_PASSWORD) ){
    Serial.println("Reconnected to MQTT broker on HA....");
  }
  else{
    Serial.println("Failed with state ");
    Serial.println (client.state());
    delay(2000);
  }
   
}

void loop() {
  

  if(!WiFi.isConnected()){
    Serial.println("Lost WiFi Connection.Trying to reconnect...........");
    reconnect_wifi();
  }

  if(!client.connected()){
    Serial.println("Lost connection to MQTT server.Trying to reconnect............");
    reconnect_mqtt();
  }
  // Allocate the JSON document
  // Inside the brackets, 200 is the RAM allocated to this document.
  // Don't forget to change this value to match your requirement.
  // Use arduinojson.org/v6/assistant to compute the capacity.
  StaticJsonDocument<256> telemetry;

  auto p1_moisture = p1.get_soil_moisture();
  
  p1.monitor();
  

  //Telemetry 
  telemetry["p1_moisture"] = p1.soil_moisture;
  telemetry["p1_watering_state"] = p1.watering_state;
  

  char telemetryAsJson[256];
  serializeJson(telemetry, telemetryAsJson);

  client.publish(topic_telemetry_data, telemetryAsJson);

  // serializeJsonPretty(telemetry, Serial);
  Serial.println();

  Serial.print("Moisture:");
  Serial.println(p1_moisture);
  delay(2000);
}
