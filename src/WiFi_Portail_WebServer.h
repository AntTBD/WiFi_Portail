
//
// Created by antbd on 05/06/2021.
// https://gitlab.com/artonworkstm/asyncwebservertls
// https://gitlab.users.ch.eu.org/smarthome/eaws-ssl-updater/-/tree/master/
//

#ifndef TBD_WIFI_PORTAIL_WEBSERVER_H
#define TBD_WIFI_PORTAIL_WEBSERVER_H

#include "WiFi_Portail.h"
#include "WiFi_Portail_WebEvents.h"
#include "WiFi_Portail_WebSocket.h"

// to get secure server (not work realy) https://github.com/me-no-dev/ESPAsyncWebServer/issues/392 or https://gitlab.users.ch.eu.org:10443/smarthome/eaws-ssl-updater

#include <ESPAsyncTCP.h>       // https://github.com/me-no-dev/ESPAsyncTCP
#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer
#include <AsyncJson.h>

#include <vector>
#include <ArduinoJson.h>

#define CACHE_HEADER "max-age=86400"
#define CORS_HEADER "Access-Control-Allow-Origin"

#define CONTENT_TYPE_JSON "application/json"
#define CONTENT_TYPE_PLAIN "text/plain"
#define CONTENT_TYPE_HTML "text/html"

namespace WiFi_Portail_API {

// empty function
    static void handleNULL(AsyncWebServerRequest *request) {}

// class Path, Method and OnRequest for serve.on(...)
    class PathMethodOnRequest {
    public:
        const char *uri;
        WebRequestMethodComposite method;
        ArRequestHandlerFunction onRequest;

        PathMethodOnRequest() : uri(""), method(HTTP_ANY), onRequest(handleNULL) {}

        PathMethodOnRequest(const char *_uri, WebRequestMethodComposite _method, ArRequestHandlerFunction _onRequest)
                : uri(_uri), method(_method), onRequest(_onRequest) {}

        //PathMethodOnRequest(const char *_uri, WebRequestMethodComposite _method, void* _onRequest) : uri(_uri), method(_method), onRequest(_onRequest) {}
        ~PathMethodOnRequest() {}
    };

    class WebServer {
    public:
        WebServer(int port);

        ~WebServer();

        // add root
        void addRoot(const char *uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest);

        void addServerStatic(const char *uri, const char *path, const char *defaultFile, ArRequestFilterFunction filter = 0) const;
        void addServerStatic(const char *uri, const char *path, const char *defaultFile, const char *username, const char *password, ArRequestFilterFunction filter = 0) const;

        void addWebEvents(WebEvents &webEvents);

        void addWebSocket(WebSocket &webSocket);

        void begin();

        void replyOK(AsyncWebServerRequest *request);

        void replyOKWithMsg(const String &msg, AsyncWebServerRequest *request);

        void replyOKWithJson(const String &resultOfName, AsyncWebServerRequest *request, int res, JsonVariant json);

        void replyNotFound(String msg, AsyncWebServerRequest *request);

        void replyBadRequest(const String &msg, AsyncWebServerRequest *request);

        void replyServerError(const String &msg, AsyncWebServerRequest *request);

        void handleWebRequests(AsyncWebServerRequest *request);

        void handleResetWifi(AsyncWebServerRequest *request);

        void handleLoginConsole(AsyncWebServerRequest *request);

        void handleScanWifi(AsyncWebServerRequest *request);

        DynamicJsonDocument handleScanResult(int networksFound);

        void handleGetRealTime(AsyncWebServerRequest *request);

        void handleGetUptime(AsyncWebServerRequest *request);

        void handleGetESPInfos(AsyncWebServerRequest *request);

        void handleDebug(AsyncWebServerRequest *request);

        // ------------------------------------------ Init Serveur Web ---------------------------------------------
        AsyncWebServer *server;

    private:
        WebEvents *_webEvents;
        WebSocket *_webSocket;

        int _port;
        std::vector <PathMethodOnRequest> *_allRoots;

        const char *_loginConsole_username;
        const char *_loginConsole_password;

#ifdef USE_SECURE_SERVER
        static int sslFileRequest(void *arg, const char *filename, uint8_t **buf);
#endif

    };
}
#endif //TBD_WIFI_PORTAIL_WEBSERVER_H
