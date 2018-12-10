#include <PubSubClient.h>
#include <Arduino.h>
#include <WiFi.h>
#include <IPAddress.h>

PubSubClient client;

const char* clientID = "FEUP_SEAI"; // MQTT client ID

/******   ONLINE BROKER  ******/
const char* brokerUser = "jbeleza.tr@gmail.com";
const char* brokerPass = "6ec21d8d";
const char* broker = "mqtt.dioty.co";
const char* broker_ip = "mqtt.dioty.co";
const uint16_t port = 1883;
const char* outTopic = "/jbeleza.tr@gmail.com/out";

/******   LOCAL BROKER  ******/
/* const char* broker_ip = "192.168.43.64";
IPAddress broker(192,168,43,64);
//IPAddress broker = IPAddress().fromString(broker_ip);
const uint16_t port = 1883;
const char* outTopic = "topic"; */


/**
 * @brief Setup MQTT server;
 * @param
 * 
 */
void setupmqtt(WiFiClient wifiClient){
    client = PubSubClient(wifiClient);     // Setup MQTT server on espClient network
    client.setServer(broker, port);
}


/**
 * @brief Connect to MQTT broker;
 * @param
 * 
 */
void mqtt_connect(){
    while(!client.connected()){
        Serial.print("Connecting to " + String(broker_ip));
        //if(client.connect(clientID)){                           // Line for MQTT local server
        if(client.connect(clientID, brokerUser, brokerPass)){     // Line for MQTT online server
            Serial.println(": Connected.");
        } else{
            Serial.println(": [Error] Not connected: " + String(client.state()) + " Wait 5 seconds before retry.");
            delay(5000);
        }
    }
}

/**
 * @brief Send Data to topic on MQTT server;
 * @param
 *        topic -> string with topic name;
 *        m -> message;
 */
void publish(const char* topic, char* m){
    char messages[strlen(m)];
    snprintf(messages, (strlen(m)+1) , m);
    Serial.println("Sending: " + String(messages));
    client.publish(topic, messages);
}

