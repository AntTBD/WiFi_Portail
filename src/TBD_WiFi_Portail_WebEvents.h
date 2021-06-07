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
    String error;
    String warn;
    String success;
    String info;
    String cancel;
    String debug;

    sEventType() : error(F("ERROR")), warn(F("WARN")), success(F("SUCCESS")), info(F("INFO")), cancel(F("CANCEL")), debug(F("DEBUG")) {}
    ~sEventType() {}
};



class TBD_WiFi_Portail_WebEvents {
public:
    TBD_WiFi_Portail_WebEvents(TBD_WiFi_Portail_SerialDebug& serialDebug, TBD_WiFi_Portail_WebServer& webServer, String eventsRoot = "/events", String eventsAdminRoot = "/eventsAdmin");
    ~TBD_WiFi_Portail_WebEvents();

    void setEventsAdmin_UserPass(String user = "userAdmin", String password = "passAdmin");

    void begin();
    void debug_to_WSEvents(String message, String type = "debug", int niveau = 1);
    void envoieInfosBroadcast();
    void envoiePositionsBroadcast();
    void envoieAllInfosBroadcast();


    AsyncEventSource* events;
    AsyncEventSource* eventsAdmin;

    sEventType* eventType;

private:
    TBD_WiFi_Portail_SerialDebug* _serialDebug;
    TBD_WiFi_Portail_WebServer* _webServer;

    String _eventsRoot;
    String _eventsAdminRoot;

    String _adminUser;
    String _adminPass;

    //unsigned long previousMillisSendInfos = 0; // will store last time send informations was updated
    //const long intervalSendInfos = 1000;        // interval at which to send informations when turn motor (milliseconds)
};


#endif //TBD_WIFI_PORTAIL_WEBEVENTS_H
