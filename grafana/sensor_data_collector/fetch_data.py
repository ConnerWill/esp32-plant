import os
import time
from typing import Any, NoReturn
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
# TODO: I think the DB is created by Docker
# # Ensure the database exists; create it if not
# if {'name': INFLUXDB_DB} not in client.get_list_database():
#     client.create_database(INFLUXDB_DB)
client.switch_database(INFLUXDB_DB)


def safe_float(value: Any) -> float:
    """
    Attempt to convert a value to float.
    If conversion fails or the value is None, return 0.0.
    """
    try:
        return float(value)
    except (ValueError, TypeError):
        return 0.0


def fetch_data() -> None:
    """
    Fetch Data from ESP32 and write it to InfluxDB
    """
    try:
        # Fetch data from the ESP32 endpoint
        response = requests.get(url=ESP32_IP)
        response.raise_for_status()  # Will raise an exception for 4xx/5xx responses
        data: dict[str, Any] = response.json()

        # Log the fetched data for debugging
        logging.info(msg=f"Data fetched from {ESP32_IP}: {data}")

        # Get data
        # Convert the sensor values to float to ensure consistency.
        # If a sensor doesn't work or key is missing, default to 0.0.
        temperature: float = safe_float(value=data.get("temperature", 0.0))
        temperatureF: float = safe_float(value=data.get("temperatureF", 0.0))
        humidity: float = safe_float(value=data.get("humidity", 0.0))
        co2: float = safe_float(value=data.get("co2", 0.0))

        # Create the data point for InfluxDB
        json_body: list[dict[str, str | dict[str, float]]] = [
            {
                "measurement": "sensor_data",
                "fields": {
                    "temperature": temperature,
                    "temperatureF": temperatureF,
                    "humidity": humidity,
                    "co2": co2,
                },
            }
        ]

        # Write data to InfluxDB
        client.write_points(json_body)
        logging.info(msg=f"Data written to InfluxDB: {data}")

    except requests.exceptions.RequestException as e:
        # If an error occurs, log it but dont stop the script
        logging.error(msg=f"Error fetching data from {ESP32_IP}: {e}")
    except ValueError as ve:
        # Handle conversion errors if the data is not convertible to float
        logging.error(msg=f"Data conversion error: {ve}")


def main() -> NoReturn:
    """
    Main function to fetch data continuously.
    """
    try:
        while True:
            fetch_data()
            time.sleep(POLL_INTERVAL)
    except KeyboardInterrupt:
        logging.info(msg="Data collection stopped by user.")
        exit(code=0)


if __name__ == "__main__":
    main()
