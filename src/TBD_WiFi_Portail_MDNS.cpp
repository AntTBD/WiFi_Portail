//
// Created by antbd on 05/06/2021.
//

#include "TBD_WiFi_Portail_MDNS.h"

TBD_WiFi_Portail_MDNS::TBD_WiFi_Portail_MDNS(TBD_WiFi_Portail_SerialDebug& serialDebug, TBD_WiFi_Portail_Wifi& allWifi): _serialDebug(&serialDebug), _allWifi(&allWifi){}
TBD_WiFi_Portail_MDNS::~TBD_WiFi_Portail_MDNS() {}

// --------------------------- Start the mDNS responder ---------------------------------
void TBD_WiFi_Portail_MDNS::begin() {
    this->_serialDebug->println(F("===== Setup MDNS ====="));
    if (!MDNS.begin(this->_allWifi->wifiAll.getMdnsName().c_str())) // start the multicast domain name server
    {
        this->_serialDebug->println(F("MDNS.begin failed, REBOOT manualy please !"));
        while (1)
        {
            delay(1000);
            this->_serialDebug->print(F("."));
        }
    }
    else
    {
        this->_serialDebug->println(F("MDNS responder started"));
        MDNS.addService("http", "tcp", 80); // add http service
        this->_serialDebug->println(F("Port HTTP open (80)"));

        MDNS.addService("ws", "tcp", 81); // add websocket service
        this->_serialDebug->println(F("Port WebSocket open (81)"));
    }
    this->_serialDebug->print(F("Connect to http://"));
    this->_serialDebug->print(this->_allWifi->wifiAll.getMdnsName().c_str());
    this->_serialDebug->print(F(".local or http://"));
    this->_serialDebug->println(WiFi.localIP().toString());
    this->_serialDebug->println(F("======================"));
}

// -------------------------------- loop MDNS --------------------------------------
void TBD_WiFi_Portail_MDNS::loop() {
    // Allow MDNS processing
    MDNS.update();
}