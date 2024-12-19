import os
import time
import requests
from influxdb import InfluxDBClient

# Get environment variables with default values if they do not exist
ESP32_IP = os.getenv("ESP32_IP", "http://192.168.1.100")  # Default to a placeholder IP
INFLUXDB_HOST = os.getenv("INFLUXDB_HOST", "influxdb")  # Default to the container name
INFLUXDB_PORT = int(os.getenv("INFLUXDB_PORT", 8086))  # Default to 8086
INFLUXDB_DB = os.getenv("INFLUXDB_DB", "esp32_data")  # Default database name
POLL_INTERVAL = int(os.getenv("POLL_INTERVAL", 2))  # Default polling interval to 2 seconds

# InfluxDB client
client = InfluxDBClient(host=INFLUXDB_HOST, port=INFLUXDB_PORT)
client.switch_database(INFLUXDB_DB)

def fetch_data():
    try:
        # Fetch data from the ESP32 endpoint
        response = requests.get(ESP32_IP)
        response.raise_for_status()
        data = response.json()

        # Create the data point for InfluxDB
        json_body = [
            {
                "measurement": "sensor_data",
                "fields": {
                    "temperature": data.get("temperature", 0),
                    "humidity": data.get("humidity", 0),
                    "co2": data.get("co2", 0)
                }
            }
        ]

        # Write data to InfluxDB
        client.write_points(json_body)
        print(f"Data written: {data}")
    except requests.exceptions.RequestException as e:
        print(f"Error fetching data: {e}")

def main():
    while True:
        fetch_data()
        time.sleep(POLL_INTERVAL)

if __name__ == "__main__":
    main()
