//#include <TBD_WiFi_Portail.h>
#include <TBD_WiFi_Portail_SerialDebug.h>
#include <TBD_WiFi_Portail_FileSystem.h>
#include <TBD_WiFi_Portail_FilesManager.h>
#include <TBD_WiFi_Portail_Wifi.h>
#include <TBD_WiFi_Portail_MDNS.h>
#include <TBD_WiFi_Portail_FTP.h>
#include <TBD_WiFi_Portail_WebServer.h>
#include <TBD_WiFi_Portail_WebSocket.h>
#include <TBD_WiFi_Portail_WebEvents.h>
#include <TBD_WiFi_Portail_NTP.h>

// https://github.com/esprfid/esp-rfid


TBD_WiFi_Portail_SerialDebug debug(Serial, 115200, true); // TBD_WiFi_Portail_SerialDebug debug; => (port = Serial, baudRate=115200, wifiDiagnostic=false) par default
TBD_WiFi_Portail_FileSystem files(debug, false);
TBD_WiFi_Portail_FilesManager filesManager(debug, files);

//STA STA1("ssid","********");
//AP defaultAP("ESP_DEFAULT_AP_"+WiFi.macAddress(),"password");
TBD_WiFi_Portail_Wifi wifi(debug, filesManager);

TBD_WiFi_Portail_MDNS mdns(debug, wifi);
TBD_WiFi_Portail_FTP ftp(debug, files);
TBD_WiFi_Portail_WebServer webServer(debug, files, wifi, 80);
TBD_WiFi_Portail_WebSocket webSocket(debug, webServer, "/ws");
TBD_WiFi_Portail_WebEvents webEvents(debug, webServer, "/events", "/eventsAdmin");

TBD_WiFi_Portail_NTP ntp(debug, "pool.ntp.org");

void handleStop(AsyncWebServerRequest *request) {
  request->send(200, F("text/plain"), F("Stop ok"));
}

void setup()
{
  debug.begin();
  debug.printf(F("Free heap: %u\n"), ESP.getFreeHeap());

  files.begin();

  //wifi.wifiAll.allSTA.add(STA1);
  //wifi.wifiAll.setAP(defaultAP);

  filesManager.add(wifi);
  filesManager.add(ftp);
  filesManager.begin();

  wifi.begin();

  mdns.begin();

  ftp.begin();

  webServer.addRoot("/stop", HTTP_GET, handleStop);
  webServer.addWebSocket(webSocket);
  webServer.addWebEvents(webEvents);
  webServer.addNTP(ntp);
  webServer.begin();

  webSocket.addWebEvents(webEvents);
  webSocket.begin();

  webEvents.begin();

  ntp.addWebSocket(webSocket);
  ntp.begin();
  debug.println(ntp.getTimeDateString());
  debug.println(ntp.getUptimeString());


  
  debug.printf(F("Free heap: %u\n"), ESP.getFreeHeap());
  //debug.printf("%s\n", __FUNCTION__);
  debug.println(F("========================= END SETUP ========================="));
}

void loop()
{
  mdns.loop();

  ftp.loop();

  webSocket.loop();

  ntp.testIfSyncEventTriggered();
  //ntp.loop();
  
}
