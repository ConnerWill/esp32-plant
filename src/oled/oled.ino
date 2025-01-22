/*
 * File: ./oled.ino
 */

// Dependencies
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "DHTesp.h"
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Include config header file
#include "config.h"

// Include bitmap header file
#include "bitmap.h"


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

// -------------------------------------
// Sensor Functions
// -------------------------------------

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


// -------------------------------------
// WIFI Functions
// -------------------------------------

// Function to connect to Wi-Fi
void connectToWiFi() {
  Serial.println(F("Setting up Wi-Fi..."));
  if (!WiFi.setHostname(WIFI_HOSTNAME)) {
    Serial.printf("Error: Failed to set Wi-Fi hostname: %s\n", WIFI_HOSTNAME);
  }

  // TODO: Add an if oled display check
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Connecting to WiFi");
  display.setCursor(0, 16);
  display.println(WIFI_SSID);
  display.display();
  delay(100);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.printf("Connecting to WiFi: %s\n", WIFI_SSID);

  // Record the start time of the connection attempt
  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - startAttemptTime >= WIFI_TIMEOUT_TIME) { // Timeout after X seconds
      Serial.printf("Failed to connect to WiFi: %s\n", WIFI_SSID);
      
      // TODO: Add an if oled display check
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("Failed to connect to WiFi!");
      display.setCursor(0, 16);
      display.println(WIFI_SSID);
      display.display();
      //TODO: Find a better time interval
      delay(SCREEN_STARTUP_DISPLAY_TIME);
      display.clearDisplay();
      // TODO: Values are not updating when wifi is disconnected

      return;
    }
    delay(1000);
    Serial.print(".");

    // TODO: Add an if oled display check
    display.print("_");
    display.display();
  }

  Serial.println(F("Wi-Fi Connected"));
  Serial.printf("SSID    : %s\n", WIFI_SSID);
  Serial.printf("IP      : %s\n", WiFi.localIP().toString().c_str());
  Serial.printf("HOSTNAME: %s\n", WiFi.getHostname());
  
  showIPInfo();
  delay(SCREEN_STARTUP_DISPLAY_TIME);
}

// -------------------------------------
// OLED Functions
// -------------------------------------

// Function to initialize OLED
void initOLED() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Failed to start SSD1306 OLED"));
    while (1);
  }

  // Show start text if SHOW_STARTUP is true
  if (SHOW_STARTUP) {
    showStart();
    // Display bitmap if SHOW_BITMAP is true
    if (SHOW_BITMAP) {
      showBitmap();
    }
  }

  // Default text settings
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
}


// Function to show startup display
void showStart() {
    display.clearDisplay();
    display.setTextSize(3);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(32, 16);
    display.println(STARTUP_TEXT);
    display.display();
    
    // Scroll diag right
    //display.startscrolldiagright(0x00, 0x07);
    // Scroll left
    display.startscrollleft(0x00, 0x0F);
    delay(SCREEN_STARTUP_DISPLAY_TIME);
    display.stopscroll();
    display.clearDisplay();
}

// Function to show bitmap image
void showBitmap() {
      display.clearDisplay();
      display.drawBitmap(0, 0, bitmap_image, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
      display.display();
      delay(SCREEN_STARTUP_DISPLAY_TIME);

      // Scroll right
      display.startscrollright(0x00, 0x0F);
      delay(SCREEN_STARTUP_DISPLAY_TIME);
      display.stopscroll();
      delay(100);
      display.clearDisplay();
}

// Function to display IP Info
void showIPInfo() {
  // TODO: Add an if oled display check
  // TODO: Wrap in bool if statemtn variable to show network info on oled
  display.clearDisplay();
  display.setCursor(0, 16);
  display.print("SSID:     ");
  display.println(WIFI_SSID);
  display.setCursor(0, 26);
  display.print("Hostname: ");
  display.println(WiFi.getHostname());
  display.setCursor(0, 36);
  display.print("IP:       ");
  display.println(WiFi.localIP().toString().c_str());

  // Custom Text
  // TODO: This gives a warning 
  if (SHOW_CUSTOM_TEXT || CUSTOM_TEXT == "") {
    display.setCursor(0, 56);
    display.print(CUSTOM_TEXT);
  }
  display.display();
}

// Function to update the OLED with sensor readings
//   - TODO: Avoid full screen redraws when only specific values change
void updateOLED(float co2, float temperature, float temperatureF, float humidity) {
  // Clear Display
  display.clearDisplay();

  // IP Address
  display.setCursor(0, 0);
  display.print("IP: ");
  display.println(WiFi.localIP().toString().c_str());

  // Temperature
  display.setCursor(0, 16);
  display.print("Temp: ");
  display.print(temperature);
  display.println(" C");

  // Temperature
  display.setCursor(0, 26);
  display.print("Temp: ");
  display.print(temperatureF);
  display.println(" F");

  // Humidity
  display.setCursor(0, 36);
  display.print("RH:   ");
  display.print(humidity);
  display.println(" %");

  // CO2
  display.setCursor(0, 46);
  display.print("CO2:  ");
  display.print(co2);
  display.println(" ppm");

  // Custom Text
  if (SHOW_CUSTOM_TEXT || CUSTOM_TEXT == "") {
    display.setCursor(0, 56);
    display.print(CUSTOM_TEXT);
  }

  // Update Display
  display.display();
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
    // Get sensor values
    float co2 = readCO2();
    float temperature = readTemperature();
    float temperatureF = celsiusToFahrenheit(temperature);
    float humidity = readHumidity();

    // Create a JSON document
    JsonDocument jsonDoc;

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
    updateOLED(co2, temperature, temperatureF, humidity);
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
}
