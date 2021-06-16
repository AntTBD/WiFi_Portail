//
// Created by antbd on 29/05/2021.
//

#ifndef TBD_WIFI_PORTAIL_H
#define TBD_WIFI_PORTAIL_H

#include <Arduino.h>

#ifndef ESP8266
#error This code is intended to run on the ESP8266 platform! Please check your Tools->Board setting.
#endif

//#define DEBUG_ESP_WIFI 1
//#define DEBUG_ESP_PORT Serial

//void debug_to_WSEvents(String message, String type = "debug", int niveau = 1);


static void restartESP(){
    ESP.restart();
}


//#include <AFArray.h>

//#include "log.h"
//#include "TBD_WiFi_Portail_SerialDebug.h"
//#include "TBD_WiFi_Portail_FileSystem.h"

//#include "project.h"
//#include "FileSystem.h"
/*#include "configFiles.h"

#include "WifiAll.h"
#include "wifi.h"
#include "mdns.h"

#include "FTP.h"

#include "webserver.h"
#include "websocket.h"
#include "webevents.h"*/

class TBD_WiFi_Portail_SerialDebug;
class TBD_WiFi_Portail_FileSystem;
class TBD_WiFi_Portail_FilesManager;
class TBD_WiFi_Portail_Package;
//class AP;
//class STA;
class WifiAll;
class TBD_WiFi_Portail_Wifi;
class TBD_WiFi_Portail_MDNS;
class TBD_WiFi_Portail_FTP;
class TBD_WiFi_Portail_WebServer;
class TBD_WiFi_Portail_WebSocket;
class TBD_WiFi_Portail_WebEvents;
class TBD_WiFi_Portail_NTP;
class TBD_WiFi_Portail_ESP;


class TBD_WiFi_Portail/* :
    public TBD_WiFi_Portail_SerialDebug,
    public TBD_WiFi_Portail_FileSystem,
    public TBD_WiFi_Portail_Wifi,
    public TBD_WiFi_Portail_MDNS,
    public TBD_WiFi_Portail_FTP,
    public TBD_WiFi_Portail_WebServer,
    public TBD_WiFi_Portail_WebSocket,
    public TBD_WiFi_Portail_WebEvents*/ {
public:
    //TBD_WiFi_Portail() {};
    /*TBD_WiFi_Portail(TBD_WiFi_Portail_SerialDebug& _portail_SerialDebug, TBD_WiFi_Portail_FileSystem& _portail_FileSystem) :
        portail_SerialDebug(&_portail_SerialDebug),
        portail_FileSystem(&_portail_FileSystem)
        {};

    ~TBD_WiFi_Portail() {};*/

    /*using TBD_WiFi_Portail_SerialDebug::begin;
    using TBD_WiFi_Portail_FileSystem::setup;
    using TBD_WiFi_Portail_Wifi::setup;
    using TBD_WiFi_Portail_MDNS::setup;
    using TBD_WiFi_Portail_FTP::setup;
    using TBD_WiFi_Portail_WebServer::setup;
    using TBD_WiFi_Portail_WebSocket::setup;
    using TBD_WiFi_Portail_WebEvents::setup;


    using TBD_WiFi_Portail_MDNS::loop;
    using TBD_WiFi_Portail_FTP::loop;
*/



    /*
    bool loadAllInformationsFromJsonFiles();

    bool setup_Wifi(); // Wifi

    bool setup_MDNS(); // MDNS

    bool setup_FTP(); // FTP

    bool setup_WebServer(); // WebServer

    bool setup_WebSocket(); // WebSocket
    bool setup_WebEvents(); // WebEvents*/



protected:

    /*TBD_WiFi_Portail_SerialDebug* portail_SerialDebug;
    TBD_WiFi_Portail_FileSystem* portail_FileSystem;
    TBD_WiFi_Portail_Wifi;
    TBD_WiFi_Portail_MDNS;
    TBD_WiFi_Portail_FTP;
    TBD_WiFi_Portail_WebServer;
    TBD_WiFi_Portail_WebSocket;
    TBD_WiFi_Portail_WebEvents;*/

private:

};

//extern TBD_WiFi_Portail TBD_WiFi_Portail;

#endif //TBD_WIFI_PORTAIL_H
