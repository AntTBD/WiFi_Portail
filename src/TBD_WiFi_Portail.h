//
// Created by antbd on 29/05/2021.
//

#ifndef TBD_WIFI_PORTAIL_H
#define TBD_WIFI_PORTAIL_H

#include <Arduino.h>


#ifndef ESP8266
#error This code is intended to run on the ESP8266 platform! Please check your Tools->Board setting.
#endif

#define WIFI_PORTAIL_VERSION "V0.0.6"

#define DEBUG_ON_SERIAL
#define USE_NTP
#define USE_FTP // lag some times
#define USE_MDNS
#define USE_OTA

#ifdef DEBUG_ON_SERIAL
#define SerialDebug_print(...) SerialDebug.print(__VA_ARGS__)
#define SerialDebug_println(...) SerialDebug.println(__VA_ARGS__)
#define SerialDebug_printf(...) SerialDebug.printf(__VA_ARGS__)
#else
#define SerialDebug_print(...)
    #define SerialDebug_println(...)
    #define SerialDebug_printf(...)
#endif

namespace WiFi_Portail_API {
    class WiFi_Portail;
    class UtilsClass;
    class Package;
    class AP;
    class STA;
    class WifiAll;
    class MDNSManagerClass;
    class FTPManagerClass;
    class WebServer;
    class WebSocket;
    class WebEvents;
    class Service;
    class NTPManagerClass;
    class ESPInfosClass;
    class OTAManagerClass;
}

#include "Utils.h"
#include "Services/Service.h"

// Static instance
#include "TBD_WiFi_Portail_SerialDebug.h"
#include "TBD_WiFi_Portail_FileSystem.h"
#include "TBD_WiFi_Portail_FilesManager.h"
#include "TBD_WiFi_Portail_Wifi.h"



#include "TBD_WiFi_Portail_WebServer.h"
#include "TBD_WiFi_Portail_WebSocket.h"
#include "TBD_WiFi_Portail_WebEvents.h"

// singletons
#include "TBD_WiFi_Portail_Package.h"
#include "Services/TBD_WiFi_Portail_ESP.h"
#ifdef USE_MDNS
#include "TBD_WiFi_Portail_MDNS.h"
#endif // USE_MDNS
#ifdef USE_FTP
#include "TBD_WiFi_Portail_FTP.h"
#endif // USE_FTP
#ifdef USE_NTP
#include "Services/TBD_WiFi_Portail_NTP.h"
#endif // USE_NTP
#ifdef USE_OTA
#include "TBD_WiFi_Portail_OTA.h"
#endif // USE_OTA


namespace WiFi_Portail_API {
    class WiFi_Portail {

    };
}

#endif //TBD_WIFI_PORTAIL_H
