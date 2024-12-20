#include "web_server.h"
#include "config.h"
#include "sensors.h"

AsyncWebServer server(WEBSERVER_PORT);

void setupWebServer() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String measurements = getAllMeasurements();
    request->send(200, "application/json", measurements);
  });
  server.begin();
}
