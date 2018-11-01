#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>


WiFiClient espClient;
//MQTT
const char* brokerUser = "jbeleza.tr@gmail.com";
const char* brokerPass = "6ec21d8d";
const char* broker = "mqtt.dioty.co";
const uint16_t port = 1883;
const char* outTopic = "/jbeleza.tr@gmail.com/out";
const char* inTopic = "/jbeleza.tr@gmail.com/in";
PubSubClient client(espClient);

//Setup MQTT
void setupmqqt(){
    client.setServer(broker, port);
    client.setCallback(callback);
}

//Check Mqtt Connection
void check_mqtt_conn(){
  while(!client.connected()){
    Serial.print("\nConnecting to");
    Serial.println(broker);
    if(client.connect("koikoikoi", brokerUser, brokerPass)){
      Serial.print("\nConnected to");
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
  char messages[50];
  client.loop();
  snprintf(messages, 75, "Value: %ld", value);
  client.publish(outTopic, messages);
}

void setupWifi(){
  const char* ssid = "MEO-Casa";
  const char* pass = "2BC2355B2C";
  delay(100);
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while(WiFi.status() != WL_CONNECTED){
    delay(100);
    Serial.print("-");
  }

  Serial.print("\nConnected");
  Serial.print(ssid);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  setupWifi();
  delay(1000);
}

void loop() {
  if(!client.connected()){
    check_mqtt_conn()
  }
}