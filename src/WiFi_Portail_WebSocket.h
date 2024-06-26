//
// Created by antbd on 05/06/2021.
//

#ifndef TBD_WIFI_PORTAIL_WEBSOCKET_H
#define TBD_WIFI_PORTAIL_WEBSOCKET_H

#include "WiFi_Portail.h"
#include "WiFi_Portail_WebServer.h"
#include "WiFi_Portail_WebEvents.h"

#include <ESPAsyncTCP.h>       // https://github.com/me-no-dev/ESPAsyncTCP
#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer

#include <vector>
#include <ArduinoJson.h>
namespace WiFi_Portail_API {

// empty function
    static void handleNULLWebSocket(AsyncWebSocketClient *client = nullptr, String value = "") {}

    typedef std::function<void(AsyncWebSocketClient* client,String value)>HandlerFunctionWebSocket; //unknow number of params

// class Path, Method and OnCommand
    class PathMethodOnRequestWebSocket {
    public:
        const char *commandNameIn;
        const char *commandNameOut;
        HandlerFunctionWebSocket onCommand;

        PathMethodOnRequestWebSocket() : commandNameIn(""), commandNameOut(""), onCommand(handleNULLWebSocket) {}

        PathMethodOnRequestWebSocket(const char *_commandNameIn, const char *_commandNameOut, HandlerFunctionWebSocket _onCommand) :
        commandNameIn(_commandNameIn), commandNameOut(_commandNameOut), onCommand(_onCommand) {}

        ~PathMethodOnRequestWebSocket() {}

        // surcharge operator
        void operator()(AsyncWebSocketClient *client = nullptr, String value = "") {
            return onCommand(client, value);
        } // can be called like : pathMethodOnRequestWebSocket(0, "")
        void operator()(String value = "") {
            return onCommand(nullptr, value);
        }                            // can be called like : pathMethodOnRequestWebSocket("")
    };

    class WebSocket {
    public:
        WebSocket(WebServer &webServer, const String &root = "/ws");

        ~WebSocket();

        void addWebEvents(WebEvents &webEvents);

        void addCommand(const char *commandNameIn, const char *commandNameOut, HandlerFunctionWebSocket onCommand);

        void begin();

        void loop();

        void handleReceivedMessage(const String &message, AsyncWebSocketClient *client);

        void
        onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);

        void close_WebSocket();

        /*void envoieAllInfosNewClient(AsyncWebSocketClient * client);
        void envoieHardwareInfosToClient(AsyncWebSocketClient * client);
        void envoieValuesToClient(AsyncWebSocketClient * client);*/


        void send(const JsonDocument &doc, AsyncWebSocketClient *client = nullptr);

        void send(const String &data, AsyncWebSocketClient *client = nullptr);

        void
        sendStringResultOf(const String &resultOfName, const String &result, AsyncWebSocketClient *client = nullptr);

        void sendJsonResultOf(const String &resultOfName, const DynamicJsonDocument &result, AsyncWebSocketClient *client = nullptr);

        void handleCommandScan(AsyncWebSocketClient *client = nullptr, String value = "");

        void handleCommandStatus(AsyncWebSocketClient *client = nullptr, String value = "");

        void handleCommandRealTime(AsyncWebSocketClient *client = nullptr, String value = "");
        void handleCommandSetNtp(AsyncWebSocketClient *client = nullptr, String value = "");

        void handleCommandNetwork(AsyncWebSocketClient *client = nullptr, String value = "");

        void handleCommandFiles(AsyncWebSocketClient *client = nullptr, String value = "");

        AsyncWebSocket *webSocket;

    private:
        WebServer *_webServer;
        WebEvents *_webEvents;

        String _root;
        std::vector <PathMethodOnRequestWebSocket> *_allCommands;
    };
}
#endif //TBD_WIFI_PORTAIL_WEBSOCKET_H
