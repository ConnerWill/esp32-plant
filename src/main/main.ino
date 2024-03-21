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
DHTesp dht;                            // Initialize the DHT sensor object
AsyncWebServer server(WEBSERVER_PORT); // Initialize the AsyncWebServer object on port

// FUNCTION PROTOTYPES ----------------------------------------------------------
void connectWiFi(const char* ssid, const char* password);
String getAllMeasurements();

// SETUP ------------------------------------------------------------------------
void setup() {

  // Begin serial communication for debugging
  Serial.begin(BAUD_RATE);

  // Setup DHT sensor on specified pin
  dht.setup(DHT_PIN, DHTesp::DHT_MODEL_t::DHT22);

  // Connect to WiFi network using specified credentials
  connectWiFi(WIFI_SSID, WIFI_PASSWORD);

  // Handle all measurements request
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    String measurements = getAllMeasurements();
    request->send(200, "text/plain", measurements);
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
  Serial.print("Connecting to WiFi: " + String(ssid));

  // Wait for WiFi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  // Print connection status
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n-----------------------------");
    Serial.println("Connected to: " + String(ssid));
    Serial.println("IP Address  : " + String(WiFi.localIP()));
    Serial.println("-----------------------------");
  } else {
    Serial.println("\nFailed to connect to WiFi: " + String(ssid));
  }
}

String getAllMeasurements() {
  float temperature = dht.getTemperature(); // Get temperature
  float humidity = dht.getHumidity();       // Get humidity
  int co2 = getCo2Measurement();            // Get CO2 measurement

  String measurements = "Temperature: " + String(temperature) + " ºC\n";
  measurements += "Humidity: " + String(humidity) + " %\n";
  measurements += "CO2 Level: " + String(co2) + " ppm";

  return measurements;
}

int getCo2Measurement() {
  int adcVal = analogRead(ANALOG_PIN);     // Read analog value from CO2 sensor
  float voltage = adcVal * (3.3 / 4095.0); // Calculate voltage based on ADC value

  // Calculate CO2 measurement based on voltage difference
  if (voltage == 0) {
    return -1; // Sensor not operating correctly
  } else if (voltage < 0.4) {
    return -2; // Sensor pre-heating
  } else {
    float voltageDifference = voltage - 0.4;
    return static_cast<int>((voltageDifference * 5000.0) / 1.6);
  }
}

