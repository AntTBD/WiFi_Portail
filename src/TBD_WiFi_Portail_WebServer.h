
//
// Created by antbd on 05/06/2021.
//

#ifndef TBD_WIFI_PORTAIL_WEBSERVER_H
#define TBD_WIFI_PORTAIL_WEBSERVER_H

#include <Arduino.h>
#include "TBD_WiFi_Portail.h"
#include "Service.h"
#include "TBD_WiFi_Portail_SerialDebug.h"
#include "TBD_WiFi_Portail_FileSystem.h"
#include "TBD_WiFi_Portail_WebEvents.h"
#include "TBD_WiFi_Portail_WebSocket.h"
#include "TBD_WiFi_Portail_Wifi.h"
//#include "TBD_WiFi_Portail_NTP.h"
//#include "TBD_WiFi_Portail_ESP.h"

// to get secure server (not work realy) https://github.com/me-no-dev/ESPAsyncWebServer/issues/392 or https://gitlab.users.ch.eu.org:10443/smarthome/eaws-ssl-updater

//#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>       // https://github.com/me-no-dev/ESPAsyncTCP
#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer

#include <AFArray.h>
#include <ArduinoJson.h>

static String IpToString(IPAddress adress)
{
    return (String)adress[0] + "." + (String)adress[1] + "." + (String)adress[2] + "." + (String)adress[3];
}

// empty function
static void handleNULL(AsyncWebServerRequest *request) {}

// class Path, Method and OnRequest for serve.on(...)
class PathMethodOnRequest
{
public:
    const char *uri;
    WebRequestMethodComposite method;
    ArRequestHandlerFunction onRequest;

    PathMethodOnRequest() : uri(""), method(HTTP_ANY), onRequest(handleNULL) {}
    PathMethodOnRequest(const char *_uri, WebRequestMethodComposite _method, ArRequestHandlerFunction _onRequest) : uri(_uri), method(_method), onRequest(_onRequest) {}
    //PathMethodOnRequest(const char *_uri, WebRequestMethodComposite _method, void* _onRequest) : uri(_uri), method(_method), onRequest(_onRequest) {}
    ~PathMethodOnRequest() {}
};

class TBD_WiFi_Portail_WebServer
{
public:
    TBD_WiFi_Portail_WebServer(TBD_WiFi_Portail_SerialDebug &serialDebug, TBD_WiFi_Portail_FileSystem &fileSystem, TBD_WiFi_Portail_Wifi &wifi, int port);
    ~TBD_WiFi_Portail_WebServer();

    // add root
    void addRoot(const char *uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest);

    void addWebEvents(TBD_WiFi_Portail_WebEvents &webEvents);
    void addWebSocket(TBD_WiFi_Portail_WebSocket &webSocket);
    void addNTP(Service &ntp);
    void addESPInfos(Service &espInfos);

    void begin();

    void replyOK(AsyncWebServerRequest *request);
    void replyOKWithMsg(const String &msg, AsyncWebServerRequest *request);
    void replyNotFound(String msg, AsyncWebServerRequest *request);
    void replyBadRequest(const String &msg, AsyncWebServerRequest *request);
    void replyServerError(const String &msg, AsyncWebServerRequest *request);
    void handleWebRequests(AsyncWebServerRequest *request);
    void handleResetWifi(AsyncWebServerRequest *request);
    void handleLoginConsole(AsyncWebServerRequest *request);
    void handleScanWifi(AsyncWebServerRequest *request);
    void handleScanResult(int networksFound);
    void handleGetRealTime(AsyncWebServerRequest *request);
    void handleGetUptime(AsyncWebServerRequest *request);
    void handleGetESPInfos(AsyncWebServerRequest *request);
    void handleDebug(AsyncWebServerRequest *request);

    // ------------------------------------------ Init Serveur Web ---------------------------------------------
    AsyncWebServer *server;

    TBD_WiFi_Portail_Wifi *_wifi;

private:
    TBD_WiFi_Portail_SerialDebug *_serialDebug;
    TBD_WiFi_Portail_FileSystem *_fileSystem;
    TBD_WiFi_Portail_WebEvents *_webEvents;
    TBD_WiFi_Portail_WebSocket *_webSocket;
    Service *_ntp;
    Service *_espInfos;

    int _port;
    AFArray<PathMethodOnRequest> *_allRoot;

    const char *_loginConsole_username;
    const char *_loginConsole_password;
};

#endif //TBD_WIFI_PORTAIL_WEBSERVER_H
