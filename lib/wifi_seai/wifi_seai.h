#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <IPAddress.h>

// ToDo - configure connection with static IP

const char* ssid = "HUAWEI";
const char* pass = "1234567890esp";

IPAddress espIP, gatewayIP;

/* Configuration for wifi network with static IP */
/*
IPAddress ip(192, 168, 1, 34);          // ESP IP, if static
IPAddress gateway(192, 168, 1, 1);      // Gateway IP
IPAddress subnet(255, 255, 255, 0);     // Subnet mask
*/

/**
 * @brief Connect to wifi network;
 * @param
 * 
 */
void setupWifi(){

  WiFi.persistent( false );       // Stop saving Wifi config on flash memory
  //WiFi.forceSleepWake();
  //delay(1);
  
  WiFi.mode(WIFI_STA);            // Set WIFI mode

  //WiFi.config( ip, gateway, subnet ); // Setup static IP configuration

  delay(100);
  Serial.print("\nConnecting to wifi: " + String(ssid) + " ");

  
  WiFi.begin(ssid, pass);         // Connect to network

  while(WiFi.status() != WL_CONNECTED){   // Wait connection finish
    delay(100);
    Serial.print(".");
  }

  espIP = WiFi.localIP();
  gatewayIP = WiFi.gatewayIP();
  Serial.print("\nConnected. ESP32 IP: ");
  Serial.print(espIP);
  Serial.print(" Gateway IP: ");
  Serial.println(gatewayIP);
}