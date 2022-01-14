//
// Created by antbd on 05/06/2021.
//

#ifndef TBD_WIFI_PORTAIL_MDNS_H
#define TBD_WIFI_PORTAIL_MDNS_H

#include <Arduino.h>
#include "TBD_WiFi_Portail.h"
#ifdef USE_MDNS
#include "TBD_WiFi_Portail_SerialDebug.h"
#include "TBD_WiFi_Portail_Wifi.h"

#define NO_GLOBAL_MDNS
#include <ESP8266mDNS.h>
using MDNSResponder = esp8266::MDNSImplementation::MDNSResponder;
extern MDNSResponder MDNSOfficial;

// https://tttapa.github.io/ESP8266/Chap08%20-%20mDNS.html
// to replace IP address by a name

namespace WiFi_Portail_API {


    class MDNS {
    public:
        MDNS(SerialDebug &serialDebug, WifiManager &_wifiManager);

        ~MDNS();

        void begin();

        void loop();

        String getMdnsName() const;

        void setMdnsName(const String &mdnsName);

        bool MDNSisRunning() const;

        String toString() const;

    private:
        SerialDebug *_serialDebug;
        WifiManager *_wifiManager;
        bool _updateMDNS;
    };
}
#endif // USE_MDNS

#endif //TBD_WIFI_PORTAIL_MDNS_H
