//
// Created by antbd on 05/06/2021.
//

#ifndef TBD_WIFI_PORTAIL_MDNS_H
#define TBD_WIFI_PORTAIL_MDNS_H

#include "TBD_WiFi_Portail.h"
#ifdef USE_MDNS

#include <ESP8266mDNS.h>

// https://tttapa.github.io/ESP8266/Chap08%20-%20mDNS.html
// to replace IP address by a name

namespace WiFi_Portail_API {


    class MDNSManagerClass {
    public:
        MDNSManagerClass();

        ~MDNSManagerClass();

        void begin();

        void loop();

        String getMdnsName() const;

        void setMdnsName(const String &mdnsName);

        bool MDNSisRunning() const;

        String toString() const;

        bool isUpdatingMDNSInThisClass() const;

        void setUpdatingMDNSInThisClass(bool updateMDNSInThisClass);

    private:

        bool _updateMDNSInThisClass;


    };

    extern MDNSManagerClass MDNSManager;
}
#endif // USE_MDNS

#endif //TBD_WIFI_PORTAIL_MDNS_H
