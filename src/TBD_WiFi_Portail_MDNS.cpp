//
// Created by antbd on 05/06/2021.
//

#include "TBD_WiFi_Portail_MDNS.h"

#ifdef USE_MDNS

TBD_WiFi_Portail_MDNS::TBD_WiFi_Portail_MDNS(TBD_WiFi_Portail_SerialDebug &serialDebug, TBD_WiFi_Portail_Wifi &allWifi) : _serialDebug(&serialDebug),
                                                                                                                          _allWifi(&allWifi)
{
    this->_updateMDNS = true;
}

TBD_WiFi_Portail_MDNS::~TBD_WiFi_Portail_MDNS()
{
    delete this->_serialDebug;
    delete this->_allWifi;
}

// --------------------------- Start the mDNS responder ---------------------------------
void TBD_WiFi_Portail_MDNS::begin()
{
    this->_serialDebug->println(F("===== Setup MDNS ====="));
    if (this->MDNSisRunning()) //if is already started, MDNS.update() could be implemented so we don't have to update again
    {
        this->_updateMDNS = false;
    }
    // first, check if MDNS is already started (by Arduino OTA for example via ArduinoOTA.begin(true))
    if (!this->MDNSisRunning() && !MDNS.begin(this->_allWifi->wifiAll->getMdnsName().c_str())) // start the multicast domain name server
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
        if (this->MDNSisRunning())
        {
            this->setMdnsName(this->_allWifi->wifiAll->getMdnsName());
        }
        this->_serialDebug->println(F("MDNS responder started"));
        MDNS.addService("http", "tcp", 80); // add http service
        this->_serialDebug->println(F("Port HTTP open (80)"));

        MDNS.addService("ws", "tcp", 81); // add websocket service
        this->_serialDebug->println(F("Port WebSocket open (81)"));
    }
    this->_serialDebug->println(this->toString());
    this->_serialDebug->println(F("======================"));
}

// -------------------------------- loop MDNS --------------------------------------
void TBD_WiFi_Portail_MDNS::loop()
{
    if (this->_updateMDNS)
    {
        // Allow MDNS processing
        MDNS.update();
    }
}

String TBD_WiFi_Portail_MDNS::getMdnsName() const
{
    return this->_allWifi->wifiAll->getMdnsName();
}

void TBD_WiFi_Portail_MDNS::setMdnsName(const String &mdnsName)
{
    MDNS.setHostname(mdnsName.c_str());
    this->_allWifi->wifiAll->setMdnsName(mdnsName.c_str());
}

bool TBD_WiFi_Portail_MDNS::MDNSisRunning() const
{
    return MDNS.isRunning();
}

String TBD_WiFi_Portail_MDNS::toString() const
{

    return (String)F("======== MDNS =======") + F("\n") +
           F("MDNS running : ") + (this->MDNSisRunning() ? F("true") : F("false")) + F("\n") +
           F("Update MDNS in this class : ") + (this->_updateMDNS ? F("true") : F("false")) + F("\n") +
           F("Connect to http://") + this->getMdnsName() /*.c_str()*/ + F(".local or http://") + WiFi.localIP().toString() + F("\n") +
           F("=====================");
}

#endif // USE_MDNS