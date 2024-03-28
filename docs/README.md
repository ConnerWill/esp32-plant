# ESP32 Plant Monitoring / Watering System

![ESP32 Plant Monitoring Project Image](/static/img/project.png)

---

# Overview

This project implements a monitoring system using an ESP32 microcontroller to measure and display temperature, humidity, and CO2 levels in real-time.

This is designed to be used to monitor the environment when growing plants

---

# Shopping List

Here is a list of parts used in this project

> *Amazon links are non-affiliate*

| NAME           | PRICE | COUNT | DESCRIPTION                     | LINK                                                                                                | NOTE                                               |
|----------------|-------|-------|---------------------------------|-----------------------------------------------------------------------------------------------------|----------------------------------------------------|
| ESP32          | $5    | 1     | Microcontroller                 | https://www.amazon.com/ESP-WROOM-31-Development-Microcontroller-Integrated-Compatible/dp/B08D5ZD528 |                                                    |
| DHT22          | $7    | 1     | Temp/Humidity Sensor            | https://www.amazon.com/gp/product/B0795F19W6                                                        |                                                    |
| MQ-135         | $3    | 1     | CO2 Sensor                      | https://www.amazon.com/Ximimark-Quality-Hazardous-Detection-Arduino/dp/B07L73VTTY                   |                                                    |
| 1K Ω Resistor  | $0.25 | 1     | 1K Ohm Resistor                 | https://www.amazon.com/California-JOS-Carbon-Resistor-Tolerance/dp/B0BR66ZN6B                       | MQ-135 analog pin                                  |
| 10K Ω Resistor | $0.25 | 0/1   | 10K Ohm Resistor                | https://www.amazon.com/California-JOS-Carbon-Resistor-Tolerance/dp/B0BR67DJHM                       | Most DHT22 sensor modules have a built-in resistor |
| Wire/Breadboard| $1    | 1     | Jumper wire or breadboard       | https://www.amazon.com/DEYUE-breadboard-Set-Prototype-Board/dp/B07LFD4LT6                           | Optional depending on setup                        |
| TLC555         | $3    | 1     | Capacitive soil moisture sensor | https://www.amazon.com/dp/B0C5STDNSX                                                                |                                                    |


<!--
| TLC555         | $3    | 1     | Capacitive soil moisture sensor | https://www.amazon.com/dp/B0C5STDNSX                                                                |                                                    |
| SSD1306        | $6    | 1     | 0.96" 128x64 OLED         | https://www.amazon.com/HiLetgo-Serial-128X64-Display-Color/dp/B06XRBTBTB                            | Optional if you want a screen display              |
| ENS160       | $5    | 0/1   | CO2 Sensor           | https://www.amazon.com/HiLetgo-Dioxide-Temperature-Humidity-Monitoring/dp/B0CDWYFSGH                |
| SGP30        | $5    | 0/1   | CO2 Sensor           | https://www.makerfabs.com/sgp30-air-quality-sensor-breakout.html                                    |
-->

> *Lower prices can be found if you shop around*


---

# Setup

Once you have all the required parts in hand, you can start wiring


## Circuit Schematic

![ESP32 Plant Monitoring Circuit Schematic Image](/static/img/circuit-schematic.png)


| ESP32 Pin | Destination | Type    | Power Requirements (Volts) | ESP32 VCC Pin | ESP32 GND Pin |
|-----------|-------------|---------|----------------------------|---------------|---------------|
| 27        | DHT22       | Digital | 3.3 - 5                    | 3.3V          | GND           |
| 35        | CO2 Sensor  | Analog  | 5                          | 5V            | GND           |
| 36        | TLC555      | Analog  | 5                          | 5V            | GND           |

> *Feel free to change the pins used on the ESP32, just note, if you change the pins, you will need to update the values in the code.*


## Configuration

You will need to define values to suite your needs. Variables are defined in [`./src/main/config.h`](/src/main/config.h)

### WiFI Configuration

Here is a list of WiFi variables that **must** be defined and their descriptions...

`WIFI_SSID`
: WiFi SSID (network/WiFi name)

`WIFI_PASSWORD`
: WiFi password

`WIFI_HOSTNAME`
: Hostname of ESP32 to set

### Other Configuration

Other variables in [`config.h`](/src/main/config.h) can be changed to suite your needs

## Uploading

I use Arduino IDE to upload the code to the board. I had to install additional drivers ***[(Download)](https://www.silabs.com/documents/public/software/CP210x_Universal_Windows_Driver.zip)*** to get connected to my board.

I installed the ESP32 boards and selected ESP32dev board.

---

# 3D Print

![ESP32 Plant Monitoring Housing Image](/static/img/3d-housing.png)

The housing to hold the ESP32 and sensors can be 3D printed.

The 3D model files are located under the [3D-models](/3D-models) directory.

## DHT22 Case

![DHT22 3D Printed Case](/static/img/DHT22-3D-model.jpg)

[Over-engineered DHT22 / AM2302 Board Case](https://www.thingiverse.com/thing:4521313)
: (Credit [SciMonster](https://www.thingiverse.com/SciMonster))


## MQ-135 Case

![MQ-135 3D Printed Case](/static/img/MQ-135-3D-model.jpg)

[MQ-2 Sensor Case](https://www.thingiverse.com/thing:2893581)
: (Credit [RJGII](https://www.thingiverse.com/RJGII))

---

# TODO

* Add wiring/setup instructions
* Add 3D models
* Replace images
* Improve code
* Add plotting

---

# Links

* https://microcontrollerslab.com/esp32-dht11-dht22-web-server
* https://blog.asksensors.com/air-quality-sensor-mq135-cloud-mqtt
* https://maker.pro/everything-esp/projects/how-to-control-a-relay-module-with-esp32
* https://www.thingiverse.com/thing:4521313
* https://www.thingiverse.com/thing:2893581

---

