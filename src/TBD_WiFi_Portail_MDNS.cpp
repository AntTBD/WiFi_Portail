//
// Created by antbd on 05/06/2021.
//

#include "TBD_WiFi_Portail_MDNS.h"

#ifdef USE_MDNS
namespace WiFi_Portail_API {

    MDNSManagerClass MDNSManager;

    MDNSManagerClass::MDNSManagerClass() {
        this->_updateMDNSInThisClass = true;
    }

    MDNSManagerClass::~MDNSManagerClass() {
    }

// --------------------------- Start the mDNS responder ---------------------------------
    void MDNSManagerClass::begin() {
        SerialDebug_println(F("===== Setup MDNS ====="));
        if (this->MDNSisRunning()) //if is already started, MDNS.update() could be implemented so we don't have to update again
        {
            this->_updateMDNSInThisClass = false;
        }
        // first, check if MDNS is already started (by Arduino OTA for example via ArduinoOTA.begin(true))
        if (!this->MDNSisRunning() && !MDNS.begin(
                WifiManager.wifiAll->getMdnsName().c_str())) // start the multicast domain name server
        {
            SerialDebug_println(this->toString());
            SerialDebug_println(F("!!! MDNS.begin failed, REBOOT manualy please !!!\n..."));
            while (this->MDNSisRunning()==false || MDNS.begin(WifiManager.wifiAll->getMdnsName().c_str())==false) {
                delay(1000);
                SerialDebug_print(F("."));
            }
        }

        if (this->MDNSisRunning()) {
            this->setMdnsName(WifiManager.wifiAll->getMdnsName());
        }
        SerialDebug_println(F("MDNS responder started"));
        MDNS.addService("http", "tcp", 80); // add http service
        SerialDebug_println(F("Port HTTP open (80)"));

        MDNS.addService("ws", "tcp", 81); // add websocket service
        SerialDebug_println(F("Port WebSocket open (81)"));

        SerialDebug_println(this->toString());
        SerialDebug_println(F("======================"));
    }

// -------------------------------- loop MDNS --------------------------------------
    void MDNSManagerClass::loop() {
        if (this->_updateMDNSInThisClass) {
            // Allow MDNS processing
            MDNS.update();
        }
    }

    String MDNSManagerClass::getMdnsName() const {
        return WifiManager.wifiAll->getMdnsName();
    }

    void MDNSManagerClass::setMdnsName(const String &mdnsName) {
        MDNS.setHostname(mdnsName.c_str());
        WifiManager.wifiAll->setMdnsName(mdnsName.c_str());
    }

    bool MDNSManagerClass::MDNSisRunning() const {
        return MDNS.isRunning();
    }

    String MDNSManagerClass::toString() const {
        String str;
        str += F("======== MDNS =======");
        str += F("\n");
        str += F("MDNS is running: ");
        str += this->MDNSisRunning() ? F("true") : F("false");
        str += F("\n");
        str += F("Update MDNS in this class: ");
        str += this->_updateMDNSInThisClass ? F("true") : F("false");
        str += F("\n");
        str += F("Connect to http://");
        str += this->getMdnsName(); /*.c_str()*/
        str += F(".local or http://");
        str += WiFi.localIP().toString();
        str += F("\n");
        str += F("=====================");

        return str;
    }

    bool MDNSManagerClass::isUpdatingMDNSInThisClass() const {
        return this->_updateMDNSInThisClass;
    }

    void MDNSManagerClass::setUpdatingMDNSInThisClass(bool updateMDNSInThisClass) {
        this->_updateMDNSInThisClass = updateMDNSInThisClass;
    }
}
#endif // USE_MDNS