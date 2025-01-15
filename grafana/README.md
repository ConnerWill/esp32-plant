# ESP32 Sensor Data Collector and Grafana Dashboard

This project collects sensor data (temperature, humidity, CO2) from an ESP32 device,
stores the data in InfluxDB, and visualizes it using Grafana.

The entire setup runs in Docker containers, including a Python script that
fetches sensor data, InfluxDB for storing the data, and Grafana for visualization.

## Project Structure

```console
.
├── docker-compose.yml    # Docker Compose file to bring up all services
├── README.md             # Documentation
└── sensor_data_collector # Directory for Python script and requirements
    ├── Dockerfile        # Dockerfile to build the Python script container
    ├── fetch_data.py     # Python script to fetch data from ESP32 and store in InfluxDB
    └── requirements.txt  # Python dependencies
```

## Prerequisites

- Docker and Docker Compose installed on your system.
- ESP32 device providing data via HTTP.

## Setup

### Step 1: Build and Start Containers

Run the following command to start the containers:

```bash
docker-compose up --build
```

This will:

- Build the Python script container (python-script).
- Start the InfluxDB container to store the sensor data.
- Start the Grafana container for data visualization.

### Step 2: Configure Environment Variables

You can configure the following environment variables, which will be used by the Python script:

- `ESP32_IP`: IP address of the ESP32 device *(defaults to [`http://192.168.0.30`](http://192.168.0.30))*.
- `INFLUXDB_HOST`: Hostname of the InfluxDB instance *(defaults to `influxdb`)*.
- `INFLUXDB_PORT`: Port for InfluxDB *(defaults to `8086`)*.
- `INFLUXDB_DB`: Name of the database to store data *(defaults to `esp32_data`)*.
- `POLL_INTERVAL`: Interval in seconds for polling the ESP32 device *(defaults to `2`)*.

These environment variables can be set directly in your system
or in the Docker container using .env files or
Docker's environment variable configuration.

```bash
ESP32_IP=http://192.168.0.30
INFLUXDB_HOST=influxdb
INFLUXDB_DB=esp32_data
POLL_INTERVAL=2
```

### Step 3: Grafana Configuration

Once the containers are up and running, you can access Grafana at `http://localhost:3000`:

- Username: `admin`
- Password: `admin`

In Grafana, you'll need to add InfluxDB as a data source:

1. Go to Configuration > Data Sources.
2. Choose InfluxDB.
3. Set the URL to `http://influxdb:8086` *(or the relevant InfluxDB service URL)*.
4. Set the database name to esp32_data *(or the value you configured in the `INFLUXDB_DB` variable)*.
5. Save and test the connection.

### Step 4: Create Dashboards in Grafana

Once InfluxDB is configured in Grafana,
you can create dashboards to visualize the sensor data.
You'll be able to plot the temperature, humidity, and CO2 values over time.

## Docker Compose Configuration

The docker-compose.yml file defines three services:

- `influxdb`: The InfluxDB instance where sensor data is stored.
- `grafana`: The Grafana instance to visualize the data.
- `python-script`: The Python script that fetches data from the ESP32 and writes it to InfluxDB.

```yaml
version: '3.8'

services:
  influxdb:
    image: influxdb:1.8
    container_name: influxdb
    ports:
      - "8086:8086"
    volumes:
      - influxdb-data:/var/lib/influxdb
    environment:
      INFLUXDB_DB: esp32_data
      INFLUXDB_ADMIN_USER: admin
      INFLUXDB_ADMIN_PASSWORD: admin

  grafana:
    image: grafana/grafana:latest
    container_name: grafana
    ports:
      - "3000:3000"
    environment:
      - GF_SECURITY_ADMIN_USER=admin
      - GF_SECURITY_ADMIN_PASSWORD=admin
    depends_on:
      - influxdb
    volumes:
      - grafana-data:/var/lib/grafana

  python-script:
    build:
      context: ./sensor_data_collector
    container_name: sensor_data_collector
    depends_on:
      - influxdb

volumes:
  influxdb-data:
  grafana-data:
```

## Python Script

The Python script *(`fetch_data.py`)* fetches data from the ESP32 device
and writes it to InfluxDB.

It uses the requests library to pull data from the ESP32's IP address, and influxdb to store the data.

### Python Dependencies

The required Python dependencies are listed in requirements.txt:

```python
requests>=2.25.0
influxdb>=5.3.0
```

The `fetch_data.py` script handles the following:

- Fetching sensor data from the ESP32 using the HTTP GET request.
- Writing the fetched data (temperature, humidity, CO2) to InfluxDB.
- Configuring default values for environment variables if they are not provided.

### Dockerfile for Python Script

The Dockerfile builds the Python script container,
ensuring the required dependencies are installed.

```bash
FROM python:3.9-slim

WORKDIR /app

COPY ./sensor_data_collector/requirements.txt /app/requirements.txt
RUN pip install --no-cache-dir -r requirements.txt

COPY ./sensor_data_collector /app

CMD ["python", "fetch_data.py"]
```

## Conclusion

This project provides an easy way to collect, store, and visualize sensor data
from an ESP32 device using InfluxDB and Grafana in Docker containers.
The Python script handles the data collection and storage, and Grafana provides an intuitive dashboard for data visualization.
