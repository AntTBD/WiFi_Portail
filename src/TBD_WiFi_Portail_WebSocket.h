//
// Created by antbd on 05/06/2021.
//

#ifndef TBD_WIFI_PORTAIL_WEBSOCKET_H
#define TBD_WIFI_PORTAIL_WEBSOCKET_H

#include <Arduino.h>
#include "TBD_WiFi_Portail.h"
#include "Service.h"
#include "TBD_WiFi_Portail_SerialDebug.h"
#include "TBD_WiFi_Portail_WebServer.h"
#include "TBD_WiFi_Portail_WebEvents.h"
//#include "TBD_WiFi_Portail_Wifi.h"
//#include "TBD_WiFi_Portail_NTP.h"
//#include "TBD_WiFi_Portail_ESP.h"

#include <ESPAsyncTCP.h>       // https://github.com/me-no-dev/ESPAsyncTCP
#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer

#include <list>
#include <ArduinoJson.h>
#include <AFArray.h>

// empty function
static void handleNULLWebSocket(int numClient = -1, String value = "") {}

typedef std::function<void(int numClient, String value)> HandlerFunctionWebSocket; //unknow number of params

// class Path, Method and OnCommand
class PathMethodOnRequestWebSocket
{
public:
    const char *commandNameIn;
    const char *commandNameOut;
    HandlerFunctionWebSocket onCommand;

    PathMethodOnRequestWebSocket() : commandNameIn(""), commandNameOut(""), onCommand(handleNULLWebSocket) {}
    PathMethodOnRequestWebSocket(const char *_commandNameIn, const char *_commandNameOut, HandlerFunctionWebSocket _onCommand) : commandNameIn(_commandNameIn), commandNameOut(_commandNameOut), onCommand(_onCommand) {}
    ~PathMethodOnRequestWebSocket() {}

    // surcharge operator
    void operator()(int numClient = -1, String value = "") { return onCommand(numClient, value); } // can be called like : pathMethodOnRequestWebSocket(0, "")
    void operator()(String value = "") { return onCommand(-1, value); }                            // can be called like : pathMethodOnRequestWebSocket("")
};

class TBD_WiFi_Portail_WebSocket
{
public:
    TBD_WiFi_Portail_WebSocket(TBD_WiFi_Portail_SerialDebug &serialDebug, TBD_WiFi_Portail_WebServer &webServer /*, TBD_WiFi_Portail_Wifi& wifi*/, const String &root = "/ws");
    ~TBD_WiFi_Portail_WebSocket();

    void addWebEvents(TBD_WiFi_Portail_WebEvents &webEvents);
    void addNTP(Service &ntp);
    void addESPInfos(Service &espInfos);

    void addCommand(const char *commandNameIn, const char *commandNameOut, HandlerFunctionWebSocket onCommand);

    void begin();
    void loop();

    void handleReceivedMessage(const String &message, uint32_t numClient);
    void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);

    void close_WebSocket();
    void sendJsonByWebsocket(const JsonDocument &doc);
    void sendJsonByWebsocket2(DynamicJsonDocument doc);
    //void sendDataWs(AsyncWebSocketClient * client);
    void ws_send_to_client(uint32_t client_id, String message);
    void ws_send_to_all_client(String message);
    void ws_send_to_all_client(AsyncWebSocketMessageBuffer *buffer);
    void envoieAllInfosNewClient(uint32_t client_id);
    void envoieHardwareInfosToClient(uint32_t client_id);
    void envoieValuesToClient(uint32_t client_id);

    void sendStringResultOf(const String &resultOf, const String &result);
    void sendJsonResultOf(const String &resultOf, const DynamicJsonDocument &result);

    void handleCommandScan(int numClient = -1, String value = "");
    void handleCommandStatus(int numClient = -1, String value = "");
    void handleCommandRealTime(int numClient = -1, String value = "");
    void handleCommandNetwork(int numClient = -1, String value = "");

    AsyncWebSocket *webSocket;

private:
    TBD_WiFi_Portail_SerialDebug *_serialDebug;
    TBD_WiFi_Portail_WebServer *_webServer;
    TBD_WiFi_Portail_WebEvents *_webEvents;
    //TBD_WiFi_Portail_Wifi* _wifi;
    Service *_ntp;
    Service *_espInfos;

    String _root;
    AFArray<PathMethodOnRequestWebSocket> *_allCommand;
    std::list<PathMethodOnRequestWebSocket> test;
};

#endif //TBD_WIFI_PORTAIL_WEBSOCKET_H
