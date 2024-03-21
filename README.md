# ESP32 Plant Monitoring / Watering System

![ESP32 Plant Monitoring Project Image](/static/img/project.png)

---

# Overview

This project implements a monitoring system using an ESP32 microcontroller to measure and display temperature, humidity, and CO2 levels in real-time.

This is designed to be used to monitor the environment when growing plants

---

# Shopping List

Here is a list of parts used in this project

***Still a work in progress until I decide which CO2 sensor to use***

| NAME  | PRICE | COUNT | DESCRIPTION | LINK                                                                                                         |
|-------|-------|-------|-------------|--------------------------------------------------------------------------------------------------------------|
| ESP32 | $5    | 1     | Microcontroller      | https://www.amazon.com/ESP-WROOM-31-Development-Microcontroller-Integrated-Compatible/dp/B08D5ZD528 |
| DHT22 | $5    | 1     | Temp/Humidity Sensor | https://www.amazon.com/gp/product/B0795F19W6                                                        |
| ENS160 | $5   | 0/1   | CO2 Sensor  | https://www.amazon.com/HiLetgo-Dioxide-Temperature-Humidity-Monitoring/dp/B0CDWYFSGH                          |
| MQ135 | $5    | 0/1    | CO2 Sensor | https://www.amazon.com/Ximimark-Quality-Hazardous-Detection-Arduino/dp/B07L73VTTY                          |
| SGP30 | $5    | 0/1    | CO2 Sensor | https://www.makerfabs.com/sgp30-air-quality-sensor-breakout.html                          |

> *Lower prices can be found if you shop around*

---

# Setup

Once you have all the required parts in hand, you can start wiring


## Circuit Schematic

![ESP32 Plant Monitoring Circuit Schematic Image](/static/img/circuit-schematic.png)


| ESP32 Pin | Destination | Type    |
|-----------|-------------|---------|
| 27        | DHT22       | Digital |
| 35        | CO2 Sensor  | Analog  |

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

---

# 3D Print

![ESP32 Plant Monitoring Housing Image](/static/img/3d-housing.png)

The housing to hold the ESP32 and sensors can be 3D printed.

The 3D model files are located under the [3D-models](/3D-models) directory.

---

# TODO

* Decide on CO2 sensor
* Add wiring/setup instructions
* Add 3D models
* Replace images
* Improve code

---

# Links

* https://microcontrollerslab.com/esp32-dht11-dht22-web-server
* https://blog.asksensors.com/air-quality-sensor-mq135-cloud-mqtt

---

