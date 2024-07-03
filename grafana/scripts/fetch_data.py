#!/usr/bin/env python

import os
import requests
import json
from influxdb import InfluxDBClient
import time

# ESP32 IP address
esp32_ip = os.getenv("ESP32_IP", "http://<ESP32_IP>")

# InfluxDB configuration
influxdb_host: str = os.getenv("INFLUXDB_HOST", "influxdb")
influxdb_port: int = int(os.getenv("INFLUXDB_PORT", 8086))
influxdb_user: str = os.getenv("INFLUXDB_USER", "admin")
influxdb_password: str = os.getenv("INFLUXDB_PASSWORD", "admin")
influxdb_dbname: str = os.getenv("INFLUXDB_DBNAME", "esp32_data")

# Initialize InfluxDB client
client = InfluxDBClient(
    influxdb_host, influxdb_port, influxdb_user, influxdb_password, influxdb_dbname
)


def fetch_and_store_data():
    try:
        response = requests.get(esp32_ip)
        data = response.json()

        # Create a JSON body for InfluxDB
        json_body = [
            {
                "measurement": "sensor_data",
                "tags": {"device": "esp32"},
                "fields": {
                    "temperature": data["temperature"],
                    "humidity": data["humidity"],
                    "co2": data["co2"],
                },
            }
        ]

        # Write data to InfluxDB
        client.write_points(json_body)
        print(f"Data written to InfluxDB: {json_body}")
    except Exception as e:
        print(f"Error: {e}")


if __name__ == "__main__":
    while True:
        fetch_and_store_data()
        time.sleep(60)  # Wait for 1 minute before fetching data again
