// INCLUDE DIRECTIVES -----------------------------------------------------------
#include "config.h"
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "DHTesp.h"

// OBJECTS -----------------------------------------------------------------------
DHTesp dht;                            // Initialize the DHT sensor object
AsyncWebServer server(WEBSERVER_PORT); // Initialize the AsyncWebServer object on port

// FUNCTION PROTOTYPES ----------------------------------------------------------
void connectWiFi(const char* ssid, const char* password);
void handleMeasurementsRequest(AsyncWebServerRequest *request);
void checkWiFiConnection();
String getAllMeasurements();
float readHumidity();
float readTemperature();
int readCO2Level();

// SETUP ------------------------------------------------------------------------
void setup() {
  Serial.begin(BAUD_RATE);                             // Begin serial communication for debugging
  dht.setup(DHT_PIN, DHTesp::DHT_MODEL_t::DHT22);      // Setup DHT sensor on specified pin
  connectWiFi(WIFI_SSID, WIFI_PASSWORD);               // Connect to WiFi network using specified credentials
  server.on("/", HTTP_GET, handleMeasurementsRequest); // Handle all measurements request
  server.begin();                                      // Start the web server
}

// LOOP -------------------------------------------------------------------------
void loop() {
  checkWiFiConnection(); // Check and reconnect to WiFi if necessary
}

// FUNCTION DEFINITIONS ---------------------------------------------------------
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
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi Disconnected. Attempting reconnection...");
    connectWiFi(WIFI_SSID, WIFI_PASSWORD);
  }
}

void handleMeasurementsRequest(AsyncWebServerRequest *request) {
  request->send(200, "text/plain", getAllMeasurements());
}

String getAllMeasurements() {
  String measurements = "Temperature: "  + String(readTemperature()) + " ºC\n";
         measurements += "Humidity   : " + String(readHumidity())    + " %\n";
         measurements += "CO2 Level  : " + String(readCO2Level())    + " ppm";
  return measurements;
}


float readTemperature() {
  return dht.getTemperature();
}

float readHumidity() {
  return dht.getHumidity();
}


int readCO2Level() {
  static const float ReferenceVoltage = 3.3;
  static const float MaxAdcValue = 4095.0;
  static const float VoltageThreshold = 0.4;
  static const float CalibrationFactor = 5000.0;
  static const float VoltageOffset = 1.6;

  int adcVal = analogRead(ANALOG_PIN);                       // Read analog value from CO2 sensor
  float voltage = adcVal * (ReferenceVoltage / MaxAdcValue); // Calculate voltage based on ADC value

  // Calculate CO2 measurement based on voltage difference
  if (voltage == 0) {
    return -1; // Sensor not operating correctly
  } else if (voltage < VoltageThreshold) {
    return -2; // Sensor pre-heating
  } else {
    float voltageDifference = voltage - VoltageThreshold;
    return static_cast<int>((voltageDifference * CalibrationFactor) / VoltageOffset);
  }
}

