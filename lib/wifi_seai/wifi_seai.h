#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <IPAddress.h>

// ToDo - configure connection with static IP

const char* ssid = "HUAWEI";
const char* pass = "1234567890esp";

IPAddress espIP, gatewayIP;

/* UNCOMMENT for Configuration of wifi network with static IP */
/* 
IPAddress ip(192, 168, 43, 72);          // ESP IP, if static
IPAddress gateway(192, 168, 43, 1);      // Gateway IP
IPAddress subnet(255, 255, 255, 0);     // Subnet mask
IPAddress primaryDNS(8, 8, 8, 8);       // Primary DNS (optional)
IPAddress secundaryDNS(8, 8, 4, 4);     // Secundary DNS (optional)
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
  

  if(!WiFi.mode(WIFI_STA)){
    Serial.println("Error: set STA mode."); // Set WIFI mode
  }            

  // Setup static IP configuration
  /* if(!WiFi.config( ip, gateway, subnet, primaryDNS, secundaryDNS )){
    Serial.println("Error: STA Failed to configure");
  } */

  delay(100);
  Serial.print("\nConnecting to wifi: " + String(ssid) + " ");

  
  WiFi.begin(ssid, pass);         // Connect to network

  while(WiFi.status() != WL_CONNECTED){   // Wait connection finish
    delay(100);
    Serial.print(".");
  }

  espIP = WiFi.localIP();
  gatewayIP = WiFi.gatewayIP();
  Serial.print(" Connected.\nESP32 IP: ");
  Serial.print(espIP);
  Serial.print(" Gateway IP: ");
  Serial.println(gatewayIP);
}