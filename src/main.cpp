#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <esp_wifi.h>
#include <IPAddress.h>
#include <esp_bt_main.h>
#include <esp_bt.h>
#include <wifi_seai.h>
#include <mqtt_seai.h>
#include <sensors.h>

#define uS_TO_S_FACTOR 1000000      /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  5            /* Time ESP32 will go to sleep (in seconds) */

#define moduleID 123                // sensor module ID between 1-3

WiFiClient espClient;               // Wifi client


void RTC_IRAM_ATTR esp_wake_deep_sleep(void) {
  esp_default_wake_deep_sleep();
  // Add additional functionality here
  WiFi.mode( WIFI_OFF );
  delay( 1 );
}


void setup() {
  WiFi.mode( WIFI_OFF );

  Serial.begin(115200);           // Init Serial Monitor
  
  sensors_setup();

  // FUNCTION TO READ SENSORS DATA AND CONSTRUCT STRING DATA
  //--------------------------------------------------------------------------------
  String mes = String( String(moduleID) + "," + String(temp_read()) + "," + String(turb_read()) + "," + String(dens_read()) );
  char data[(mes.length())+1];
  mes.toCharArray(data,mes.length() + 1);
  //--------------------------------------------------------------------------------

  setupWifi();                    // Setup WiFi
  setupmqtt(espClient);           // Connect to MQTT server
  delay(100);

  mqtt_connect();

  // PUBLISH DATA DO MQTT SERVER
  //--------------------------------------------------------------------------------
  publish(outTopic, data);        // publish message in specific topic
  delay(3000);                    // delay is needed for server accept published message
  //--------------------------------------------------------------------------------


  esp_wifi_disconnect();          // Disconnect from network
  esp_bluedroid_disable();        // Disable bluetooth driver
  esp_bt_controller_disable();    // Disable bluetooth driver before deep sleep
  esp_wifi_stop();                // Disable WIFI driver before deep sleep

  // DEEP SLEEP MODE SETUP AND START
  //--------------------------------------------------------------------------------
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);  // set wakeup time
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) + " Seconds");

  Serial.println("Going to sleep now");
  Serial.flush();                 // wait for finish serial prints
  esp_deep_sleep_start();         // entry in deep sleep mode
  //--------------------------------------------------------------------------------
}

void loop() {
}

