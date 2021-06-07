//
// Created by antbd on 05/06/2021.
//

#ifndef TBD_WIFI_PORTAIL_WEBSOCKET_H
#define TBD_WIFI_PORTAIL_WEBSOCKET_H

#include <Arduino.h>
#include "TBD_WiFi_Portail.h"
#include "TBD_WiFi_Portail_SerialDebug.h"
#include "TBD_WiFi_Portail_WebServer.h"
#include "TBD_WiFi_Portail_WebEvents.h"

#include <ESPAsyncTCP.h>       // https://github.com/me-no-dev/ESPAsyncTCP
#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer

#include <ArduinoJson.h>


class TBD_WiFi_Portail_WebSocket {
public:
    TBD_WiFi_Portail_WebSocket(TBD_WiFi_Portail_SerialDebug& serialDebug, TBD_WiFi_Portail_WebServer& webServer, String root = "/ws");
    ~TBD_WiFi_Portail_WebSocket();

    void addWebEvents(TBD_WiFi_Portail_WebEvents& webEvents);

    void begin();
    void loop();

    void handleReceivedMessage(String message, uint32_t numClient);
    void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len);

    void close_WebSocket();
    void sendJsonByWebsocket(DynamicJsonDocument *doc);
    void sendDataWs(AsyncWebSocketClient * client);
    void ws_send_to_client(uint32_t client_id, const char* message);
    void ws_send_to_all_client(char* message);
    void ws_send_to_all_client(AsyncWebSocketMessageBuffer * buffer);
    void envoieAllInfosNewClient(uint32_t client_id);
    void envoieHardwareInfosToClient(uint32_t client_id);
    void envoieValuesToClient(uint32_t client_id);


    AsyncWebSocket* webSocket;
private:
    TBD_WiFi_Portail_SerialDebug* _serialDebug;
    TBD_WiFi_Portail_WebServer* _webServer;
    TBD_WiFi_Portail_WebEvents* _webEvents;

    String _root;

};


#endif //TBD_WIFI_PORTAIL_WEBSOCKET_H
