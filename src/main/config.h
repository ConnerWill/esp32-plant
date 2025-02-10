/*
 * File: config.h
 *
 */

#pragma once

// ============================================================================
// CONFIGURATION --------------------------------------------------------------
// ============================================================================
// WIFI CONFIGURATION
constexpr char WIFI_SSID[]         = "";                 // Wi-Fi SSID
constexpr char WIFI_PASSWORD[]     = "";                 // Wi-Fi password
constexpr char WIFI_HOSTNAME[]     = "esp32-plant";      // Hostname
constexpr int  WIFI_TIMEOUT_TIME   = 30000;              // Timeout if unable to connect to WiFi (ms)
constexpr int  WIFI_CHECK_INTERVAL = 5000;               // Check WiFi time (ms)

// SERVER CONFIGURATION
constexpr uint16_t SERVER_PORT = 80;                     // Port for the web server
constexpr char     SERVER_PATH[] = "/";                  // Path for serving the data

// PINS CONFIGURATION
constexpr uint8_t CO2_PIN           = 35;                // Analog pin for CO2 sensor
constexpr uint8_t DHT_PIN           = 27;                // GPIO pin for DHT sensor
constexpr uint8_t ROCKER_SWITCH_PIN = 19;                // GPIO pin for flower rocker switch

// SCREEN CONFIGURATION
constexpr int     SCREEN_WIDTH                = 128;     // OLED display width, in pixels
constexpr int     SCREEN_HEIGHT               = 64;      // OLED display height, in pixels
constexpr uint8_t SCREEN_ADDRESS              = 0x3C;    // Address of OLED display (could also be '0x3D' depending on screen resolution)
constexpr int     SCREEN_UPDATE_TIME          = 1000;    // Time to wait before updating OLED (ms)
constexpr int     SCREEN_STARTUP_DISPLAY_TIME = 3000;    // Startup screen delay time (ms)
bool              SHOW_STARTUP                = true;    // Set to true to show the startup sequence
bool              SHOW_BITMAP                 = true;    // Set to true to show the bitmap
bool              SHOW_IP_INFO                = true;    // Set to true to show IP information
constexpr char    STARTUP_TEXT[]              = ":)";    // Startup custom text
bool              INTERRUPT_WITH_BITMAP       = true;    // Periodically show bitmap
constexpr int     INTERRUPT_BITMAP_TIME       = 300000;  // Check WiFi time (ms)

// SMART PLUG CONFIGURATION
const char      *intakePlugAlias        = "tent1_intake";     // Kasa plug alias (Intake)
const char      *exhaustPlugAlias       = "tent1_exhaust";    // Kasa plug aliases (Exhaust)
const char      *humidifierPlugAlias    = "tent1_humidifier"; // Kasa plug aliases (Humidifier)
const char      *lightPlugAlias         = "tent1_light";      // Kasa plug aliases (Lights)
constexpr int   SMARTPLUG_UPDATE_TIME   = 30000;              // Update smart plugs time (ms)
bool            FLOWER                  = false;              // Flower default value (this changes based on rocker switch)
constexpr float DESIRED_TEMP_VEG        = 80.0;               // Desired temperature in F (veg)
constexpr float DESIRED_HUMIDITY_VEG    = 50.0;               // Desired humidity in percentage (veg)
constexpr float DESIRED_CO2_VEG         = 800.0;              // Desired CO2 level (ppm) (veg)
constexpr float DESIRED_TEMP_FLOWER     = 70.0;               // Desired temperature in F (flower)
constexpr float DESIRED_HUMIDITY_FLOWER = 45.0;               // Desired humidity in percentage (flower)
constexpr float DESIRED_CO2_FLOWER      = 1000.0;             // Desired CO2 level (ppm) (flower)
constexpr float TEMP_HYSTERESIS         = 1.0;                // Temperature hysteresis to prevent rapid switching
constexpr float HUMIDITY_HYSTERESIS     = 5.0;                // Humidity hysteresis to prevent rapid switching
constexpr float CO2_HYSTERESIS          = 100.0;              // CO2 hysteresis to prevent rapid switching

// TIME / LIGHT CONFIGURATION
constexpr int    TIMEZONE_OFFSET            = -6 * 3600;      // Adjust for your timezone (CST example)
constexpr int    NTP_UPDATE_INTERVAL        = 86400;          // Sync every 24 hours (seconds)
constexpr int    RTC_CHECK_INTERVAL         = 60000;          // Check RTC time (ms)
constexpr char   NTP_SERVER[]               = "pool.ntp.org"; // NTP server address
bool             ENABLE_NTP_SYNC            = true;           // Enable or disable NTP synchronization
bool             ENABLE_RTC                 = true;           // Enable RTC synchronization
constexpr int    RTC_I2C_ADDRESS            = 0x68;           // RTC module I2C address (DS3231 default)
constexpr int    LIGHTS_ON_HOUR_FLOWER_ON   = 18;             // Lights on hour when flower mode is active (6:00 PM)
constexpr int    LIGHTS_OFF_HOUR_FLOWER_ON  = 6;              // Lights off hour when flower mode is active (6:00 AM)
constexpr int    LIGHTS_ON_HOUR_FLOWER_OFF  = 6;              // Lights on hour when flower mode is inactive (6:00 AM)
constexpr int    LIGHTS_OFF_HOUR_FLOWER_OFF = 22;             // Lights off hour when flower mode is inactive (10:00 PM)

// SERIAL CONFIGURATION
constexpr int BAUD_RATE = 115200;                        // Baud rate
// ============================================================================
//vim: filetype=arduino:shiftwidth=2:softtabstop=2:expandtab:nowrap:cursorline:cursorcolumn:number:relativenumber
