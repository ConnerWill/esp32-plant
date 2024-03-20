#include "config.h"

#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "DHTesp.h"

int analogPin = 35;
int dhtPin = 27;

DHTesp dht;

// const char* wifi_ssid = "YourNetworkName";
// const char* wifi_password =  "YourNetworkPass";

AsyncWebServer server(80);

int getCo2Measurement() {

  int adcVal = analogRead(analogPin);

  float voltage = adcVal * (3.3 / 4095.0);

  if (voltage == 0)
  {
    return -1;
  }
  else if (voltage < 0.4)
  {
    return -2;
  }
  else
  {
    float voltageDifference = voltage - 0.4;
    return (int) ((voltageDifference * 5000.0) / 1.6);
  }
}

void setup() {

  dht.setup(dhtPin);

  Serial.begin(115200);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());

  server.on("/co2", HTTP_GET, [](AsyncWebServerRequest * request) {

    int measurement = getCo2Measurement(); 

    String message;

    if(measurement == -1){message = "Sensor is not operating correctly";}
    else if(measurement == -2){message = "Sensor is pre-heating";}
    else {message = String(measurement) + " ppm";}

    request->send(200, "text/plain", message);

  });

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest * request) {

    float temperature = dht.getTemperature();

    request->send(200, "text/plain", String(temperature) + " ºC");
  });

  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest * request) {

    float humidity = dht.getHumidity(); 

    request->send(200, "text/plain", String(humidity) + " %");
  });

  server.begin();
}

void loop() {}
