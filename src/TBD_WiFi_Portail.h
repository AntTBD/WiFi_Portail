//
// Created by antbd on 29/05/2021.
//

#ifndef TBD_WIFI_PORTAIL_H
#define TBD_WIFI_PORTAIL_H

#include <Arduino.h>

#ifndef ESP8266
#error This code is intended to run on the ESP8266 platform! Please check your Tools->Board setting.
#endif

#define WIFI_PORTAIL_VERSION "V0.0.3"

//#define DEBUG_ESP_WIFI 1
//#define DEBUG_ESP_PORT Serial

#define USE_NTP
#define USE_FTP
#define USE_MDNS
#define USE_OTA

static void restartESP()
{
    ESP.restart();
}

class TBD_WiFi_Portail_SerialDebug;
class TBD_WiFi_Portail_FileSystem;
class TBD_WiFi_Portail_FilesManager;
class TBD_WiFi_Portail_Package;
class AP;
class STA;
class WifiAll;
class TBD_WiFi_Portail_Wifi;
class TBD_WiFi_Portail_MDNS;
class TBD_WiFi_Portail_FTP;
class TBD_WiFi_Portail_WebServer;
class TBD_WiFi_Portail_WebSocket;
class TBD_WiFi_Portail_WebEvents;
class TBD_WiFi_Portail_NTP;
class TBD_WiFi_Portail_ESP;
class TBD_WiFi_Portail_OTA;

/* #include "TBD_WiFi_Portail_SerialDebug.h"
#include "TBD_WiFi_Portail_FileSystem.h"
#include "TBD_WiFi_Portail_FilesManager.h"
#include "TBD_WiFi_Portail_Package.h"
#include "WifiAll.h"
#include "TBD_WiFi_Portail_Wifi.h"
#include "TBD_WiFi_Portail_MDNS.h"
#include "TBD_WiFi_Portail_FTP.h"
#include "TBD_WiFi_Portail_WebServer.h"
#include "TBD_WiFi_Portail_WebSocket.h"
#include "TBD_WiFi_Portail_WebEvents.h"
#include "TBD_WiFi_Portail_NTP.h"
#include "TBD_WiFi_Portail_ESP.h"
#include "TBD_WiFi_Portail_OTA.h" */

class TBD_WiFi_Portail
{
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
