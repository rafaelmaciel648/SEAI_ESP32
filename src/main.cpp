#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "MEO-Casa";
const char* pass = "2BC2355B2C";

const char* clientID = "FEUP_SEAI";
const char* brokerUser = "jbeleza.tr@gmail.com";
const char* brokerPass = "6ec21d8d";
const char* broker = "mqtt.dioty.co";
const uint16_t port = 1883;
const char* outTopic = "/jbeleza.tr@gmail.com/out";
const char* inTopic = "/jbeleza.tr@gmail.com/in";

WiFiClient espClient;
PubSubClient client(espClient);
char messages[50];
int i=0;

void setupWifi(){
  delay(100);
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while(WiFi.status() != WL_CONNECTED){
    delay(100);
    Serial.print("-");
  }

  Serial.print("\nConnected to ");
  Serial.println(ssid);
}

void reconnect(){
  while(!client.connected()){
    Serial.print("\nConnecting to ");
    Serial.println(broker);
    if(client.connect(clientID, brokerUser, brokerPass)){
      Serial.print("\nConnected to ");
      Serial.println(broker);
      client.subscribe(inTopic);
    } else{
      Serial.println("\nTrying to connect again");
      delay(5000);
    }
  }
}

//Send Data (Publisher)
void publish(int value){
  client.loop();
  snprintf(messages, 75, "Value: %ld", value);
  Serial.println("Sending: ");
  Serial.println(messages);
  client.publish(outTopic, messages);
}

//Receive Data (Subscriber)
void callback(char* topic, byte* payload, unsigned int length){
  Serial.print("Received message: ");
  Serial.println(topic);
  for(int i=0; i<length; i++){
    Serial.print((char) payload[i]);
  }
  Serial.println();
}

//Setup MQTT
void setupmqtt(){
  client.setServer(broker, port);
  client.setCallback(callback);
}

void setup() {
  delay(10000);
  Serial.begin(115200);
  setupWifi();
  setupmqtt();
  delay(1000);
}

void loop() {
  if(!client.connected()){
    reconnect();
  }
  publish(i);
  i++;
  delay(1000);
}