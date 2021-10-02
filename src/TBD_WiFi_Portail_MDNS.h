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

#include <ESP8266mDNS.h>
// https://tttapa.github.io/ESP8266/Chap08%20-%20mDNS.html
// to replace IP address by a name

class TBD_WiFi_Portail_MDNS
{
public:
    TBD_WiFi_Portail_MDNS(TBD_WiFi_Portail_SerialDebug &serialDebug, TBD_WiFi_Portail_Wifi &allWifi);
    ~TBD_WiFi_Portail_MDNS();

    void begin();
    void loop();

    String getMdnsName() const;
    void setMdnsName(const String &mdnsName);
    bool MDNSisRunning() const;
    String toString() const;

private:
    TBD_WiFi_Portail_SerialDebug *_serialDebug;
    TBD_WiFi_Portail_Wifi *_allWifi;
    bool _updateMDNS;
};

#endif // USE_MDNS

#endif //TBD_WIFI_PORTAIL_MDNS_H
