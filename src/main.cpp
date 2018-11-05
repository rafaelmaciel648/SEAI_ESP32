


#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <esp_wifi.h>

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  10       /* Time ESP32 will go to sleep (in seconds) */

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
RTC_DATA_ATTR int i=0;

void setupWifi(){
  delay(100);
  Serial.print("\nConnecting to wifi: " + String(ssid) + " ");

  WiFi.begin(ssid, pass);

  while(WiFi.status() != WL_CONNECTED){
    delay(100);
    Serial.print(".");
  }

  Serial.println(" Connected.");
}

void reconnect(){
  while(!client.connected()){
    Serial.print("Connecting to " + String(broker));
    if(client.connect(clientID, brokerUser, brokerPass)){
      Serial.println(": Connected.");
      //client.subscribe(inTopic);
    } else{
      Serial.println(": [Error] Not connected: " + String(client.state()) + " Wait 5 seconds before retry.");
      delay(5000);
    }
  }
}

//Send Data (Publisher)
void publish(const char* topic, char* m){
  char messages[strlen(m)];
  snprintf(messages, (strlen(m)+1) , m);
  Serial.println("Sending: " + String(messages);
  client.publish(topic, messages);
}

/* void publish(int value){
  char messages[50];
  snprintf(messages, 75, "Value: %d", value);
  Serial.print("Sending: ");
  Serial.println(messages);
  client.publish(outTopic, messages);
} */

//Receive Data (Subscriber)
/* void callback(char* topic, byte* payload, unsigned int length){
  Serial.print("Received message: ");
  Serial.println(topic);
  for(int i=0; i<length; i++){
    Serial.print((char) payload[i]);
  }
  Serial.println();
} */

//Setup MQTT
void setupmqtt(){
  client.setServer(broker, port);
  //client.setCallback(callback);
}

void setup() {
  Serial.begin(115200);
  setupWifi();
  setupmqtt();
  delay(100);

  if(!client.connected()){
    reconnect();
    //client.loop();
  }

  // FUNCTION TO READ SENSORS DATA

  // PUBLISH DATA DO MQTT SERVER
  //--------------------------------------------------------------------------------
  char* mes = "Hello World!";
  publish(outTopic, mes); // publish message in specific topic
  delay(3000);  // delay is needed for server accept published message
  //--------------------------------------------------------------------------------

  esp_wifi_stop();

  // DEEP SLEEP MODE SETUP AND START
  //--------------------------------------------------------------------------------
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);  // set wakeup time
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) + " Seconds");

  Serial.println("Going to sleep now");
  Serial.flush();         // wait for finish serial prints
  esp_deep_sleep_start(); // entry in deep sleep mode
  //--------------------------------------------------------------------------------
  Serial.println("This will never be printed");
}

void loop() {
}