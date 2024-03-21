// INCLUDE DIRECTIVES -----------------------------------------------------------
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "DHTesp.h"

// CONSTANTS --------------------------------------------------------------------
const char* const WIFI_SSID     = "YourWifiSSID";     // WiFi SSID (network name)
const char* const WIFI_PASSWORD = "YourWifiPassword"; // WiFi password
const int ANALOG_PIN            = 35;                 // Analog pin for CO2 measurement
const int DHT_PIN               = 27;                 // DHT sensor pin for temperature and humidity measurement
const int WEBSERVER_PORT        = 80;                 // Webserver port
const int BAUD_RATE             = 115200;             // Baud rate for serial connection

// OBJECTS -----------------------------------------------------------------------
DHTesp dht;                           // Initialize the DHT sensor object
AsyncWebServer server(WEBSERVER_PORT); // Initialize the AsyncWebServer object on port

// FUNCTION PROTOTYPES ----------------------------------------------------------
void connectWiFi(const char* ssid, const char* password);
int getCo2Measurement();

// SETUP ------------------------------------------------------------------------
void setup() {

  // Setup DHT sensor on specified pin
  dht.setup(DHT_PIN, DHTesp::DHT_MODEL_t::DHT22);

  // Begin serial communication for debugging
  Serial.begin(BAUD_RATE);

  // Connect to WiFi network using specified credentials
  connectWiFi(WIFI_SSID, WIFI_PASSWORD);

  // Handle CO2 measurement request
  server.on("/co2", HTTP_GET, [](AsyncWebServerRequest * request) {

    // Get CO2 measurement
    int measurement = getCo2Measurement();

    // Prepare response message based on measurement
    String message;
    if(measurement == -1){message = "Sensor is not operating correctly";}
    else if(measurement == -2){message = "Sensor is pre-heating";}
    else {message = String(measurement) + " ppm";}

    // Send response with HTTP status 200 (OK)
    request->send(200, "text/plain", message);

  });

  // Handle temperature measurement request
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest * request) {

    // Get temperature from DHT sensor
    float temperature = dht.getTemperature();

    // Send temperature response with HTTP status 200 (OK)
    request->send(200, "text/plain", String(temperature) + " ºC");
  });

  // Handle humidity measurement request
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest * request) {

    // Get humidity from DHT sensor
    float humidity = dht.getHumidity();

    // Send humidity response with HTTP status 200 (OK)
    request->send(200, "text/plain", String(humidity) + " %");
  });

  // Start the web server
  server.begin();
}

// LOOP -------------------------------------------------------------------------
void loop() {
  // Add any continuous operations here
}

// FUNCTION DEFINITIONS ---------------------------------------------------------
void connectWiFi(const char* ssid, const char* password) {

  // Connect to WiFi network using specified credentials
  WiFi.begin(ssid, password);

  // Variable to store connection status
  bool connected = false;

  Serial.print("Connecting to WiFi: " + String(ssid));

  // Wait for WiFi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  // Check if connected
  if (WiFi.status() == WL_CONNECTED) {
    connected = true;
    Serial.println("\n-----------------------------");
    Serial.println("Connected to: " + String(ssid));
    Serial.println("IP Address  : " + String(WiFi.localIP()));
    Serial.println("-----------------------------");
  }

  // Use the 'connected' variable to print the connection status
  if (!connected) {
    Serial.println("\nFailed to connect to WiFi: " + String(ssid));
  }
}

int getCo2Measurement() {

  // Read analog value from CO2 sensor
  int adcVal = analogRead(ANALOG_PIN);

  // Calculate voltage based on ADC value
  float voltage = adcVal * (3.3 / 4095.0);

  // Check for zero voltage (sensor not operating correctly)
  if (voltage == 0) {
    return -1;
  }
  // Check for low voltage (sensor pre-heating)
  else if (voltage < 0.4) {
    return -2;
  }
  // Calculate CO2 measurement based on voltage difference
  else {
    float voltageDifference = voltage - 0.4;
    return static_cast<int>((voltageDifference * 5000.0) / 1.6);
  }
}

