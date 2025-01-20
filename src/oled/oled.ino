#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>

// Configurable variables
const char* ssid = "YOUR_WIFI_SSID"; // Wi-Fi SSID
const char* password = "YOUR_WIFI_PASSWORD"; // Wi-Fi password
const char* WIFI_HOSTNAME = "esp32-oled";    // Hostname for ESP32

const int DHT_PIN = 27;  // DHT22 sensor pin
const int CO2_ANALOG_PIN = 35;  // CO2 sensor analog pin

const int CO2_SENSOR_VOLTAGE_THRESHOLD = 0.4; // CO2 sensor threshold voltage
const int CO2_SENSOR_CALIBRATION_FACTOR = 5000; // CO2 sensor calibration factor
const int CO2_SENSOR_VOLTAGE_OFFSET = 1.6; // CO2 sensor voltage offset

const int OLED_ADDRESS = 0x3C;  // OLED Address. 0x3D for 128x64
const int SCREEN_WIDTH = 128;   // OLED screen width
const int SCREEN_HEIGHT = 64;   // OLED screen height

const int BAUD_RATE             = 115200;             // Baud rate for serial connection

// DHT22 sensor setup
DHT dht(DHT_PIN, DHT22);

// OLED display setup
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Web server on port 80
AsyncWebServer server(80);

// Function to read CO2 level
int readCO2Level() {
  static const float ReferenceVoltage = 3.3;
  static const float MaxAdcValue = 4095.0;

  int adcVal = analogRead(CO2_ANALOG_PIN); // Read analog value from CO2 sensor
  float voltage = adcVal * (ReferenceVoltage / MaxAdcValue); // Calculate voltage based on ADC value

  // Calculate CO2 measurement based on voltage difference
  if (voltage == 0) {
    return -1; // Sensor not operating correctly
  } else if (voltage < CO2_SENSOR_VOLTAGE_THRESHOLD) {
    return -2; // Sensor pre-heating
  } else {
    float voltageDifference = voltage - CO2_SENSOR_VOLTAGE_THRESHOLD;
    return static_cast<int>((voltageDifference * CO2_SENSOR_CALIBRATION_FACTOR) / CO2_SENSOR_VOLTAGE_OFFSET);
  }
}

void setup() {
  // Initialize serial communication
  Serial.begin(BAUD_RATE);

  // Initialize DHT sensor
  dht.begin();

  // Setup DHT sensor
  DHTesp dht;
  dht.setup(DHT_PIN, DHTesp::DHT_MODEL_t::DHT22);

  // Initialize OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  WiFi.setHostname(WIFI_HOSTNAME); // Set hostname before connecting to WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(F("Connecting to WiFi..."));
  }
  Serial.println(F("WiFi connected"));

  // Serve current readings as JSON on port 80
  server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request){
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    int co2Level = readCO2Level();

    if (isnan(temperature) || isnan(humidity)) {
      request->send(500, "application/json", "{\"error\":\"Failed to read DHT sensor\"}");
      return;
    }

    // Create JSON object
    StaticJsonDocument<200> doc;
    doc["temperature"] = temperature;
    doc["humidity"] = humidity;
    doc["co2"] = co2Level;

    String jsonResponse;
    serializeJson(doc, jsonResponse);

    // Send response
    request->send(200, "application/json", jsonResponse);
  });

  // Start the web server
  server.begin();
}

void loop() {
  // Read temperature and humidity
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  // Display the readings on the OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  // Display Temperature and Humidity
  display.print("Temp: ");
  display.print(temperature);
  display.println(" C");
  
  display.print("Humidity: ");
  display.print(humidity);
  display.println(" %");

  // Display CO2 level
  int co2Level = readCO2Level();
  display.print("CO2: ");
  display.print(co2Level);
  display.println(" ppm");

  // Update OLED display
  display.display();

  // Wait 2 seconds before updating the screen again
  delay(2000);
}
