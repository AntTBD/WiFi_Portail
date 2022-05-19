// esp8266 V2.7.4
// NodeMCU 1.0
// upload speed : 921600
// VTables : Flash
// CPU Frequency : 160 MHz


#include <TBD_WiFi_Portail.h>

// https://github.com/esprfid/esp-rfid
using namespace WiFi_Portail_API;


WebServer webServer(80);
WebSocket webSocket(webServer, "/ws");
WebEvents webEvents(webServer, "/events", "/eventsAdmin");

void handleStop(AsyncWebServerRequest *request) {
  request->send(200, F("text/plain"), F("Stop ok"));
}

void setup() {
    pinMode(D4, OUTPUT);
    analogWrite(D4, 1020);//ON

    //SerialDebug.setDebug(false);
    SerialDebug.begin(Serial, 115200, true);
    SerialDebug.printf(F("Free heap: %u\n"), ESP.getFreeHeap());

    FileSystem.begin(false);

    Utils.beginWatchdog();

    //STA STA1("ssid","********");
    //AP defaultAP("ESP_DEFAULT_AP_"+WiFi.macAddress(),"password");
    //WifiManager.wifiAll.allSTA.add(STA1);
    //WifiManager.wifiAll.setAP(defaultAP);

    FilesManager.begin();

    WifiManager.begin();

#ifdef USE_OTA
    // init ota before mdns (because ota set a default hostname)
    OTAManager.setArduinoOtaUseMDNS(true);//default=true
    OTAManager.addWebEvents(webEvents);// add web events to have ability to informe client during ota update
    OTAManager.begin("esp8266-ikea");
#endif // USE_OTA

#ifdef USE_MDNS
    MDNSManager.begin();
#endif // USE_MDNS

#ifdef USE_FTP
    // user and pass set from files
    //FTPManager.setUser("esp8266_FTP");
    //FTPManager.setPassword("pass");
    FTPManager.begin();
#endif // USE_FTP

    webServer.addRoot("/stop", HTTP_GET, handleStop);
    webServer.addWebSocket(webSocket);
    webServer.addWebEvents(webEvents);
    webServer.begin();

    webSocket.addWebEvents(webEvents);
    webSocket.begin();

    webEvents.begin();

#ifdef USE_NTP
    //NTPManager.addWebSocket(webSocket);
    NTPManager.setTimeZone(1, 0);
    NTPManager.begin("pool.ntp.org");
    SerialDebug.println(NTPManager.getTimeDateString());
    SerialDebug.println(NTPManager.getUptimeString());
#endif // USE_NTP

    SerialDebug.printf(F("Free heap: %u\n"), ESP.getFreeHeap());
    //SerialDebug.printf("%s\n", __FUNCTION__);
    SerialDebug.println(F("========================= END SETUP ========================="));

    analogWrite(D4, 1024);//OFF
}

void loop() {
    Utils.loopWatchdog();

#ifdef USE_MDNS
    MDNSManager.loop();
#endif // USE_MDNS

#ifdef USE_FTP
    FTPManager.loop();
#endif // USE_FTP

    webSocket.loop();

#ifdef USE_NTP
    NTPManager.testIfSyncEventTriggered();
    //NTPManager.loop();
#endif // USE_NTP

#ifdef USE_OTA
    OTAManager.loop();
#endif // USE_OTA

}
