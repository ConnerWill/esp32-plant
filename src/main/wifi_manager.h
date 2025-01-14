/*
 * File: wifi_manager.h
 *
 */

#pragma once

#include <WiFi.h>

void connectWiFi(const char* ssid, const char* password);
void checkWiFiConnection();
