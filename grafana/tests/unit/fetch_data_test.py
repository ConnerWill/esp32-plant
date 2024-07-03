import pytest
from unittest.mock import patch, MagicMock
import json
from influxdb import InfluxDBClient
import requests
from main import fetch_and_store_data, esp32_ip, client

# Mock response data from ESP32
mock_response_data: dict[str, float | int] = {
    "temperature": 25.0,
    "humidity": 50.0,
    "co2": 800,
}


# Mock response object for requests.get()
class MockResponse:
    def __init__(self, json_data, status_code):
        self.json_data = json_data
        self.status_code = status_code

    def json(self):
        return self.json_data


@pytest.fixture
def mock_requests_get():
    with patch("requests.get") as mock_get:
        mock_get.return_value = MockResponse(mock_response_data, 200)
        yield mock_get


def test_fetch_and_store_data(mock_requests_get):
    # Mock client.write_points() to assert its call
    mock_write_points = MagicMock()
    client.write_points = mock_write_points

    # Call the function
    fetch_and_store_data()

    # Check if requests.get() was called with the expected URL
    mock_requests_get.assert_called_once_with(esp32_ip)

    # Check if client.write_points() was called with the expected JSON body
    expected_json_body = [
        {
            "measurement": "sensor_data",
            "tags": {"device": "esp32"},
            "fields": {
                "temperature": mock_response_data["temperature"],
                "humidity": mock_response_data["humidity"],
                "co2": mock_response_data["co2"],
            },
        }
    ]
    mock_write_points.assert_called_once_with(expected_json_body)


if __name__ == "__main__":
    pytest.main()
