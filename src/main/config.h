/*
 * File: config.h
 *
 */

#pragma once

// WIFI & Server Configuration
const char* const WIFI_SSID     = "YourWifiSSID";     // WiFi SSID (network name)
const char* const WIFI_PASSWORD = "YourWifiPassword"; // WiFi password
const char* const WIFI_HOSTNAME = "esp32-plant";      // Hostname for ESP32
const int WEBSERVER_PORT        = 80;                 // Webserver port

// Pins Configuration
const int CO2_ANALOG_PIN        = 35; // Analog pin for CO2 measurement
const int DHT_PIN               = 27; // DHT sensor pin for temperature and humidity measurement

// OLED Configuration
const int OLED_SDA_PIN = 21; // SDA pin for OLED (default for many ESP32 boards)
const int OLED_SCL_PIN = 22; // SCL pin for OLED (default for many ESP32 boards)
const int OLED_WIDTH  = 128; // OLED pixel width
const int OLED_HEIGHT = 64;  // OLED pixel height
const int OLED_RESET  = -1;  // Set to -1 if not used

// Misc Configuration
const int BAUD_RATE             = 115200;             // Baud rate for serial connection
