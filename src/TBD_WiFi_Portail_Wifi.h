//
// Created by antbd on 04/06/2021.
//

#ifndef TBD_WIFI_PORTAIL_WIFI_H
#define TBD_WIFI_PORTAIL_WIFI_H

#include <Arduino.h>
#include "TBD_WiFi_Portail.h"
#include "TBD_WiFi_Portail_SerialDebug.h"
//#include "TBD_WiFi_Portail_FilesManager.h"
#include "WifiAll.h"

#include <ESP8266WiFi.h> //ESP8266 Core WiFi Library (you most likely already have this in your sketch)
#include <ESP8266WiFiMulti.h>

#include <ArduinoJson.h>
namespace WiFi_Portail_API {

    class WifiManager {
    public:
        WifiManager(SerialDebug &serialDebug /*, FilesManager& _filesManager*/, WiFiMode_t startMode = WIFI_STA,
             bool resetWifi = false);

        ~WifiManager();

        void begin();

        void onWifiConnect(const WiFiEventStationModeConnected &event);

        void onWifiDisconnect(const WiFiEventStationModeDisconnected &event);

        void onWifiGotIP(const WiFiEventStationModeGotIP &event);

        bool startAP(AP *ap);

        void fallbacktoAPMode();

        bool connectSTA(std::vector <STA> *allSTA, String hostname, uint32_t connectionTimeoutMUltiWifi);

        void infosWifi();

        String wifiStatusToString() const;

        DynamicJsonDocument getNetworkInfosJson();

        String getNetworkInfosString();

        void disableWifi();

        void enableWifi();

        void resetWifi();

        // ---------- Structure all infos -----------
        WifiAll *wifiAll; // max 10 STA

    private:
        SerialDebug *_serialDebug;
        //FilesManager* _filesManager;

        // ------------------- wifi config -----------------
        ESP8266WiFiMulti _wifiMulti;
        uint32_t _connectTimeoutMs; // WiFi connect timeout per AP. Increase when connecting takes longer.  = WIFI_CONNECT_TIMEOUT_MS 5000
        bool _wifiFlag = false;
        bool configMode = false;

        bool _inAPMode = false;
        bool _isWifiConnected = false;

        // ------------- event listeners ------------
        WiFiEventHandler _wifiDisconnectHandler, _wifiConnectHandler, _wifiOnStationModeGotIPHandler;
    };
}
#endif //TBD_WIFI_PORTAIL_WIFI_H
