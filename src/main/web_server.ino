#include "web_server.h"
#include "config.h"
#include "sensors.h"

AsyncWebServer server(WEBSERVER_PORT);

void setupWebServer() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", getAllMeasurements());
  });
  server.begin();
}
