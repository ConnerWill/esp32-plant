#pragma once

const char* const WIFI_SSID     = "YourWifiSSID";     // WiFi SSID (network name)
const char* const WIFI_PASSWORD = "YourWifiPassword"; // WiFi password
const char* const WIFI_HOSTNAME = "esp32-plant";      // Hostname for ESP32
const int CO2_ANALOG_PIN        = 35;                 // Analog pin for CO2 measurement
const int DHT_PIN               = 27;                 // DHT sensor pin for temperature and humidity measurement
const int SOIL_ANALOG_PIN       = 36;                 // Analog pin for soil moisture measurement
const int WEBSERVER_PORT        = 80;                 // Webserver port
const int BAUD_RATE             = 115200;             // Baud rate for serial connection
