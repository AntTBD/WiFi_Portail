//
// Created by antbd on 29/05/2021.
//

#ifndef TBD_WIFI_PORTAIL_H
#define TBD_WIFI_PORTAIL_H

#include <Arduino.h>

#ifndef ESP8266
#error This code is intended to run on the ESP8266 platform! Please check your Tools->Board setting.
#endif

#define WIFI_PORTAIL_VERSION "V0.0.5"

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

namespace WiFi_Portail_API {
    class SerialDebug;
    class FileSystem;
    class FilesManager;
    class Package;
    class AP;
    class STA;
    class WifiAll;
    class WifiManager;
    class MDNS;
    class FTP;
    class WebServer;
    class WebSocket;
    class WebEvents;
    class NTP;
    class ESPInfos;
    class OTA;
}
#include "TBD_WiFi_Portail_SerialDebug.h"
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
#include "TBD_WiFi_Portail_OTA.h"

namespace WiFi_Portail_API {
    class TBD_WiFi_Portail {
//    public:
//        TBD_WiFi_Portail() {
//            this->serial = new SerialDebug(&this);
//            this->fileSystem = new FileSystem(&this);
//            this->wifi = new Wifi(&this);
//            this->mdns = new MDNS(&this);
//            this->ftpServer = new FTP(&this);
//            this->webServer = new WebServer(&this);
//            this->webSocket = new WebSocket(&this);
//            this->webEvents = new WebEvents(&this);
//            this->ntp = new NTP(&this);
//            this->esp = new ESPInfo(&this);
//            this->ota = new OTA(&this);
//        }
//
//        ~TBD_WiFi_Portail() {
//            delete (this->serial);
//            delete (this->fileSystem);
//            delete (this->wifi);
//            delete (this->mdns);
//            delete (this->ftpServer);
//            delete (this->webServer);
//            delete (this->webSocket);
//            delete (this->webEvents);
//            delete (this->ntp);
//            delete (this->esp);
//            delete (this->ota);
//        }
//
//
//        /*using TBD_WiFi_Portail_SerialDebug::begin;
//        using TBD_WiFi_Portail_FileSystem::setup;
//        using TBD_WiFi_Portail_Wifi::setup;
//        using TBD_WiFi_Portail_MDNS::setup;
//        using TBD_WiFi_Portail_FTP::setup;
//        using TBD_WiFi_Portail_WebServer::setup;
//        using TBD_WiFi_Portail_WebSocket::setup;
//        using TBD_WiFi_Portail_WebEvents::setup;
//
//
//        using TBD_WiFi_Portail_MDNS::loop;
//        using TBD_WiFi_Portail_FTP::loop;
//    */
//
//        /*
//        bool loadAllInformationsFromJsonFiles();
//
//        bool setup_Wifi(); // Wifi
//
//        bool setup_MDNS(); // MDNS
//
//        bool setup_FTP(); // FTP
//
//        bool setup_WebServer(); // WebServer
//
//        bool setup_WebSocket(); // WebSocket
//        bool setup_WebEvents(); // WebEvents*/
//
//    public:
//        SerialDebug *serial;
//        FileSystem *fileSystem;
//        WifiManager *wifi;
//        MDNS *mdns;
//        FTP *ftpServer;
//        WebServer *webServer;
//        WebSocket *webSocket;
//        WebEvents *webEvents;
//        NTP *ntp;
//        ESPInfo *esp;
//        OTA *ota;
//
//    private:
    };
}
extern WiFi_Portail_API::TBD_WiFi_Portail WiFi_Portail;

#endif //TBD_WIFI_PORTAIL_H
