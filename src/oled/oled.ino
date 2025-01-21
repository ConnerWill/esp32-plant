#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "DHTesp.h"
#include <ArduinoJson.h>

// ============================================================================
// CONFIGURATION --------------------------------------------------------------
// ============================================================================
constexpr char WIFI_SSID[]     = "YourNetworkName"; // Wi-Fi SSID
constexpr char WIFI_PASSWORD[] = "YourNetworkPass"; // Wi-Fi password
constexpr char WIFI_HOSTNAME[] = "esp32-oled";      // Hostname

constexpr int   SERVER_PORT = 80;  // Port for the web server
constexpr char  SERVER_PATH[] = "/"; // Path for serving the data

constexpr int CO2_PIN = 35; // Analog pin for CO2 sensor
constexpr int DHT_PIN = 27; // GPIO pin for DHT sensor

constexpr int BAUD_RATE = 115200; // Baud rate
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
    return static_cast<int>((voltageDifference * 5000.0) / 1.6);
  }
}

// Function to connect to Wi-Fi
void connectToWiFi() {
  Serial.println("Setting up Wi-Fi...");
  if (!WiFi.setHostname(WIFI_HOSTNAME)) {
    Serial.println("Error: Failed to set Wi-Fi hostname");
  }

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Wi-Fi Connected");
  Serial.printf("IP: %s\n", WiFi.localIP().toString().c_str());
  Serial.printf("Hostname: %s\n", WiFi.getHostname());
}

// ============================================================================

void setup() {
  // Initialize DHT sensor
  dht.setup(DHT_PIN);

  // Initialize Serial for debugging
  Serial.begin(BAUD_RATE);

  // Connect to Wi-Fi
  connectToWiFi();

  // Define the root endpoint
  server.on(SERVER_PATH, HTTP_GET, [](AsyncWebServerRequest* request) {
    // Create a JSON document
    StaticJsonDocument<200> jsonDoc;

    // Get sensor values
    int   co2 = getCo2Measurement();
    float temperature = dht.getTemperature();
    float humidity = dht.getHumidity();

    // Add values to the JSON document
    jsonDoc["co2"]         = co2;
    jsonDoc["temperature"] = isnan(temperature) ? 0 : temperature; // Handle NaN
    jsonDoc["humidity"]    = isnan(humidity) ? 0 : humidity;       // Handle NaN

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
