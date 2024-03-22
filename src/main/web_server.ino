#include "web_server.h"
#include "config.h"
#include "sensors.h"

AsyncWebServer server(WEBSERVER_PORT);

void setupWebServer() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String jsonResponse = getAllMeasurements(); // Ensure getAllMeasurements() returns a JSON string
    request->send(200, "application/json", jsonResponse);
  });
  server.begin();
}
