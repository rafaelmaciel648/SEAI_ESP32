


#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <esp_wifi.h>
#include <IPAddress.h>

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  10       /* Time ESP32 will go to sleep (in seconds) */

#define winepress_ID 123   // wine press ID between 1-4
#define module_ID 123        // sensor module ID between 1-3

// SENSORS INPUTS
#define TURB_PIN 36   //ADC1_CH0
#define TEMP_PIN 37   //ADC1_CH1
#define DENS_PIN 38   //ADC1_CH2

const char* ssid = "MEO-Casa";
const char* pass = "2BC2355B2C";

const char* clientID = "FEUP_SEAI";
WiFiClient espClient;
PubSubClient client(espClient);

//Broker Test
const char* brokerUser = "jbeleza.tr@gmail.com";
const char* brokerPass = "6ec21d8d";
const char* broker = "mqtt.dioty.co";
const char* broker_ip = "mqtt.dioty.co";
const uint16_t port = 1883;
const char* outTopic = "/jbeleza.tr@gmail.com/out";

//Broker Raspberry MQTT Server
/* const char* broker_ip = "192.168.43.64";
IPAddress broker(192,168,43,64);
//IPAddress broker = IPAddress().fromString(broker_ip);
const uint16_t port = 1883;
const char* outTopic = "topic"; */

IPAddress esp_ip;

// FUNCTIONS
void setupWifi();
void reconnect();
void publish(const char* topic, char* m);
void setupmqtt();
uint16_t temp_read();
uint16_t turb_read();
uint16_t dens_read();

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
  String mes = String("Press ID: " + String(winepress_ID) + "; Module ID: " + String(module_ID) + "; Turb: " + String(turb_read()) + ";\n\n");
  char data[(mes.length())+1];
  mes.toCharArray(data,mes.length() + 1);
  publish(outTopic, data); // publish message in specific topic
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

/*
 void setupWifi()
 Function: Connect to wifi network;
 Parameters: ----------------
*/
void setupWifi(){
  delay(100);
  Serial.print("\nConnecting to wifi: " + String(ssid) + " ");

  WiFi.begin(ssid, pass);

  while(WiFi.status() != WL_CONNECTED){
    delay(100);
    Serial.print(".");
  }

  esp_ip = WiFi.localIP();
  Serial.print(" Connected. ESP32 IP: ");
  Serial.println(esp_ip);
}

/*
 void reconnect();
 Function: Connect to MQTT broker;
 Parameters: ----------------
*/
void reconnect(){
  while(!client.connected()){
    Serial.print("Connecting to " + String(broker_ip));
    //if(client.connect(clientID)){
    if(client.connect(clientID, brokerUser, brokerPass)){
      Serial.println(": Connected.");
    } else{
      Serial.println(": [Error] Not connected: " + String(client.state()) + " Wait 5 seconds before retry.");
      delay(5000);
    }
  }
}


/*
 void publish(const char* topic, char* m);
 Function: Send Data to topic on MQTT server;
 Parameters:  topic -> string with topic name;
              m -> message;
*/
void publish(const char* topic, char* m){
  char messages[strlen(m)];
  snprintf(messages, (strlen(m)+1) , m);
  Serial.println("Sending: " + String(messages));
  client.publish(topic, messages);
}

/*
 void setupmqtt();
 Function: Setup MQTT server;
 Parameters: ---------------
*/
void setupmqtt(){
  client.setServer(broker, port);
  //client.setCallback(callback);
}

/*
 uint8_t temp_read()
 Function: Read value of TEMPERATURE sensor;
 Parameters: ----------------
*/
uint16_t temp_read(){
  return analogRead(TEMP_PIN);
}

/*
 uint8_t turb_read()
 Function: Read value of TURBIDITY sensor;
 Parameters: ----------------
*/
uint16_t turb_read(){
  return analogRead(TURB_PIN);
}

/*
 uint8_t dens_read()
 Function: Read value of DENSITY sensor;
 Parameters: ----------------
*/
uint16_t dens_read(){
  return analogRead(DENS_PIN);
}

