#pragma once

#include "DHTesp.h"

void dhtSetup(int pin);
float readTemperature();
float readHumidity();
int readCO2Level();
int readSoilMoisture();
String getAllMeasurements();
