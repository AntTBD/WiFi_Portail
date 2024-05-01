// esp8266 V2.7.4
// NodeMCU 1.0
// upload speed : 921600
// VTables : Flash
// CPU Frequency : 160 MHz


#include <WiFi_Portail.h>

// https://github.com/esprfid/esp-rfid
using namespace WiFi_Portail_API;


void handleStop(AsyncWebServerRequest *request) {
  request->send(200, F("text/plain"), F("Stop ok"));
}

void setup() {
  WiFi_Portail.webServer->addRoot("/stop", HTTP_GET, handleStop);
  WiFi_Portail.begin();
}

void loop() {
  WiFi_Portail.loop();
}