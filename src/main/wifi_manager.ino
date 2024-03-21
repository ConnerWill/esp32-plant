#include "wifi_manager.h"
#include "config.h"

void connectWiFi(const char* ssid, const char* password) {
  WiFi.setHostname(WIFI_HOSTNAME); // Set hostname before connecting to WiFi
  WiFi.begin(ssid, password);      // Connect to WiFi network using specified credentials
  Serial.print("Connecting to WiFi: " + String(ssid) + " ");

  // Wait for WiFi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  // Print connection status
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n-----------------------------");
    Serial.println("Connected to : " + String(WiFi.SSID));
    Serial.println("Hostname     : " + String(WiFi.getHostname()));
    Serial.println("IP Address   : " + String(WiFi.localIP()));
    Serial.println("Strength     : " + String(WiFi.RSSI()));
    Serial.println("-----------------------------");
  } else {
    Serial.println("\nFailed to connect to WiFi: " + String(ssid));
  }
}

void checkWiFiConnection() {
  static unsigned long lastCheckTime = 0;
  const unsigned long checkInterval  = 10000; // Check every 10 seconds

  if (millis() - lastCheckTime > checkInterval) {
    lastCheckTime = millis();
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi Disconnected. Attempting reconnection...");
      connectWiFi(WIFI_SSID, WIFI_PASSWORD);
    }
  }
}
