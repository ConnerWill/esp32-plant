/*
 * File: ./oled.ino
 */

#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "DHTesp.h"
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Include bitmap header file
#include "bitmap.h"

// ============================================================================
// CONFIGURATION --------------------------------------------------------------
// ============================================================================
// WIFI CONFIGURATION
constexpr char WIFI_SSID[]         = "YourNetworkName"; // Wi-Fi SSID
constexpr char WIFI_PASSWORD[]     = "YourNetworkPass"; // Wi-Fi password
constexpr char WIFI_HOSTNAME[]     = "esp32-oled";      // Hostname
constexpr int  WIFI_TIMEOUT_TIME   = 30000;             // Timeout if unable to connect to WiFi (ms)
constexpr int  WIFI_CHECK_INTERVAL = 5000;              // Check WiFi time (ms)

// SERVER CONFIGURATION
constexpr uint16_t SERVER_PORT = 80;    // Port for the web server
constexpr char     SERVER_PATH[] = "/"; // Path for serving the data

// PINS CONFIGURATION
constexpr uint8_t CO2_PIN = 35; // Analog pin for CO2 sensor
constexpr uint8_t DHT_PIN = 27; // GPIO pin for DHT sensor

// SCREEN CONFIGURATION
constexpr int     SCREEN_WIDTH                = 128;  // OLED display width, in pixels
constexpr int     SCREEN_HEIGHT               = 64;   // OLED display height, in pixels
constexpr uint8_t SCREEN_ADDRESS              = 0x3C; // Address of OLED display (could also be '0x3D' depending on screen resolution)
constexpr int     SCREEN_UPDATE_TIME          = 1000; // Time to wait before updating OLED (ms)
constexpr int     SCREEN_STARTUP_DISPLAY_TIME = 3000; // Startup screen delay time (ms)
bool              SHOW_STARTUP                = true; // Set to true to show the startup sequence
bool              SHOW_BITMAP                 = true; // Set to true to show the bitmap

// SERIAL CONFIGURATION
constexpr int BAUD_RATE = 115200; // Baud rate
// ============================================================================

// ============================================================================
// GLOBAL INSTANCES -----------------------------------------------------------
// ============================================================================
DHTesp dht;                         //
AsyncWebServer server(SERVER_PORT); // Define server on port
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)

// ============================================================================

// ============================================================================
// FUNCTIONS ------------------------------------------------------------------
// ============================================================================

// Function to get the CO2 measurement 
float readCO2() {
  constexpr float ReferenceVoltage  = 3.3;
  constexpr float MaxAdcValue       = 4095.0;
  constexpr float VoltageThreshold  = 0.4;
  constexpr float CalibrationFactor = 5000.0;
  constexpr float VoltageOffset     = 1.6;

  int adcVal = analogRead(CO2_PIN);                          // Read analog value from CO2 sensor
  float voltage = adcVal * (ReferenceVoltage / MaxAdcValue); // Calculate voltage based on ADC value

  // Calculate CO2 measurement based on voltage difference
  if (voltage == 0 || voltage < VoltageThreshold) {
    Serial.println(F("Error reading CO2"));
    return 0.0; // Return 0 for sensor errors or pre-heating
  } else {
    float voltageDifference = voltage - VoltageThreshold;
    return (voltageDifference * CalibrationFactor) / VoltageOffset;
  }
}

// Function to read temperature
float readTemperature() {
  float temperature = dht.getTemperature();
  if (isnan(temperature)) {
      Serial.printf("Error reading temperature: %s\n", dht.getStatusString());
      return 0.0;
  }
  return temperature;
}

// Function to read humidity
float readHumidity() {
  float humidity = dht.getHumidity();
  if (isnan(humidity)) {
      Serial.printf("Error reading humidity: %s\n", dht.getStatusString());
      return 0.0;
  }
  return humidity;
}

// Function to convert Celsius to Fahrenheit
float celsiusToFahrenheit(float celsius) {
    float fahrenheit = (celsius * 9.0 / 5.0) + 32.0;
    return fahrenheit;
}

// Function to connect to Wi-Fi
void connectToWiFi() {
  Serial.println(F("Setting up Wi-Fi..."));
  if (!WiFi.setHostname(WIFI_HOSTNAME)) {
    Serial.printf("Error: Failed to set Wi-Fi hostname: %s\n", WIFI_HOSTNAME);
  }

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.printf("Connecting to WiFi: %s\n", WIFI_SSID);

  // Record the start time of the connection attempt
  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - startAttemptTime >= WIFI_TIMEOUT_TIME) { // Timeout after X seconds
      Serial.printf("Failed to connect to WiFi: %s\n", WIFI_SSID);
      return;
    }
    delay(1000);
    Serial.print(".");
  }

  Serial.println(F("Wi-Fi Connected"));
  Serial.printf("SSID    : %s\n", WIFI_SSID);
  // TODO: Figure out if i need to use the 'toString() function'
  Serial.printf("IP      : %s\n", WiFi.localIP().toString().c_str());
  //Serial.printf("IP      : %s\n", WiFi.localIP());
  Serial.printf("HOSTNAME: %s\n", WiFi.getHostname());
}

// Function to initialize OLED
void initOLED() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Failed to start SSD1306 OLED"));
    while (1);
  }

  // Setup
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Show start text if SHOW_STARTUP is true
  if (SHOW_STARTUP) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 10);
    display.println("Starting");
    display.display();
    // Scroll right
    display.startscrollleft(0x00, 0x0F);
    delay(SCREEN_STARTUP_DISPLAY_TIME);
    display.stopscroll();

    // Display bitmap if SHOW_BITMAP is true
    if (SHOW_BITMAP) {
      display.clearDisplay();
      display.drawBitmap(0, 0, bitmap_image, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
      display.display();
      delay(SCREEN_STARTUP_DISPLAY_TIME);
      // Scroll right
      display.startscrollright(0x00, 0x0F);
      delay(SCREEN_STARTUP_DISPLAY_TIME);
      display.stopscroll();
    }
  }

  // Clear display
  display.clearDisplay();
}

// Function to update the OLED with sensor readings
void updateOLED(int co2, float temperature, float humidity) {
  // Clear Display
  display.clearDisplay();

  // IP Address
  display.setCursor(0, 0);
  display.print("IP: ");
  //TODO: Figure out if i need the toString function here
  display.println(WiFi.localIP().toString().c_str());

  // Temperature
  display.setCursor(0, 10);
  display.print("Temp: ");
  display.print(temperature);
  display.println(" C");

  // Humidity
  display.setCursor(0, 20);
  display.print("Humidity: ");
  display.print(humidity);
  display.println(" %");

  // CO2
  display.setCursor(0, 30);
  display.print("CO2: ");
  display.print(co2);
  display.println(" ppm");

  // Update Display
  display.display();

// TODO: Avoid full screen redraws when only specific values change
//  static String lastIP = "";
//  static float lastTemp = 0.0;
//  static float lastHumidity = 0.0;
//  static float lastCO2 = 0.0;
//
//  // IP Address
//  if (WiFi.localIP().toString() != lastIP) {
//    display.setCursor(0, 0);
//    display.print("IP: ");
//    //TODO: Figure out if i need the toString function here
//    // display.println(WiFi.localIP().toString().c_str());
//    display.println(WiFi.localIP().toString());
//    lastIP = WiFi.localIP().toString();
//  }
//
//  // Temperature
//  if (temperature != lastTemp) {
//    display.setCursor(0, 10);
//    display.print("Temp: ");
//    display.print(temperature);
//    display.println(" C");
//    lastTemp = temperature;
//  }
//
//  // Humidity
//  if (humidity != lastHumidity) {
//    display.setCursor(0, 20);
//    display.print("Humidity: ");
//    display.print(humidity);
//    display.println(" %");
//    lastHumidity = humidity;
//  }
//
//  // CO2
//  if (co2 != lastCO2) {
//    display.setCursor(0, 30);
//    display.print("CO2: ");
//    display.print(co2);
//    display.println(" ppm");
//    lastCO2 = co2
//  }
}

// ============================================================================

void setup() {
  // Set co2 pin mode
  pinMode(CO2_PIN, INPUT);

  // Initialize DHT sensor
  dht.setup(DHT_PIN, DHTesp::DHT_MODEL_t::DHT22);

  // Initialize Serial for debugging
  Serial.begin(BAUD_RATE);

  // Initialize OLED
  initOLED();

  // Connect to Wi-Fi
  connectToWiFi();

  // Define the root endpoint
  server.on(SERVER_PATH, HTTP_GET, [](AsyncWebServerRequest* request) {
    // Create a JSON document
    JsonDocument jsonDoc;
    //TODO: Old method that works but gives warning
    //StaticJsonDocument<256> jsonDoc;

    // Get sensor values
    float co2 = readCO2();
    float temperature = readTemperature();
    float temperatureF = celsiusToFahrenheit(temperature);
    float humidity = readHumidity();

    // Add values to the JSON document
    jsonDoc["co2"]          = co2;
    jsonDoc["temperature"]  = temperature;
    jsonDoc["temperatureF"] = temperatureF;
    jsonDoc["humidity"]     = humidity;

    // Serialize JSON to string
    String jsonString;
    serializeJson(jsonDoc, jsonString);

    // Send the JSON response
    request->send(200, "application/json", jsonString);

  });

  // Start the server
  server.begin();
  Serial.println(F("Server started"));
}

void loop() {

  static unsigned long lastUpdateTime = 0;
  static unsigned long lastWiFiCheck = 0;
  unsigned long currentTime = millis();

  if (currentTime - lastUpdateTime >= SCREEN_UPDATE_TIME) {
    lastUpdateTime = currentTime;

    // Get sensor values
    float co2 = readCO2();
    float temperature = readTemperature();
    float temperatureF = celsiusToFahrenheit(temperature);
    float humidity = readHumidity();

    // Debug values
    Serial.printf("Temperature: %.2f C\n", temperature);
    Serial.printf("Temperature: %.2f F\n", temperatureF);
    Serial.printf("Humidity   : %.2f %%\n", humidity);
    Serial.printf("CO2        : %.2f ppm\n", co2);
    Serial.println("Updating display...");

    // Update OLED display with latest sensor data
    updateOLED(co2, temperature, humidity);
  }

  // Periodically check Wi-Fi status
  if (currentTime - lastWiFiCheck >= WIFI_CHECK_INTERVAL) {
      lastWiFiCheck = currentTime;

      if (WiFi.status() != WL_CONNECTED) {
          Serial.println(F("Wi-Fi disconnected. Attempting to reconnect..."));
          WiFi.reconnect(); // Attempt to reconnect

          if (WiFi.status() != WL_CONNECTED) {
              Serial.println(F("Reconnection failed. Retrying full connection..."));
              connectToWiFi(); // Fallback to full connection
          } else {
              Serial.println(F("Reconnected to Wi-Fi."));
          }
      }
  }

  // Pause before running again
  // delay(SCREEN_UPDATE_TIME);
}
