/*
 * File: main.ino
 *
 */

#include "config.h"
#include "wifi_manager.h"
#include "web_server.h"
#include "sensors.h"

void setup() {
  Serial.begin(BAUD_RATE);
  dhtSetup(DHT_PIN); // Setup DHT sensor, function defined in sensors module
  connectWiFi(WIFI_SSID, WIFI_PASSWORD);
  setupWebServer();
}

void loop() {
  checkWiFiConnection();

  // Read sensor data
  float temperatureC = readTemperature();
  float temperatureF = celsiusToFahrenheit(temperatureC);
  float humidity = readHumidity();
  int co2Level = readCO2Level();

  // Display on OLED
  displaySensorData(temperatureF, humidity, co2Level);
}
