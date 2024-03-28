#include "config.h"
#include "wifi_manager.h"
#include "web_server.h"
#include "sensors.h"

void setup() {
  Serial.begin(BAUD_RATE);               // Start serial output
  dhtSetup(DHT_PIN);                     // Setup DHT sensor, function defined in sensors module
  relaySetup(RELAY_PIN);                 // Setup relay
  connectWiFi(WIFI_SSID, WIFI_PASSWORD); // Connect to WiFi
  setupWebServer();                      // Setup web server
}

void loop() {
  checkWiFiConnection(); // Check if WiFi is connected
}
