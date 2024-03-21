#include "sensors.h"

DHTesp dht;

void dhtSetup(int pin) {
  dht.setup(pin, DHTesp::DHT_MODEL_t::DHT22);
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

