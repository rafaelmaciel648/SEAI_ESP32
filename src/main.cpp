#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <esp_wifi.h>
#include <IPAddress.h>
#include <wifi_seai.h>
#include <mqtt_seai.h>
#include <sensors.h>


/* Uncomment for tests */
//#define DEBUG_SENSORS
//#define DEBUG_SERIAL
#define DEBUG_RANDOM

#define moduleID 1                  // sensor module ID between 1-9

#define uS_TO_S_FACTOR 1000000      // Conversion factor for micro seconds to seconds
#define TIME_TO_SLEEP  30           // Time ESP32 will go to sleep (in seconds)

WiFiClient espClient;               // Wifi client


void setup() {
  //Serial.begin(115200);
  WiFi.mode( WIFI_OFF );            // Just for sure that is wifi is off

  sensors_setup();            // Setup sensors functions

  // FUNCTION TO READ SENSORS DATA AND CONSTRUCT STRING DATA
  //--------------------------------------------------------------------------------
  powerOn();                        // Turn on sensors power
  delay(1000);  //ATTENTION

  uint16_t dens_diff, dens1, dens2;
  dens1 = dens_read(1);
  dens2 = dens_read(2);
  dens_diff = dens1 - dens2 - 9;    // Error offset

  #ifdef DEBUG_RANDOM
    String mes = String( String(moduleID) + "," + String(random(896,4480)) + "," + String(random(3500,3700)) + "," + String(random(10,25)) );
    #else
    String mes = String( String(moduleID) + "," + String(temp_read()) + "," + String(turb_read())  + "," + String(dens_diff));
  #endif
  
  powerOff();                       // Turn on sensors power
  char data[(mes.length())+1];
  mes.toCharArray(data,mes.length() + 1);
  //--------------------------------------------------------------------------------

  #ifdef DEBUG_SENSORS
    powerOn();
    while(1){
      String aux = String( String(moduleID) + "," + String(temp_read()) + "," + String(turb_read()) + "," + String(dens_read()) );
      Serial.println(aux);
      delay(1000);
    }
  #endif

  if(setupWifi()){                  // Setup WiFi
    setupmqtt(espClient);           // Setup MQTT server
    delay(100);
    mqtt_connect();                 // Connect to MQTT server

    // PUBLISH DATA DO MQTT SERVER
    //--------------------------------------------------------------------------------
    publish(outTopic, data);        // publish message in specific topic
    delay(3000);                    // delay is needed for server accept published message
    //--------------------------------------------------------------------------------

    esp_wifi_disconnect();          // Disconnect from network
  }

  esp_wifi_stop();                  // Disable WIFI driver before deep sleep

  // DEEP SLEEP ROUTINE: mode setup and start
  //--------------------------------------------------------------------------------
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);  // set wakeup time
  esp_deep_sleep_start();         // entry in deep sleep mode
  //--------------------------------------------------------------------------------
}

void loop() {
}
