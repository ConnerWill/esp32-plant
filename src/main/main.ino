/*
 * File: ./main.ino
 */

// ============================================================================
// DEPENDENCIES -----------------------------------------------------------
// ============================================================================
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "DHTesp.h"
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "KasaSmartPlug.h"     // KASA TP-link smart plug library: https://github.com/ConnerWill/KasaSmartPlug
#include "config.h"            // Include config header file
#include "bitmap.h"            // Include bitmap header file
// ============================================================================

// ============================================================================
// GLOBAL INSTANCES -----------------------------------------------------------
// ============================================================================
DHTesp dht;                                                       // DHT
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
AsyncWebServer server(SERVER_PORT);                               // Define server on port
KASAUtil kasaUtil;                                                // Kasa utility object
KASASmartPlug *intakePlug = NULL;                                 // Smart plug pointers (Intake)
KASASmartPlug *exhaustPlug = NULL;                                // Smart plug pointers (Exhaust)
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
// KASA TP-Link Smart Plug Functions
// -------------------------------------
// Function to initialize smart plugs
void initSmartPlugs() {
  int found = kasaUtil.ScanDevices();
  Serial.printf("Found %d Kasa devices\n", found);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("KASA devices found: ");
  display.println(found);
  display.display();
  delay(500);

  // Loop through found devices and match aliases
  for (int i = 0; i < found; i++) {
    KASASmartPlug *plug = kasaUtil.GetSmartPlugByIndex(i);
    if (plug == NULL) continue;

    Serial.printf("Found Plug: %s (IP: %s, State: %d)\n", plug->alias, plug->ip_address, plug->state);

    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("KASA Smart Plugs");
    display.setCursor(0, 16);
    display.print("Alias: ");
    display.println(plug->alias);
    display.print("IP   : ");
    display.println(plug->ip_address);
    display.print("State   : ");
    display.println(plug->state);
    display.display();
    delay(1000);

    // Match plugs by their aliases
    if (strcmp(plug->alias, intakePlugAlias) == 0) {
      intakePlug = plug;
      Serial.println("Intake plug initialized.");
    } else if (strcmp(plug->alias, exhaustPlugAlias) == 0) {
      exhaustPlug = plug;
      Serial.println("Exhaust plug initialized.");
    }
  }

  // Check if plugs were found
  if (intakePlug == NULL) {
    Serial.println("Error: Intake plug not found!");

    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("ERROR: Intake plug not found");
    display.display();
    delay(1000);
  }
  if (exhaustPlug == NULL) {
    Serial.println("Error: Exhaust plug not found!");

    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("ERROR: Exhaust plug not found");
    display.display();
    delay(1000);
  }
}

// Function to turn a plug on or off
void setPlugState(KASASmartPlug* plug, bool state) {
  // Setting variable for printing. otherwise dont need this
  //TODO: Figure out how to turn this into a string
  KASASmartPlug *plugAlias = kasaUtil.GetSmartPlug(plug->alias);

  if (plug == NULL) {
    //Serial.printf("Error: Could not find plug with alias '%s'\n", plugAlias);
    return;
  }

  if (state) {
    //Serial.printf("Turning ON plug: %s\n", plugAlias);
    plug->SetRelayState(1); // Turn on
  } else {
    //Serial.printf("Turning OFF plug: %s\n", plugAlias);
    plug->SetRelayState(0); // Turn off
  }
}

//TODO: Function to get plug state

// -------------------------------------
// WIFI Functions
// -------------------------------------
// Function to connect to Wi-Fi
void connectToWiFi() {
  Serial.println(F("Setting up Wi-Fi..."));

  WiFi.mode(WIFI_STA); // Station mode only

  if (!WiFi.setHostname(WIFI_HOSTNAME)) {
    Serial.printf("Error: Failed to set Wi-Fi hostname: %s\n", WIFI_HOSTNAME);
  }

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

      // TODO: Values are not updating when wifi is disconnected
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("Failed to connect to WiFi!");
      display.setCursor(0, 16);
      display.println(WIFI_SSID);
      display.display();
      delay(SCREEN_STARTUP_DISPLAY_TIME);
      display.clearDisplay();

      return;
    }
    delay(1000);
    Serial.print(".");

    // Connecting loading bar
    display.print("_");
    display.display();
  }

  Serial.println(F("Wi-Fi Connected"));

  // Display IP info
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
  }
  // Display bitmap if SHOW_BITMAP is true
  if (SHOW_BITMAP) {
    showBitmap();
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
  // Print IP info
  Serial.printf("SSID    : %s\n", WIFI_SSID);
  Serial.printf("HOSTNAME: %s\n", WiFi.getHostname());
  Serial.printf("IP      : %s\n", WiFi.localIP().toString().c_str());

  if (SHOW_IP_INFO) {
    display.clearDisplay();
    display.setCursor(0, 16);
    display.print("SSID: ");
    display.println(WIFI_SSID);
    display.setCursor(0, 26);
    display.print("Host: ");
    display.println(WiFi.getHostname());
    display.setCursor(0, 36);
    display.print("IP:   ");
    display.println(WiFi.localIP().toString().c_str());

    // Custom Text
    if (SHOW_CUSTOM_TEXT || strlen(CUSTOM_TEXT) == 0) {
      display.setCursor(0, 56);
      display.print(CUSTOM_TEXT);
    }
    display.display();
  }
}

// Function to update the OLED with sensor readings
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
  if (SHOW_CUSTOM_TEXT || strlen(CUSTOM_TEXT) == 0) {
    display.setCursor(0, 56);
    display.print(CUSTOM_TEXT);
  }

  // Update Display
  display.display();
}
// ============================================================================

// ============================================================================
// SETUP ----------------------------------------------------------------------
// ============================================================================
void setup() {
  pinMode(CO2_PIN, INPUT);                        // Set co2 pin mode
  dht.setup(DHT_PIN, DHTesp::DHT_MODEL_t::DHT22); // Initialize DHT sensor
  Serial.begin(BAUD_RATE);                        // Initialize Serial for debugging
  initOLED();                                     // Initialize OLED
  connectToWiFi();                                // Connect to Wi-Fi
  initSmartPlugs();                               // Initialize Smart Plugs

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
// ============================================================================

// ============================================================================
// LOOP -----------------------------------------------------------------------
// ============================================================================
void loop() {
  static unsigned long lastUpdateTime = 0;
  static unsigned long lastWiFiCheck = 0;
  static unsigned long lastBitmapCheck = 0;
  static unsigned long lastPlugCheck = 0;
  unsigned long currentTime = millis();

  // Update OLED display
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
    Serial.println(F("Updating display..."));

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

/*
 * Temperature Too High -> Turn on the exhaust fan, consider additional cooling (e.g., A/C).
 * Temperature Too Low  -> Turn off the exhaust fan, possibly add a heater.
 * Humidity Too High    -> Turn on the exhaust fan to vent humid air, or use a dehumidifier.
 * Humidity Too Low     -> Turn off the exhaust fan, reduce intake, and add a humidifier if needed.
 * CO2 Too High         -> Turn on the exhaust fan to vent excess CO2.
 * CO2 Too Low          -> Turn off the exhaust fan to retain CO2, or add a CO2 source.
*/

  // Update Smart plugs
  if (currentTime - lastPlugCheck >= SMARTPLUG_UPDATE_TIME) {
    lastPlugCheck = currentTime;

    // Get sensor values
    float co2 = readCO2();
    float temperature = readTemperature();
    float temperatureF = celsiusToFahrenheit(temperature);
    float humidity = readHumidity();

    // Temperature control logic
    if (temperatureF > DESIRED_TEMP + TEMP_HYSTERESIS) { // Temp too high
      Serial.println(F("Temperature too high! Turning on intake and exhaust fans..."));
      setPlugState(exhaustPlug, true);  // Turn on exhaust fan
      setPlugState(intakePlug, true);   // Turn on intake fan
    } else if (temperatureF < DESIRED_TEMP - TEMP_HYSTERESIS) { // Temp too low
      Serial.println(F("Temperature too low! Turning off intake and exhaust fans..."));
      setPlugState(exhaustPlug, false); // Turn off exhaust fan
      setPlugState(intakePlug, false);  // Turn off intake fan
    } else {
      // If temperature is within the target range, turn off fans to save energy
      setPlugState(exhaustPlug, false); // Turn off exhaust fan
      setPlugState(intakePlug, false);  // Turn off intake fan
    }

    // Humidity control logic
    if (humidity > DESIRED_HUMIDITY + HUMIDITY_HYSTERESIS) {             // Humidity too high
      Serial.println(F("Humidity too high! Turning off humidifier..."));
      setPlugState(humidifierPlugAlias, false);                          // Turn off humidifier
    } else if (humidity < DESIRED_HUMIDITY - HUMIDITY_HYSTERESIS) {      // Humidity too low
      Serial.println(F("Humidity too low! Turning on humidifier..."));
      setPlugState(humidifierPlugAlias, true);                           // Turn on humidifier
    } else {
      // If humidity is within the target range, turn off humidifier to save energy
      setPlugState(humidifierPlugAlias, false); // Turn off humidifier
    }

    //TODO: Add CO2 levels
  }

  // Periodically show bitmap
  if (INTERRUPT_WITH_BITMAP) {
    if (currentTime - lastBitmapCheck >= INTERRUPT_BITMAP_TIME) {
      lastBitmapCheck = currentTime;
      showBitmap();
    }
  }
}
// ============================================================================
