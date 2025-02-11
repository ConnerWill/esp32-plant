# ESP32 Plant Monitoring System

<div align="center">

![ESP32 Plant Monitoring Project Image](/static/img/project.png)

[![Arduino Lint](https://github.com/ConnerWill/esp32-plant/actions/workflows/arduino_lint.yml/badge.svg)](https://github.com/ConnerWill/esp32-plant/actions/workflows/arduino_lint.yml)
[![Python Black](https://github.com/ConnerWill/esp32-plant/actions/workflows/python_black.yml/badge.svg)](https://github.com/ConnerWill/esp32-plant/actions/workflows/python_black.yml)
[![ShellCheck](https://github.com/ConnerWill/esp32-plant/actions/workflows/shellcheck.yml/badge.svg)](https://github.com/ConnerWill/esp32-plant/actions/workflows/shellcheck.yml)

</div>

---

## Table of Contents

<!--toc:start-->
- [ESP32 Plant Monitoring System](#esp32-plant-monitoring-system)
  - [Table of Contents](#table-of-contents)
  - [Overview](#overview)
  - [Shopping List](#shopping-list)
  - [Setup](#setup)
    - [Circuit Schematic](#circuit-schematic)
    - [ESP32 Pinout](#esp32-pinout)
      - [30 Pin](#30-pin)
      - [38 Pin](#38-pin)
    - [Configuration](#configuration)
      - [WiFI Configuration](#wifi-configuration)
      - [Other Configuration](#other-configuration)
    - [Libraries](#libraries)
      - [External Libraries](#external-libraries)
    - [Uploading](#uploading)
  - [Images](#images)
  - [3D Print](#3d-print)
  - [TODO](#todo)
  - [Links](#links)
<!--toc:end-->

---

## Overview

This project implements a monitoring system using an ESP32 microcontroller to
measure and display temperature, humidity, and CO2 levels in real-time.

This is designed to be used to monitor the environment when growing plants

---

## Shopping List

Here is a list of parts used in this project

> [!NOTE]
> *Amazon links are non-affiliate*

| NAME             | PRICE   | COUNT | DESCRIPTION                           | LINK   | NOTE                                               |
|------------------|---------|-------|---------------------------------------|--------|----------------------------------------------------|
| ESP32            | $5      | 1     | Microcontroller                       | [Amazon](https://www.amazon.com/ESP-WROOM-31-Development-Microcontroller-Integrated-Compatible/dp/B08D5ZD528) | [USB-C ESP32 Amazon](https://www.amazon.com/dp/B0CR5Y2JVD)                                 |
| DHT22            | $3      | 1     | Temp/Humidity Sensor                  | [Amazon](https://www.amazon.com/dp/B0CPHQC9SF) |                                                    |
| MQ-135           | $3      | 1     | CO2 Sensor                            | [Amazon](https://www.amazon.com/Ximimark-Quality-Hazardous-Detection-Arduino/dp/B07L73VTTY) |                                                    |
| OLED Display     | $3      | 1     | 0.96" OLED Display 128x64 SSD1306     | [Amazon](https://www.amazon.com/Hosyond-Display-Self-Luminous-Compatible-Raspberry/dp/B09C5K91H7) |                                                    |
| Kasa Smart Plug  | $7.50   | 2     | TP-Link Kasa Smart Plug HS103         | [Amazon](https://www.amazon.com/dp/B07RCNB2L3) | Intake and exhaust fans smart plugs                |
| Rocker Switch    | $0.60   | 2     | Rocker Switch On/Off 2 Pin            | [Amazon](https://www.amazon.com/dp/B0CVTRKJTV) |                                                    |
| 1K Ω Resistor    | $0.25   | 1     | 1K Ohm Resistor                       | [Amazon](https://www.amazon.com/California-JOS-Carbon-Resistor-Tolerance/dp/B0BR66ZN6B) | MQ-135 analog pin                                  |
| 10K Ω Resistor   | $0.25   | 0/1/2 | 10K Ohm Resistor                      | [Amazon](https://www.amazon.com/California-JOS-Carbon-Resistor-Tolerance/dp/B0BR67DJHM) | Most DHT22 sensor modules have a built-in resistor |
| Wire/Breadboard  | $1      | 0/1   | Jumper wire or breadboard             | [Amazon](https://www.amazon.com/DEYUE-breadboard-Set-Prototype-Board/dp/B07LFD4LT6) | Optional depending on setup                        |
| Breakout Board   | $4      | 0/1   | ESP32 Breakout Board GPIO             | [Amazon](https://www.amazon.com/dp/B0BNQ8V65G) | Optional depending on setup                        |
| Buck Converter   | $1      | 0/1   | LM2596 DC-DC Buck Converter Step Down | [Amazon](https://www.amazon.com/dp/B07VVXF7YX) | Optional depending on setup                        |
| Barrel Jack      | $1      | 0/1   | Female DC Barrel Jack Panel Adapter   | [Amazon](https://www.amazon.com/Threaded-Adapter-Connector-Dustproof-lkelyonewy/dp/B091PS6XQ4) | Optional depending on setup                        |
| 1N5819 Diode     | $0.1    | 0/1   | 1N5819 Schottky Diode                 | [Amazon](https://www.amazon.com/ALLECIN-1N5819-Schottky-Rectifier-Switching/dp/B0CKSHSWPS) | Optional depending on setup                        |
| 1000µF Capacitor | $0.4    | 0/1   | 1000uF 10V 8x12mm Capacitor           | [Amazon](https://www.amazon.com/ALLECIN-Electrolytic-Capacitor-0-32x0-47in-Capacitors/dp/B0CMQCC5D3) | Optional depending on setup                        |

> [!TIP]
> *Lower prices can be found if you shop around*

---

## Setup

Once you have all the required parts in hand, you can start wiring

### Circuit Schematic

[![ESP32 Plant Wiring](/static/img/esp32-wiring.jpeg)](https://wokwi.com/projects/421434545777653761)

![ESP32 Plant Monitoring Circuit Schematic Image](/static/img/circuit-schematic.png)

![ESP32 OLED Circuit Schematic Image](/static/img/esp32-oled-schematic.jpg)

| ESP32 Pin | Destination   | Type    | Power Requirements (Volts) | ESP32 VCC Pin | ESP32 GND Pin |
|-----------|---------------|---------|----------------------------|---------------|---------------|
| 27        | DHT22         | Digital | 3.3 - 5                    | 3.3V          | GND           |
| 35        | CO2 Sensor    | Analog  | 5                          | 5V            | GND           |
| 21        | OLED SCL      | SCL     | 3.3                        | 3.3V          | GND           |
| 22        | OLED SDA      | SDA     | 3.3                        | 3.3V          | GND           |
| 19        | Rocker Switch | GPIO    |                            |               | GND           |

> [!NOTE]
> *Feel free to change the pins used on the ESP32, just note, if you change the pins, you will need to update the values in the code.*

### ESP32 Pinout

#### 30 Pin

![ESP32 30 pin Pinout Image](/static/img/esp32-pinout-30-pin.png)

#### 38 Pin

![ESP32 38 pin Pinout Image](/static/img/esp32-pinout-38-pin.jpg)

### Configuration

You will need to define values to suite your needs. Variables are defined in [`config.h`](/src/main/config.h)

#### WiFI Configuration

Here is a list of WiFi variables that **must** be defined and their descriptions...

`WIFI_SSID`
: WiFi SSID (network/WiFi name)

`WIFI_PASSWORD`
: WiFi password

`WIFI_HOSTNAME`
: Hostname of ESP32 to set

#### Other Configuration

Other variables in the code can be changed to suite your needs

### Libraries

You will need to install additional libraries.

In Arduino IDE, go to library manager and search and install the
required libraries listed in the `DEPENDENCIES` section in [`main.ino`](/src/main/main.ino)

#### External Libraries

- [KasaSmartPlug Github](https://github.com/ConnerWill/KasaSmartPlug)

---

### Uploading

I use Arduino IDE to upload the code to the board. I had to install additional drivers ***[(Download)](https://www.silabs.com/documents/public/software/CP210x_Universal_Windows_Driver.zip)*** to get connected to my board.

I installed the ESP32 boards and selected ESP32dev board.

---

## Images

<!--TODO: Add more images-->

|                                      |                                      |
|--------------------------------------|--------------------------------------|
| ![ESP32 Plant Monitoring Project Image 1](/static/img/esp32-plant-image1.jpg) | ![ESP32 Plant Monitoring Project Image 2](/static/img/esp32-plant-image2.jpg) |
| ![ESP32 Plant Monitoring Project Image](/static/img/project.png) | ![ESP32 Plant Monitoring Project Image](/static/img/project.png) |

---

## 3D Print

![ESP32 Plant Monitoring Housing 3D Image](/static/img/ESP32-Housing.jpg)

The housing to hold the ESP32 can be 3D printed.

The 3D model files are located under the [3D-models](/3D-models) directory.

| Name             | Image                  | Link                                      | Author     |
|------------------|------------------------|-------------------------------------------|------------|
| ESP32-Plant Case | ![ESP32-Plant Case Image](/static/img/ESP32-Housing.jpg) | [ESP32-Plant Case Case](/3D-models/ESP32-housing/)                     | [ConnerWill](https://github.com/ConnerWill) |
| DHT22 Case       | ![DHT22 Case Image](/static/img/DHT22-3D-model.jpg)       | [Over-engineered DHT22 / AM2302 Board Case](https://www.thingiverse.com/thing:4521313) | [SciMonster](https://www.thingiverse.com/SciMonster) |
| MQ-135 Case      | ![MQ-135 Case Image](/static/img/MQ-135-3D-model.jpg)      | [MQ-2 Sensor Case](https://www.thingiverse.com/thing:2893581)                          | [RJGII](https://www.thingiverse.com/RJGII)      |

---

## TODO

- [ ] Improve wiring/setup instructions
- [ ] Finish 3D models
- [ ] Add finished product images
- [ ] Improve code

---

## Links

- [microcontrollerslab.com/esp32-dht11-dht22-web-server](https://microcontrollerslab.com/esp32-dht11-dht22-web-server)
- [blog.asksensors.com/air-quality-sensor-mq135-cloud-mqtt](https://blog.asksensors.com/air-quality-sensor-mq135-cloud-mqtt)
- [maker.pro/everything-esp/projects/how-to-control-a-relay-module-with-esp32](https://maker.pro/everything-esp/projects/how-to-control-a-relay-module-with-esp32)
- [thingiverse.com/thing:4521313](https://www.thingiverse.com/thing:4521313)
- [thingiverse.com/thing:2893581](https://www.thingiverse.com/thing:2893581)
- [javl.github.io/image2cpp](https://javl.github.io/image2cpp)
- [github.com/ConnerWill/KasaSmartPlug](https://github.com/ConnerWill/KasaSmartPlug)
- [randomnerdtutorials.com/esp32-wi-fi-manager-asyncwebserver](https://randomnerdtutorials.com/esp32-wi-fi-manager-asyncwebserver)

---
