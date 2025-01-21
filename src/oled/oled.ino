#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "DHTesp.h"
#include <ArduinoJson.h>

// ============================================================================
// CONFIGURATION --------------------------------------------------------------
// ============================================================================
const char* WIFI_SSID     = "YourNetworkName"; // Wi-Fi SSID
const char* WIFI_PASSWORD = "YourNetworkPass"; // Wi-Fi password
const char* WIFI_HOSTNAME = "esp32-oled";      // Hostname

const int   SERVER_PORT = 80;  // Port for the web server
const char* SERVER_PATH = "/"; // Path for serving the data

int CO2_PIN = 35; // Analog pin for CO2 sensor
int DHT_PIN = 27; // GPIO pin for DHT sensor

int BAUD_RATE = 115200; // Baud rate
// ============================================================================

// Global Instances
DHTesp dht;
AsyncWebServer server(SERVER_PORT);

// ============================================================================
// FUNCTIONS ------------------------------------------------------------------
// ============================================================================

// Function to get the CO2 measurement
int getCo2Measurement() {
  int adcVal = analogRead(CO2_PIN);
  float voltage = adcVal * (3.3 / 4095.0);

  if (voltage == 0 || voltage < 0.4) {
    return 0; // Return 0 for sensor errors or preheating
  } else {
    float voltageDifference = voltage - 0.4;
    return (int)((voltageDifference * 5000.0) / 1.6);
  }
}

// ============================================================================

void setup() {
  // Initialize DHT sensor
  dht.setup(DHT_PIN);

  // Initialize Serial for debugging
  Serial.begin(BAUD_RATE);

  // Connect to WiFi
  // Set hostname before connecting to Wi-Fi
  if (!WiFi.setHostname(WIFI_HOSTNAME)) {
    Serial.println("Failed to set hostname");
  }
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi Connected");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("Hostname: ");
  Serial.println(WiFi.getHostname());

  // Define the root endpoint
  server.on(SERVER_PATH, HTTP_GET, [](AsyncWebServerRequest* request) {
    // Create a JSON document
    StaticJsonDocument<200> jsonDoc;

    // Get sensor values
    int co2           = getCo2Measurement();
    float temperature = dht.getTemperature();
    float humidity    = dht.getHumidity();

    // Add values to the JSON document
    jsonDoc["co2"] = co2;
    jsonDoc["temperature"] = isnan(temperature) ? 0 : temperature; // Handle NaN
    jsonDoc["humidity"] = isnan(humidity) ? 0 : humidity;         // Handle NaN

    // Serialize JSON to string
    String jsonString;
    serializeJson(jsonDoc, jsonString);

    // Send the JSON response
    request->send(200, "application/json", jsonString);
  });

  // Start the server
  server.begin();
  Serial.println("Server started.");
}

void loop() {}
