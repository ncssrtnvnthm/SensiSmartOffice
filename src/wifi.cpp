#include "config.h"
#include <WiFi.h>

void Setup_Wifi(){
  Serial.println("Try Connecting to ");
  Serial.println(WIFI_AP_SSID);

  // Connect to your wi-fi modem
  WiFi.begin(WIFI_AP_SSID, WIFI_AP_PASSWORD);

  // Check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());  //Show ESP32 IP on serial
}

void Setup_Wifi_AP(){
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PASSWORD);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
}
