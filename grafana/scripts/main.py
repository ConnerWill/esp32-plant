#!/usr/bin/env python

import os
import typing
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
    """
    Fetches sensor data from ESP32 device and stores it in InfluxDB.

    This function performs the following steps:
    1. Sends a GET request to ESP32 device to fetch sensor data.
    2. Parses the JSON response and extracts temperature, humidity, and CO2 levels.
    3. Constructs a JSON body formatted for InfluxDB.
    4. Writes the data points to InfluxDB using InfluxDBClient.
    5. Logs successful data write or errors encountered during the process.
    """
    try:
        response: requests.Response = requests.get(url=esp32_ip)
        response.raise_for_status()  # Raise an HTTPError for bad responses

        data: dict[str, float | int] = response.json()

        # Create a JSON body for InfluxDB
        json_body: list[dict[str, str | int | float]] = [
            {
                "measurement": "sensor_data",
                "tags": {"device": "esp32"},
                "fields": {
                    "temperature": data.get("temperature", 0),
                    "humidity": data.get("humidity", 0),
                    "co2": data.get("co2", 0),
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
    """
    Main function that runs continuously to fetch and store sensor data.

    This function executes fetch_and_store_data() in a loop with a sleep period.
    Adjusts sleep time based on sleep_wait variable.
    """
    while True:
        fetch_and_store_data()
        time.sleep(seconds=sleep_wait)


if __name__ == "__main__":
    main()
