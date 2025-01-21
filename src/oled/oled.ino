#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "DHTesp.h"
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ============================================================================
// CONFIGURATION --------------------------------------------------------------
// ============================================================================
constexpr char WIFI_SSID[]     = "YourNetworkName"; // Wi-Fi SSID
constexpr char WIFI_PASSWORD[] = "YourNetworkPass"; // Wi-Fi password
constexpr char WIFI_HOSTNAME[] = "esp32-oled";      // Hostname

constexpr int  SERVER_PORT = 80;    // Port for the web server
constexpr char SERVER_PATH[] = "/"; // Path for serving the data

constexpr int CO2_PIN = 35; // Analog pin for CO2 sensor
constexpr int DHT_PIN = 27; // GPIO pin for DHT sensor

#define SCREEN_WIDTH 128        // OLED display width, in pixels
#define SCREEN_HEIGHT 64        // OLED display height, in pixels
#define SCREEN_UPDATE_TIME 1000 // Time to wait before updating OLED (ms)
#define SCREEN_ADDRESS 0x3C     // Address of OLED display (could also be '0x3D' depending on screen resolution)


constexpr int BAUD_RATE = 115200; // Baud rate
// ============================================================================

// ============================================================================
// GLOBAL INSTANCES -----------------------------------------------------------
// ============================================================================
DHTesp dht;
AsyncWebServer server(SERVER_PORT);

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ============================================================================

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

// Function to initialize OLED
void initOLED() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Failed to start SSD1306 OLED"));
    while (1);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.display();
}

// Function to update the OLED with sensor readings
void updateOLED(int co2, float temperature, float humidity) {
  display.clearDisplay();

  // IP Address
  display.setCursor(0, 0);
  display.print("IP: ");
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
}

// ============================================================================

void setup() {
  // Set co2 pin mode
  pinMode(CO2_PIN, INPUT);

  // Initialize DHT sensor
  dht.setup(DHT_PIN);

  // Initialize Serial for debugging
  Serial.begin(BAUD_RATE);

  // Initialize OLED
  initOLED();

  // Connect to Wi-Fi
  connectToWiFi();

  // Define the root endpoint
  server.on(SERVER_PATH, HTTP_GET, [](AsyncWebServerRequest* request) {
    // Create a JSON document
    StaticJsonDocument<256> jsonDoc;

    // Get sensor values
    int co2 = getCo2Measurement();
    float temperature = dht.getTemperature();
    float humidity = dht.getHumidity();

    // Add values to the JSON document
    jsonDoc["co2"]         = co2;
    jsonDoc["temperature"] = isnan(temperature) ? 0 : temperature; // Handle NaN
    jsonDoc["humidity"]    = isnan(humidity)    ? 0 : humidity;    // Handle NaN

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

void loop() {

// WE MIGHT WANT TO UPDATE THE DISPLAY IN LOOP INSTEAD OF SETUP
  // Get sensor values
  int co2 = getCo2Measurement();
  float temperature = dht.getTemperature();
  float humidity = dht.getHumidity();

  // Debug values
  Serial.printf("Temperature: %s C\n", temperature);
  Serial.printf("Humidity   : %s %%\n", humidity);
  Serial.printf("co2        : %s ppm\n", co2);
  Serial.println("Updating display...");

  // Update OLED display with latest sensor data
  updateOLED(co2, temperature, humidity);

  delay(SCREEN_UPDATE_TIME);
}
