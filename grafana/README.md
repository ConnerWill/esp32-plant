# ESP32 Plant Monitor Grafana Dashboard

## Docker

### Build and Run the Containers

Navigate to your project directory and run the following commands to build and start the containers:

```bash
docker-compose build
docker-compose up -d
```

### Configure Grafana

* Access Grafana: Open your web browser and go to http://localhost:3000.
* Add InfluxDB as a data source:
  * Go to Configuration > Data Sources > Add data source.
  * Select InfluxDB.
  * Configure the connection details:
    * URL: http://influxdb:8086
    * Database: esp32_data
    * User: admin
    * Password: admin
* Create a Dashboard:
  * Go to Create > Dashboard > Add new panel.
  * Select the InfluxDB data source.
  * Write a query to fetch the data, for example:

```sql
SELECT "temperature", "humidity", "co2" FROM "sensor_data" WHERE $timeFilter
```

Configure the visualization type (e.g., time series).

Now, you should have a complete setup with InfluxDB, Grafana, and a script that fetches data from your ESP32 and stores it in InfluxDB.

Grafana will then be able to graph the temperature, humidity, and CO2 levels over time.
