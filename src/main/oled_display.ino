/*
 * File: oled_display.ino
 *
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "oled_display.h"

// Create an SSD1306 display object
Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET);

// Setup OLED display
void oledSetup() {
  // Initialize I2C communication with defined pins
  Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);

  // Initialize the OLED display
  if (!display.begin(SSD1306_I2C_ADDRESS, 0x3C)) { // Default I2C address for OLED is 0x3C
    Serial.println(F("OLED initialization failed!"));
    for (;;); // Infinite loop on failure
  }
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.println(F("OLED Initialized!"));
  display.display();
}

// Display sensor data on OLED
void displaySensorData(float temperatureF, float humidity, int co2Level) {
  static int count = 0;
  static float tempSum = 0, humSum = 0;
  static int co2Sum = 0;

  // Update rolling averages every 10 readings
  tempSum += temperatureF;
  humSum += humidity;
  co2Sum += co2Level;
  count++;

  float avgTemp = tempSum / count;
  float avgHum = humSum / count;
  int avgCO2 = co2Sum / count;

  // Clear display
  display.clearDisplay();

  // Display data
  display.setCursor(0, 0);
  display.print(F("Temp: "));
  display.print(temperatureF);
  display.println(F(" C"));

  display.print(F("Humidity: "));
  display.print(humidity);
  display.println(F(" %"));

  display.print(F("CO2: "));
  display.print(co2Level);
  display.println(F(" ppm"));

  // Display averages
  display.println();
  display.print(F("Avg Temp: "));
  display.print(avgTemp);
  display.println(F(" C"));

  display.print(F("Avg Hum: "));
  display.print(avgHum);
  display.println(F(" %"));

  display.print(F("Avg CO2: "));
  display.print(avgCO2);
  display.println(F(" ppm"));

  // Update display
  display.display();
}
