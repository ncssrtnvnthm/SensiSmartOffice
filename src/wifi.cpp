#include "config.h"
#include <WiFi.h>
#include <time.h>

static bool wifiStaConnected = false;
static bool timeSynced = false;

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

void Setup_Wifi_Station() {
  if (strlen(WIFI_STA_SSID) == 0) {
    Serial.println("No station SSID configured, skipping WiFi station");
    return;
  }
  Serial.print("Connecting to WiFi: ");
  Serial.println(WIFI_STA_SSID);
  WiFi.begin(WIFI_STA_SSID, WIFI_STA_PASSWORD);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    wifiStaConnected = true;
    Serial.println("");
    Serial.print("Station IP: ");
    Serial.println(WiFi.localIP());

    // Immediate NTP time sync
    configTime(NTP_GMT_OFFSET_SEC, NTP_DST_OFFSET_SEC, NTP_SERVER);
    struct tm ti;
    if (getLocalTime(&ti, 10000)) {
      timeSynced = true;
      Serial.print("NTP time synced: ");
      Serial.println(&ti, "%Y-%m-%d %H:%M:%S");
    } else {
      Serial.println("NTP sync failed, will retry");
    }
  } else {
    Serial.println("");
    Serial.println("WiFi station connect failed (no NTP time)");
  }
}

bool IsWifiStationConnected() { return wifiStaConnected; }
bool IsTimeSynced() { return timeSynced; }
