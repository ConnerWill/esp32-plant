#pragma once

// WIFI
const char* const WIFI_SSID     = "YourWifiSSID";     // WiFi SSID (network name)
const char* const WIFI_PASSWORD = "YourWifiPassword"; // WiFi password
const char* const WIFI_HOSTNAME = "ESP32-plant";      // Hostname for ESP32

// PINS
const int CO2_ANALOG_PIN        = 35;                 // Analog pin for CO2 measurement
const int DHT_PIN               = 27;                 // DHT sensor pin for temperature and humidity measurement
const int SOIL_ANALOG_PIN       = 36;                 // Analog pin for soil moisture measurement
const int RELAY_PIN             = 16;                 // GPIO pin for relay control

// OTHER
const int WEBSERVER_PORT        = 80;                 // Webserver port
const int BAUD_RATE             = 115200;             // Baud rate for serial connection
const int PUMP_SLEEP_TIME       = 4000;               // Time in milliseconds to run the pump
