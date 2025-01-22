/*
 * File: config.h
 *
 */

#pragma once

// ============================================================================
// CONFIGURATION --------------------------------------------------------------
// ============================================================================
// WIFI CONFIGURATION
constexpr char WIFI_SSID[]         = "";             // Wi-Fi SSID
constexpr char WIFI_PASSWORD[]     = "";             // Wi-Fi password
constexpr char WIFI_HOSTNAME[]     = "esp32-plant";  // Hostname
constexpr int  WIFI_TIMEOUT_TIME   = 30000;          // Timeout if unable to connect to WiFi (ms)
constexpr int  WIFI_CHECK_INTERVAL = 5000;           // Check WiFi time (ms)

// SERVER CONFIGURATION
constexpr uint16_t SERVER_PORT = 80;                 // Port for the web server
constexpr char     SERVER_PATH[] = "/";              // Path for serving the data

// PINS CONFIGURATION
constexpr uint8_t CO2_PIN = 35;                      // Analog pin for CO2 sensor
constexpr uint8_t DHT_PIN = 27;                      // GPIO pin for DHT sensor

// SCREEN CONFIGURATION
constexpr int     SCREEN_WIDTH                = 128;  // OLED display width, in pixels
constexpr int     SCREEN_HEIGHT               = 64;   // OLED display height, in pixels
constexpr uint8_t SCREEN_ADDRESS              = 0x3C; // Address of OLED display (could also be '0x3D' depending on screen resolution)
constexpr int     SCREEN_UPDATE_TIME          = 1000; // Time to wait before updating OLED (ms)
constexpr int     SCREEN_STARTUP_DISPLAY_TIME = 4000; // Startup screen delay time (ms)
bool              SHOW_STARTUP                = true; // Set to true to show the startup sequence
bool              SHOW_BITMAP                 = true; // Set to true to show the bitmap
bool              SHOW_CUSTOM_TEXT            = true; // Set to true to show custom text
constexpr char    STARTUP_TEXT[]              = "HI"; // Startup custom text
constexpr char    CUSTOM_TEXT[]               = "";   // OLED custom text "_____________________"

// SERIAL CONFIGURATION
constexpr int BAUD_RATE = 115200;                     // Baud rate
// ============================================================================
