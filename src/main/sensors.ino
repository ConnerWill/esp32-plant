#include "sensors.h"

// Setup DHT sensor
DHTesp dht;
void dhtSetup(int pin) {
  dht.setup(pin, DHTesp::DHT_MODEL_t::DHT22);
}

// Function to combine all measurements into a format
String getAllMeasurements() {
  float temperatureC = readTemperature();
  String measurements  = "Temperature (F) : " + String(celsiusToFahrenheit(temperatureC)) + " F\n"; // º
         measurements += "Temperature (C) : " + String(temperatureC)                      + " C\n";
         measurements += "Humidity        : " + String(readHumidity())                    + " %\n";
         measurements += "Moisture        : " + String(readSoilMoisture())                +  "\n" ;
         measurements += "CO2 Level       : " + String(readCO2Level())                    + " ppm";
  return measurements;
}

// Function to convert Celsius to Fahrenheit
float celsiusToFahrenheit(float celsius) {
    float fahrenheit = (celsius * 9.0 / 5.0) + 32.0;
    return fahrenheit;
}

// Function to read temperature
float readTemperature() {
  return dht.getTemperature();
}

// Function to read humidity
float readHumidity() {
  return dht.getHumidity();
}

// Function to read CO2 level
int readCO2Level() {
  static const float ReferenceVoltage  = 3.3;
  static const float MaxAdcValue       = 4095.0;
  static const float VoltageThreshold  = 0.4;
  static const float CalibrationFactor = 5000.0;
  static const float VoltageOffset     = 1.6;

  int adcVal = analogRead(CO2_ANALOG_PIN);                   // Read analog value from CO2 sensor
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

// Function to read soil moisture
int readSoilMoisture() {
  // static const float VoltageThreshold = 2800.0; //TODO: Create Dry/Wet thresholds
  //static const float MaxAdcValue       = 4095.0; //TODO Return moisture percentage
  //static const float MinAdcValue       = 0.0;    //TODO Return moisture percentage

  int adcVal = analogRead(SOIL_ANALOG_PIN); // Read analog value from soil moisture sensor
  return static_cast<int>(adcVal);
}
