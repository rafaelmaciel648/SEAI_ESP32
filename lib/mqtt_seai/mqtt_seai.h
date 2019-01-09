#include <PubSubClient.h>
#include <Arduino.h>
#include <WiFi.h>
#include <IPAddress.h>

#define LOCAL_SERVER
//#define ONLINE_SERVER


PubSubClient client;

const char* clientID = "FEUP_SEAI2"; // MQTT client ID

/******   LOCAL BROKER  ******/
#ifdef LOCAL_SERVER
    //const char* broker_ip = "192.168.43.64";
    //IPAddress broker1 = IPAddress().fromString(broker_ip);
    IPAddress broker(192,168,43,224);
    const uint16_t port = 1883;
    const char* outTopic = "topic";
#endif

/******   ONLINE BROKER  ******/
#ifdef ONLINE_SERVER
    const char* brokerUser = "rafaelmaciel648@gmail.com";
    const char* brokerPass = "cdce8090";
    const char* broker = "mqtt.dioty.co";
    const char* broker_ip = "mqtt.dioty.co";
    const uint16_t port = 1883;
    const char* outTopic = "/rafaelmaciel648@gmail.com/out";
#endif

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
        #ifdef LOCAL_SERVER                                         // Block for MQTT local server
            if(client.connect(clientID)){                           
        #endif

        #ifdef ONLINE_SERVER
            if(client.connect(clientID, brokerUser, brokerPass)){   // Block for MQTT online server
        #endif
        
        } else{
            delay(3000);
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
    client.publish(topic, messages);
}

