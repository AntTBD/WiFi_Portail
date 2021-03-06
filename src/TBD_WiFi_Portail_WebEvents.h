//
// Created by antbd on 05/06/2021.
//

#ifndef TBD_WIFI_PORTAIL_WEBEVENTS_H
#define TBD_WIFI_PORTAIL_WEBEVENTS_H

#include <Arduino.h>
#include "TBD_WiFi_Portail.h"
#include "TBD_WiFi_Portail_SerialDebug.h"
#include "TBD_WiFi_Portail_WebServer.h"

#include <ESPAsyncTCP.h>       // https://github.com/me-no-dev/ESPAsyncTCP
#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer

class sEventType
{
public:
    const char *error;
    const char *warn;
    const char *success;
    const char *info;
    const char *cancel;
    const char *debug;

    sEventType() : error("ERROR"), warn("WARN"), success("SUCCESS"), info("INFO"), cancel("CANCEL"), debug("DEBUG") {}
    ~sEventType() {}
};

class TBD_WiFi_Portail_WebEvents
{
public:
    TBD_WiFi_Portail_WebEvents(TBD_WiFi_Portail_SerialDebug &serialDebug, TBD_WiFi_Portail_WebServer &webServer, const String &eventsRoot = "/events", const String &eventsAdminRoot = "/eventsAdmin");
    ~TBD_WiFi_Portail_WebEvents();

    void setEventsAdmin_UserPass(const String &user = "userAdmin", const String &password = "passAdmin");

    void begin();
    void debug_to_WSEvents(const String &message, const String &type = "debug", int niveau = 1);
    void envoieInfosBroadcast();
    void envoiePositionsBroadcast();
    void envoieAllInfosBroadcast();

    AsyncEventSource *events;
    AsyncEventSource *eventsAdmin;

    sEventType *eventType;

private:
    TBD_WiFi_Portail_SerialDebug *_serialDebug;
    TBD_WiFi_Portail_WebServer *_webServer;

    String _eventsRoot;
    String _eventsAdminRoot;

    String _adminUser;
    String _adminPass;

    //unsigned long previousMillisSendInfos = 0; // will store last time send informations was updated
    //const long intervalSendInfos = 1000;        // interval at which to send informations when turn motor (milliseconds)
};

#endif //TBD_WIFI_PORTAIL_WEBEVENTS_H
