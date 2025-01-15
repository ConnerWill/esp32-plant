import os
import time
from typing import NoReturn
import requests
import logging
from influxdb import InfluxDBClient

# Set up logging
logging.basicConfig(level=logging.INFO)

# Get environment variables with default values if they do not exist
ESP32_IP: str = os.getenv(
    "ESP32_IP", "http://192.168.0.30"
)  # Default to a placeholder IP
INFLUXDB_HOST: str = os.getenv(
    "INFLUXDB_HOST", "influxdb"
)  # Default to the container name
INFLUXDB_PORT: int = int(os.getenv("INFLUXDB_PORT", 8086))  # Default to 8086
INFLUXDB_DB: str = os.getenv("INFLUXDB_DB", "esp32_data")  # Default database name
POLL_INTERVAL: int = int(
    os.getenv("POLL_INTERVAL", 2)
)  # Default polling interval to 2 seconds

# InfluxDB client setup
client = InfluxDBClient(host=INFLUXDB_HOST, port=INFLUXDB_PORT)
client.switch_database(INFLUXDB_DB)


def fetch_data() -> None:
    """
    Fetch Data from ESP32 and write it to InfluxDB
    """
    try:
        # Fetch data from the ESP32 endpoint
        response = requests.get(url=ESP32_IP)
        response.raise_for_status()  # Will raise an exception for 4xx/5xx responses
        data = response.json()

        # Log the fetched data for debugging
        logging.info(msg=f"Data fetched from {ESP32_IP}: {data}")

        # Create the data point for InfluxDB
        json_body = [
            {
                "measurement": "sensor_data",
                "fields": {
                    "temperature_c": data.get("temperature_c", 0),
                    "temperature_f": data.get("temperature_f", 0),
                    "humidity": data.get("humidity", 0),
                    "co2": data.get("co2_level", 0),
                },
            }
        ]

        # Write data to InfluxDB
        client.write_points(json_body)
        logging.info(msg=f"Data written to InfluxDB: {data}")

    except requests.exceptions.RequestException as e:
        # If an error occurs, log it but dont stop the script
        logging.error(msg=f"Error fetching data from {ESP32_IP}: {e}")


def main() -> NoReturn:
    """
    Main function to fetch data continuously.
    """
    while True:
        fetch_data()
        time.sleep(POLL_INTERVAL)


if __name__ == "__main__":
    main()
