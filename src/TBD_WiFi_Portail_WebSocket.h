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

#include <ESPAsyncTCP.h>       // https://github.com/me-no-dev/ESPAsyncTCP
#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer

#include <vector>
#include <ArduinoJson.h>

// empty function
static void handleNULLWebSocket(AsyncWebSocketClient * client = nullptr, String value = "") {}

typedef std::function<void(AsyncWebSocketClient * client, String value)> HandlerFunctionWebSocket; //unknow number of params

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
    void operator()(AsyncWebSocketClient * client = nullptr, String value = "") { return onCommand(client, value); } // can be called like : pathMethodOnRequestWebSocket(0, "")
    void operator()(String value = "") { return onCommand(nullptr, value); }                            // can be called like : pathMethodOnRequestWebSocket("")
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

    void handleReceivedMessage(const String &message, AsyncWebSocketClient * client);
    void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);

    void close_WebSocket();
    /*void envoieAllInfosNewClient(AsyncWebSocketClient * client);
    void envoieHardwareInfosToClient(AsyncWebSocketClient * client);
    void envoieValuesToClient(AsyncWebSocketClient * client);*/


    void send(const JsonDocument &doc, AsyncWebSocketClient * client = nullptr);
    void send(const String &data, AsyncWebSocketClient * client = nullptr);
    void sendStringResultOf(const String &resultOfName, const String &result, AsyncWebSocketClient * client = nullptr);
    void sendJsonResultOf(const String &resultOfName, const DynamicJsonDocument &result, AsyncWebSocketClient * client = nullptr);

    void handleCommandScan(AsyncWebSocketClient * client = nullptr, String value = "");
    void handleCommandStatus(AsyncWebSocketClient * client = nullptr, String value = "");
    void handleCommandRealTime(AsyncWebSocketClient * client = nullptr, String value = "");
    void handleCommandNetwork(AsyncWebSocketClient * client = nullptr, String value = "");
    void handleCommandFiles(AsyncWebSocketClient * client = nullptr, String value = "");

    AsyncWebSocket *webSocket;

private:
    TBD_WiFi_Portail_SerialDebug *_serialDebug;
    TBD_WiFi_Portail_WebServer *_webServer;
    TBD_WiFi_Portail_WebEvents *_webEvents;
    //TBD_WiFi_Portail_Wifi* _wifi;
    Service *_ntp;
    Service *_espInfos;

    String _root;
    std::vector<PathMethodOnRequestWebSocket> *_allCommands;
};

#endif //TBD_WIFI_PORTAIL_WEBSOCKET_H
