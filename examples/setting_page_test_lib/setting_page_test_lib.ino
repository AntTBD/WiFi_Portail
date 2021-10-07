// esp8266 V2.7.4
// NodeMCU 1.0
// upload speed : 921600
// VTables : Flash
// CPU Frequency : 160 MHz

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
#include <TBD_WiFi_Portail_ESP.h>
#include <TBD_WiFi_Portail_OTA.h>

// https://github.com/esprfid/esp-rfid


TBD_WiFi_Portail_SerialDebug debug(Serial, 115200, true); // TBD_WiFi_Portail_SerialDebug debug; => (port = Serial, baudRate=115200, wifiDiagnostic=false) par default
TBD_WiFi_Portail_FileSystem files(debug, false);
TBD_WiFi_Portail_FilesManager filesManager(debug, files);

TBD_WiFi_Portail_Wifi wifi(debug/*, filesManager*/);
#ifdef USE_MDNS
TBD_WiFi_Portail_MDNS mdns(debug, wifi);
#endif // USE_MDNS
#ifdef USE_FTP
TBD_WiFi_Portail_FTP ftp(debug, files);
#endif // USE_FTP
TBD_WiFi_Portail_WebServer webServer(debug, files, wifi, 80);
TBD_WiFi_Portail_WebSocket webSocket(debug, webServer/*, wifi*/, "/ws");
TBD_WiFi_Portail_WebEvents webEvents(debug, webServer, "/events", "/eventsAdmin");

#ifdef USE_NTP
TBD_WiFi_Portail_NTP ntp(debug, "pool.ntp.org");
#endif // USE_NTP
TBD_WiFi_Portail_ESP ESPInfos;
#ifdef USE_OTA
TBD_WiFi_Portail_OTA ota(debug, files);
#endif // USE_OTA

void handleStop(AsyncWebServerRequest *request) {
  request->send(200, F("text/plain"), F("Stop ok"));
}

void setup()
{
  pinMode(D4, OUTPUT);
  analogWrite(D4, 1020);//ON

//debug.setDebug(false);
  debug.begin();
  debug.printf(F("Free heap: %u\n"), ESP.getFreeHeap());

  files.begin();

//STA STA1("ssid","********");
//AP defaultAP("ESP_DEFAULT_AP_"+WiFi.macAddress(),"password");
  //wifi.wifiAll.allSTA.add(STA1);
  //wifi.wifiAll.setAP(defaultAP);

  filesManager.add(wifi);
#ifdef USE_FTP
  filesManager.add(ftp);
#endif // USE_FTP
#ifdef USE_OTA
  filesManager.add(ota);
#endif // USE_OTA
  filesManager.begin();

  wifi.begin();

#ifdef USE_OTA
  // init ota before mdns (because ota set a default hostname)
  ota.setArduinoOtaUseMDNS(true);//default=true
  ota.addWebEvents(webEvents);
#ifdef USE_MDNS
  ota.addMDNS(mdns);
#endif // USE_MDNS
  ota.begin();
#endif // USE_OTA

#ifdef USE_MDNS
  mdns.begin();
#endif // USE_MDNS

#ifdef USE_FTP
  ftp.begin();
#endif // USE_FTP

  webServer.addRoot("/stop", HTTP_GET, handleStop);
  webServer.addWebSocket(webSocket);
  webServer.addWebEvents(webEvents);
#ifdef USE_NTP
  webServer.addNTP(ntp);
#endif // USE_NTP
  webServer.addESPInfos(ESPInfos);
  webServer.begin();

  webSocket.addWebEvents(webEvents);
#ifdef USE_NTP
  webSocket.addNTP(ntp);
#endif // USE_NTP
  webSocket.addESPInfos(ESPInfos);
  webSocket.begin();

  webEvents.begin();

#ifdef USE_NTP
  //ntp.addWebSocket(webSocket);
  ntp.setTimeZone(1,0);
  ntp.begin();
  debug.println(ntp.getTimeDateString());
  debug.println(ntp.getUptimeString());
#endif // USE_NTP


#ifdef USE_NTP
  ESPInfos.addNTP(ntp);
#endif // USE_NTP
  ESPInfos.addFileSystem(files);


  
  debug.printf(F("Free heap: %u\n"), ESP.getFreeHeap());
  //debug.printf("%s\n", __FUNCTION__);
  debug.println(F("========================= END SETUP ========================="));
  
  analogWrite(D4, 1024);//OFF
}

void loop()
{
#ifdef USE_MDNS
  mdns.loop();
#endif // USE_MDNS

#ifdef USE_FTP
  ftp.loop();
#endif // USE_FTP

  webSocket.loop();

#ifdef USE_NTP
  ntp.testIfSyncEventTriggered();
  //ntp.loop();
#endif // USE_NTP

#ifdef USE_OTA
  ota.loop();
#endif // USE_OTA

}
