#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ESPmDNS.h> 
#include <DHT.h>
#include "secret.h"

#define LED 2
#define DHTPIN 23     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);
// create an instance of WiFiClientSecure 
WiFiClientSecure espClient;
PubSubClient client(espClient);

void callback(char *topic, byte *payload, unsigned int length){
  Serial.print("Message arrived on topic \"");
  Serial.print(topic);
  Serial.print("\": ");
  String messageText = "";
  for (int i = 0; i < length; i++)
  {
    messageText.concat((char)payload[i]);
  }
  Serial.println(messageText);

  if(messageText.substring(0,11) != MQTT_CLIENT_ID){
    return;
  }
  
  // Sensor waardes kunnen 2 seconden oud zijn en uitlezen duurt 250 ms
  delay(2000);
  
  // Sensorwaardes uitlezen
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    client.publish("chat/message", "Error: DHT Sensor van BOT-1030197 kan niet uitgelezen worden.");
    return;
  }
  if(messageText.substring(13) == "vochtigheid"){
    String vochtigheid = "Vochtigheid is " + String(h) + "%";
    client.publish("chat/message", vochtigheid.c_str());
  }
  if(messageText.substring(13) == "temperatuur"){
    String temperatuur = "Temperatuur is " + String(t) + "°C";
    client.publish("chat/message", temperatuur.c_str());
  }

  if(messageText.substring(13) == "led:aan"){
    client.publish("chat/message", "LED is aan");
    digitalWrite(LED, 1);
  }
  if(messageText.substring(13) == "led:uit"){
    client.publish("chat/message", "LED is uit");
    digitalWrite(LED, 0);
  }
}

void connect(){
  // Loop until connected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client";
    // Attempt to connect
    if (client.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASS))
    {
      Serial.println("Attempting to login with username and pwd");
      client.subscribe("chat/message", 0);
    }
    else
    {
      Serial.print("failed, status code =");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup(){
  Serial.begin(115200);

  network_setup();
  
  dht.begin();
  
  pinMode(LED, OUTPUT);
  digitalWrite(LED, 0);
}

void network_setup() {
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // set SSL/TLS certificate
  espClient.setCACert(local_root_ca);

  // configure the MQTT server with IPaddress and port 
  client.setServer(MQTT_HOST, MQTT_PORT);

  if (client.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASS)){
    Serial.println("Sent user and passwd");
    client.subscribe("chat/message", 0);
    client.publish("chat/message", "BOT-1030197 joined the chat");
  }

  // this receivedCallback function will be invoked when client received subscribed topic 
  client.setCallback(callback);
}

void loop(){
  // if client was disconnected then try to reconnect again 
  if (!client.connected()){
    connect();
  }
  
  // this function will listen for incomming subscribed topic-process-invoke receivedCallback 
  client.loop();
};
