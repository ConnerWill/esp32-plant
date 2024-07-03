#!/usr/bin/env python

import os
import requests
import json
from influxdb import InfluxDBClient
import time
import logging

# Configure logging
logging.basicConfig(level=logging.INFO)

# Sleep time (seconds)
sleep_wait: float = 60.0

# ESP32 IP address
esp32_ip: str = os.getenv("ESP32_IP", "http://esp32-plant")

# InfluxDB configuration
influxdb_host: str = os.getenv("INFLUXDB_HOST", "influxdb")
influxdb_port: int = int(os.getenv("INFLUXDB_PORT", 8086))
influxdb_user: str = os.getenv("INFLUXDB_USER", "admin")
influxdb_password: str = os.getenv("INFLUXDB_PASSWORD", "admin")
influxdb_dbname: str = os.getenv("INFLUXDB_DBNAME", "esp32_data")

# Initialize InfluxDB client
client: InfluxDBClient = InfluxDBClient(
    host=influxdb_host,
    port=influxdb_port,
    username=influxdb_user,
    password=influxdb_password,
    database=influxdb_dbname,
)


def fetch_and_store_data():
    try:
        response: requests.Response = requests.get(url=esp32_ip)
        response.raise_for_status()  # Raise an HTTPError for bad responses

        data: dict = response.json()

        # Create a JSON body for InfluxDB
        json_body: list = [
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
        client.write_points(points=json_body)
        logging.info(f"Data written to InfluxDB: {json_body}")

    except requests.exceptions.HTTPError as e:
        logging.error(msg=f"HTTP error occurred: {e}")

    except requests.exceptions.RequestException as e:
        logging.error(msg=f"Request error occurred: {e}")

    except json.JSONDecodeError as e:
        logging.error(msg=f"Error decoding JSON response: {e}")

    except Exception as e:
        logging.error(msg=f"Unexpected error occurred: {e}")


def main():
    while True:
        fetch_and_store_data()
        time.sleep(seconds=sleep_wait)


if __name__ == "__main__":
    main()
