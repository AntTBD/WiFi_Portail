//
// Created by antbd on 05/06/2021.
//

#include "TBD_WiFi_Portail_MDNS.h"

#ifdef USE_MDNS
MDNSResponder MDNSOfficial;
namespace WiFi_Portail_API {

    MDNS::MDNS(SerialDebug &serialDebug, WifiManager &_wifiManager) : _serialDebug(&serialDebug), _wifiManager(&_wifiManager) {
        this->_updateMDNS = true;
    }

    MDNS::~MDNS() {
        delete this->_serialDebug;
        delete this->_wifiManager;
    }

// --------------------------- Start the mDNS responder ---------------------------------
    void MDNS::begin() {
        this->_serialDebug->println(F("===== Setup MDNS ====="));
        if (this->MDNSisRunning()) //if is already started, MDNS.update() could be implemented so we don't have to update again
        {
            this->_updateMDNS = false;
        }
        // first, check if MDNS is already started (by Arduino OTA for example via ArduinoOTA.begin(true))
        if (!this->MDNSisRunning() && !MDNSOfficial.begin(
                this->_wifiManager->wifiAll->getMdnsName().c_str())) // start the multicast domain name server
        {
            this->_serialDebug->println(this->toString());
            this->_serialDebug->println(F("!!! MDNS.begin failed, REBOOT manualy please !!!\n..."));
            while (this->MDNSisRunning()==false && MDNSOfficial.begin(this->_wifiManager->wifiAll->getMdnsName().c_str())==false) {
                delay(1000);
                this->_serialDebug->print(F("."));
            }
        }

        if (this->MDNSisRunning()) {
            this->setMdnsName(this->_wifiManager->wifiAll->getMdnsName());
        }
        this->_serialDebug->println(F("MDNS responder started"));
        MDNSOfficial.addService("http", "tcp", 80); // add http service
        this->_serialDebug->println(F("Port HTTP open (80)"));

        MDNSOfficial.addService("ws", "tcp", 81); // add websocket service
        this->_serialDebug->println(F("Port WebSocket open (81)"));

        this->_serialDebug->println(this->toString());
        this->_serialDebug->println(F("======================"));
    }

// -------------------------------- loop MDNS --------------------------------------
    void MDNS::loop() {
        if (this->_updateMDNS) {
            // Allow MDNS processing
            MDNSOfficial.update();
        }
    }

    String MDNS::getMdnsName() const {
        return this->_wifiManager->wifiAll->getMdnsName();
    }

    void MDNS::setMdnsName(const String &mdnsName) {
        MDNSOfficial.setHostname(mdnsName.c_str());
        this->_wifiManager->wifiAll->setMdnsName(mdnsName.c_str());
    }

    bool MDNS::MDNSisRunning() const {
        return MDNSOfficial.isRunning();
    }

    String MDNS::toString() const {

        return (String) F("======== MDNS =======") + F("\n") +
               F("MDNS running : ") + (this->MDNSisRunning() ? F("true") : F("false")) + F("\n") +
               F("Update MDNS in this class : ") + (this->_updateMDNS ? F("true") : F("false")) + F("\n") +
               F("Connect to http://") + this->getMdnsName() /*.c_str()*/ + F(".local or http://") + WiFi.localIP().toString() + F("\n") +
               F("=====================");
    }
}
#endif // USE_MDNS