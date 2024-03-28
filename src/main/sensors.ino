#include "sensors.h"

DHTesp dht;

void dhtSetup(int pin) {
  dht.setup(pin, DHTesp::DHT_MODEL_t::DHT22);
}

String getAllMeasurements() {
  String measurements  = "Temperature : " + String(readTemperature()) + " ºC\n";
         measurements += "Humidity    : " + String(readHumidity())    + " %\n" ;
         measurements += "Moisture    : " + String(readSoilMoisture())         ;
         measurements += "CO2 Level   : " + String(readCO2Level())    + " ppm" ;
  return measurements;
}

float readTemperature() {
  return dht.getTemperature();
}

float readHumidity() {
  return dht.getHumidity();
}

int readCO2Level() {
  static const float ReferenceVoltage  = 3.3;
  static const float MaxAdcValue       = 4095.0;
  static const float VoltageThreshold  = 0.4;
  static const float CalibrationFactor = 5000.0;
  static const float VoltageOffset     = 1.6;

  int adcVal = analogRead(CO2_ANALOG_PIN);                   // Read analog value from CO2 sensor
  float voltage = adcVal * (ReferenceVoltage / MaxAdcValue); // Calculate voltage based on ADC value

  // Calculate CO2 measurement based on voltage difference
  // If volate is 0
  if (voltage == 0) {
    Serial.println("CO2 level sensor not operating correctly");
    return -1; // Sensor not operating correctly
  }
  // If voltage is less than VoltageThreshold
  else if (voltage < VoltageThreshold) {
    Serial.println("CO2 level sensor pre-heating");
    return -2; // Sensor pre-heating
  }
  else {
    float voltageDifference = voltage - VoltageThreshold;
    return static_cast<int>((voltageDifference * CalibrationFactor) / VoltageOffset);
  }
}

int readSoilMoisture() {
  //static const float MaxAdcValue       = 4095.0; //TODO Return moisture percentage
  //static const float MinAdcValue       = 0.0;    //TODO Return moisture percentage

  int adcVal = analogRead(SOIL_ANALOG_PIN); // Read analog value from soil moisture sensor
  return static_cast<int>(adcVal)
}

void relaySetup(int pin) {
  pinMode(pin, OUTPUT);
}

void operateRelay() {
  static const int VoltageThreshold  = 2800;  // Define dry / wet threshold
  int soilMoistureValue = readSoilMoisture(); // Get soil moisture value

  // If 0
  if (soilMoistureValue == 0) {
    Serial.println("Soil Moisture sensor not operating correctly");
  }
  // If DRY
  else if (soilMoistureValue > VoltageThreshold) {
    Serial.println("Soil is DRY\nRunning pump for " + String(PUMP_SLEEP_TIME) + "Milliseconds");
    Serial.println("Turning pump ON")
    digitalWrite(RELAY_PIN, HIGH);     // Turn on the pump 
    delay(PUMP_SLEEP_TIME);            // Sleep for <PUMP_SLEEP_TIME> ms
    Serial.println("Turning pump OFF")
    digitalWrite(RELAY_PIN, LOW);      // Turn off the pump
    delay(PUMP_SLEEP_TIME);            // Sleep for <PUMP_SLEEP_TIME> ms
  }
  // If WET
  else if (soilMoistureValue < VoltageThreshold) {
    Serial.println("Soil is WET");
  }
  else {
    Serial.println("Soil is ELSE (this shouldnt happen often)")
  }
}
