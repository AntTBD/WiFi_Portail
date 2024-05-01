//
// Created by antbd on 05/06/2021.
//

#include "WiFi_Portail_WebServer.h"

namespace WiFi_Portail_API {
    WebServer::WebServer(int port) :
            _port(port) {
        this->server = new AsyncWebServer(this->_port);
        this->_loginConsole_username = "admin";
        this->_loginConsole_password = "admin";

        this->_webEvents = nullptr;
        this->_webSocket = nullptr;
        this->_allRoots = new std::vector<PathMethodOnRequest>();
    }

    WebServer::~WebServer() {
        delete this->_webEvents;
        delete this->_webSocket;

        delete this->server;

        this->_allRoots->clear();
        delete this->_allRoots;
    }

    void WebServer::addRoot(const char *uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest) {

        // define root
        PathMethodOnRequest pathMethodOnRequest(uri, method, onRequest);
        this->_allRoots->push_back(pathMethodOnRequest);
        this->server->on(pathMethodOnRequest.uri, pathMethodOnRequest.method, pathMethodOnRequest.onRequest);
    }

    void WebServer::addServerStatic(const char *uri, const char *path, const char *defaultFile, ArRequestFilterFunction filter) const {
        this->addServerStatic(uri, path, defaultFile, "", "", filter);
    }

    void WebServer::addServerStatic(const char *uri, const char *path, const char *defaultFile, const char *username, const char *password, ArRequestFilterFunction filter) const {
        AsyncStaticWebHandler* handler = &this->server->serveStatic(uri, *FileSystem.fileSystem, path);
        handler->setDefaultFile(defaultFile);
        if(filter != 0) handler->setFilter(filter);
        if(username != "" && password != "") handler->setAuthentication(username, password);
    }

    void WebServer::addWebEvents(WebEvents &webEvents) {
        this->_webEvents = &webEvents;
    }

    void WebServer::addWebSocket(WebSocket &webSocket) {
        this->_webSocket = &webSocket;
    }

#ifdef USE_SECURE_SERVER
    int WebServer::sslFileRequest(void * arg, const char *filename, uint8_t **buf) {
        SerialDebug_printf(F("Reading file: %s\n"), filename);
        if(FileSystem.fileSystem->begin()) {
            SerialDebug_println(F("SPIFFS has started"));
        }
        File file = FileSystem.fileSystem->open(filename, "r");
        if(file){
            SerialDebug_printf(F("File found: %s\n"), filename);
            size_t size = file.size();
            uint8_t * nbuf = (uint8_t*)malloc(size);
            if(nbuf){
                size = file.read(nbuf, size);
                file.close();
                *buf = nbuf;
                return size;
            }
            file.close();
        } else {
            SerialDebug_println(F("File not found!"));
            *buf = 0;
            return 0;
        }
    }
#endif

    void WebServer::begin() {
        SerialDebug_println(F("== Setup Web Server =="));

        // define roots
        this->addRoot(
                "/resetWifi",
                HTTP_GET,
                //std::bind(&WebServer::handleResetWifi, this,std::placeholders::_1)
                [&](AsyncWebServerRequest *request) { this->handleResetWifi(request); });
        this->addRoot(
                "/scan",
                HTTP_GET,
                //std::bind(&WebServer::handleScanWifi, this,std::placeholders::_1)
                [&](AsyncWebServerRequest *request) { this->handleScanWifi(request); });
        this->addRoot(
                "/login_console",
                HTTP_POST,
                //std::bind(&WebServer::handleLoginConsole, this,std::placeholders::_1)
                [&](AsyncWebServerRequest *request) { this->handleLoginConsole(request); });
        this->addRoot(
                "/getRealTime",
                HTTP_GET,
                //std::bind(&WebServer::handleGetRealTime, this,std::placeholders::_1)
                [&](AsyncWebServerRequest *request) { this->handleGetRealTime(request); });
        this->addRoot(
                "/getUptime",
                HTTP_GET,
                //std::bind(&WebServer::handleGetUptime, this,std::placeholders::_1)
                [&](AsyncWebServerRequest *request) { this->handleGetUptime(request); });
        this->addRoot(
                "/getESPInfos",
                HTTP_GET,
                //std::bind(&WebServer::handleGetESPInfos, this,std::placeholders::_1)
                [&](AsyncWebServerRequest *request) { this->handleGetESPInfos(request); });
        this->addRoot(
                "/debug",
                HTTP_GET,
                //std::bind(&WebServer::handleDebug, this,std::placeholders::_1)
                [&](AsyncWebServerRequest *request) { this->handleDebug(request); }
                );

        //Serve different site files in AP or STA mode
        this->addServerStatic("/", "/www/", "index.html", ON_STA_FILTER);
        this->addServerStatic("/", "/ap/", "index.html", ON_AP_FILTER);
        this->addServerStatic("/ws/", "/ws/", "ws.html"/*, "_user", "pass"*/);
        this->addServerStatic("/admin/", "/admin/", "index.html"/*, "_user", "pass"*/);

        SerialDebug_print(F("root: (nbr="));
        SerialDebug_print(this->_allRoots->size() + 3);
        SerialDebug_println(F(")"));

        // add preconfigurated root with function
        for (PathMethodOnRequest &pathMethodOnRequestTemp : *this->_allRoots) {
            SerialDebug_print(F(" "));
            SerialDebug_println(pathMethodOnRequestTemp.uri);
        }

        SerialDebug_println(F(" /     (in STA Mode)"));
        SerialDebug_println(F(" /     (in AP Mode)"));
        SerialDebug_println(F(" /ws/"));

        //server->addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);                 //only when requested from AP

        this->server->onNotFound(
                //std::bind(&WebServer::handleWebRequests, this,std::placeholders::_1)
                [&](AsyncWebServerRequest *request) {
                    this->handleWebRequests(request);
                }); // if someone requests any other file or page, go to function 'handleWebRequests'
        SerialDebug_println(F("onNotFound : handleWebRequests"));

        DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
        SerialDebug_println(F("default header : Access-Control-Allow-Origin : *"));

#ifdef USE_SECURE_SERVER

        this->server->onSslFileRequest(this->sslFileRequest, NULL);
        SerialDebug_println(F("Certs validation"));

        this->server->beginSecure("/Cert.pem", "/Key.pem", NULL);
        SerialDebug_println(F(" -> HTTPS server started."));

#else
        // start the HTTP server
        this->server->begin(); // Web server start
        SerialDebug_println(F(" -> HTTP server started."));
#endif
        SerialDebug_print(F(" -> port: "));
        SerialDebug_println(this->_port);
        SerialDebug_println(F("======================"));
    }

// -------------------------------------------- Utils to return HTTP codes --------------------------------------------------------

    void WebServer::replyOK(AsyncWebServerRequest *request) {
        request->send(200, F(CONTENT_TYPE_PLAIN), F(""));
    }

// --------------------------------------------

    void WebServer::replyOKWithMsg(const String &msg, AsyncWebServerRequest *request) {
        request->send(200, F(CONTENT_TYPE_PLAIN), msg);
    }

    void WebServer::replyOKWithJson(const String &resultOfName, AsyncWebServerRequest *request, int code, JsonVariant json)//doc.to<JsonVariant>();
    {
        AsyncJsonResponse * response = new AsyncJsonResponse();// true if is array
        response->addHeader("Server","ESP Async Web Server");
        ArduinoJson6194_F1::VariantRef root = response->getRoot();
        root[F("resultof")] = resultOfName.c_str();
        root[F("value")] = json.as<JsonObject>();
        response->setLength();
        request->send(response);

        /*AsyncResponseStream *response = request->beginResponseStream(F(CONTENT_TYPE_JSON));
        response->addHeader(F(CORS_HEADER), "*");
        response->setCode(code);
        serializeJson(json, *response);
        request->send(response);*/
    }

// --------------------------------------------

    void WebServer::replyNotFound(String msg, AsyncWebServerRequest *request) {
        msg.replace(F("\n"), F("<br>")); //pour affichage sur page web
        String messageHTML = F(
                "<html><head><title>ESP8266 Web Server - Error 404</title><meta charset='UTF-8'><link rel='shortcut icon' href='#'></head><body style='background:#222;color:white;'><div style='text-align:center'><h1>Error 404:</h1><br><h3>Sorry, that page cannot be found!</h3><br></div><p>");
        messageHTML += msg;
        messageHTML += F("</p></body>");
        request->send(404, F(CONTENT_TYPE_HTML), messageHTML);
    }

// --------------------------------------------

    void WebServer::replyBadRequest(const String &msg, AsyncWebServerRequest *request) {
        SerialDebug_println(msg);
        request->send(400, F(CONTENT_TYPE_PLAIN), msg + F("\r\n"));
    }

// --------------------------------------------

    void WebServer::replyServerError(const String &msg, AsyncWebServerRequest *request) {
        SerialDebug_println(msg);
        request->send(500, F(CONTENT_TYPE_PLAIN), msg + F("\r\n"));
    }

// ----------------- Fonction permettant de gérer les pages "not found" (au cas ou le fichier n'existe pas)------------
    void WebServer::handleWebRequests(AsyncWebServerRequest *request) {
        if (request->method() == HTTP_OPTIONS) {
            request->send(200);
            return;
        }
        /*String uri = ESP8266WebServer::urlDecode(server.uri()); // required to read paths with blanks
        // called when the url is not defined here
        // use it to load content from SPIFFS
        */

        // si ne trouve pas le fichier
        String message;
        message.reserve(500);
        message += F("File Not Detected :\n");
        message += F("\n  URI: ");
        message += F("http://");
        message += request->host().c_str();
        message += request->url().c_str();
        message += F("\n  Method: ");
        if (request->method() == HTTP_GET)
            message += F("GET");
        else if (request->method() == HTTP_POST)
            message += F("POST");
        else if (request->method() == HTTP_DELETE)
            message += F("DELETE");
        else if (request->method() == HTTP_PUT)
            message += F("PUT");
        else if (request->method() == HTTP_PATCH)
            message += F("PATCH");
        else if (request->method() == HTTP_HEAD)
            message += F("HEAD");
        else if (request->method() == HTTP_OPTIONS)
            message += F("OPTIONS");
        else
            message += F("UNKNOWN");

        if (request->contentLength()) {
            message += F("\n _CONTENT_TYPE: ");
            message += request->contentType().c_str();
            message += F("\n _CONTENT_LENGTH: ");
            message += request->contentLength();
        }

        int headers = request->headers();
        int i;
        for (i = 0; i < headers; i++) {
            AsyncWebHeader *h = request->getHeader(i);
            message += F("_HEADER[");
            message += h->name().c_str();
            message += F("]: ");
            message += h->value().c_str();
            message += F("\n");
        }

        int params = request->params();
        for (i = 0; i < params; i++) {
            AsyncWebParameter *p = request->getParam(i);
            if (p->isFile()) {
                message += F("_FILE[");
                message += p->name().c_str();
                message += F("]: ");
                message += p->value().c_str();
                message += F(", size: ");
                message += p->size();
                message += F("\n");
            } else if (p->isPost()) {
                message += F("_POST[");
                message += p->name().c_str();
                message += F("]: ");
                message += p->value().c_str();
                message += F("\n");
            } else {
                message += F("_GET[");
                message += p->name().c_str();
                message += F("]: ");
                message += p->value().c_str();
                message += F("\n");
            }
        }
        message += F("\n");
        SerialDebug_println(message);
        return this->replyNotFound(message, request);
    }

// --------------------------------------------

    void WebServer::handleResetWifi(AsyncWebServerRequest *request) { /*
    String messageHTML = F("<html><head><title>ESP8266 Web Server - Reset Wifi</title><meta charset='UTF-8'><link rel='shortcut icon' href='#'></head><body style='text-align:center'><h1>Reset WifiManager:</h1><br><h3>Your ESP is restarting.</h3><br>");
    messageHTML += F("<p>Please look at disponible Wifi Network</p><p><b>SSID :</b> ");
    messageHTML += WifiManager.wifiAll.getAP().getSSID()+F("<br><b>Password :</b> ") + WifiManager.wifiAll.getAP().getPassword();
    messageHTML += F("</p></body>");
    request->send(404, F("text/html"), messageHTML);

    WifiManager.resetWifi();*/
    }

    void WebServer::handleLoginConsole(AsyncWebServerRequest *request) {
        // HTTP basic authentication
        String remoteIP = Utils.IpToString(request->client()->remoteIP());
        if (!request->authenticate(this->_loginConsole_username, this->_loginConsole_password)) {
            if (this->_webEvents != nullptr)
                this->_webEvents->debug_to_WSEvents(F("New login attempt"), this->_webEvents->eventType->error);
            //        writeEvent("WARN", "websrv", "New login attempt", remoteIP);
            return request->requestAuthentication();
        }
        request->send(200, F(CONTENT_TYPE_PLAIN), F("Login Success! /ws/ws.js"));
        if (this->_webEvents != nullptr)
            this->_webEvents->debug_to_WSEvents(F("Login success!"), this->_webEvents->eventType->success);
        //    writeEvent("INFO", "websrv", "Login success!", remoteIP);
    }

    void WebServer::handleScanWifi(AsyncWebServerRequest *request) {
        //First request will return 0 results unless you start scan from somewhere else (loop/setup)
        //Do not request more often than 3-5 seconds
        //https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/scan-class.html#scannetworksasync
        //WiFi.scanNetworksAsync(std::bind(&WebServer::handleScanResult, this, std::placeholders::_1), true); // first param :  the event handler executed when the scan is done
        // second param : set it to true to scan for hidden networks

        request->send(200, F(CONTENT_TYPE_PLAIN), F("Response will be sended by WebSocket"));
        WiFi.scanNetworksAsync(
                [&](int networksFound) {
                    if (this->_webSocket != nullptr)
                        this->_webSocket->sendJsonResultOf(F("ssidlist"), this->handleScanResult(networksFound));
                },
                true);
    }

    DynamicJsonDocument WebServer::handleScanResult(int networksFound) {
        // sort by RSSI
        int n = networksFound;
        int indices[n];
        int skip[n];
        for (int i = 0; i < networksFound; i++) {
            indices[i] = i;
        }
        for (int i = 0; i < networksFound; i++) {
            for (int j = i + 1; j < networksFound; j++) {
                if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i])) {
                    std::swap(indices[i], indices[j]);
                    std::swap(skip[i], skip[j]);
                }
            }
        }
        DynamicJsonDocument doc(JSON_ARRAY_SIZE(networksFound) + networksFound * JSON_OBJECT_SIZE(6));
        JsonArray scan = doc.createNestedArray(F("list"));
        for (int i = 0; i < 5 && i < networksFound; ++i) {
            JsonObject item = scan.createNestedObject();
            item[F("ssid")] = WiFi.SSID(indices[i]).c_str();
            item[F("bssid")] = WiFi.BSSIDstr(indices[i]).c_str();
            item[F("rssi")] = WiFi.RSSI(indices[i]);
            item[F("channel")] = WiFi.channel(indices[i]);
            item[F("enctype")] = WiFi.encryptionType(indices[i]);
            item[F("hidden")] = WiFi.isHidden(indices[i]) ? true : false;
        }

        //if (this->_webSocket != nullptr)
        //    this->_webSocket->sendJsonResultOf(F("ssidlist"),doc);

        WiFi.scanDelete();

        return doc;
    }

    void WebServer::handleGetRealTime(AsyncWebServerRequest *request) {
#ifdef USE_NTP
        request->send(200, F(CONTENT_TYPE_PLAIN), F("Response will be sended by WebSocket"));

        if (this->_webSocket != nullptr)
            this->_webSocket->sendJsonResultOf(F("getRealTime"), NTPManager.getRealTimeJsonObj());
#else
        request->send(200, F(CONTENT_TYPE_PLAIN), F("NTP disabled !"));
#endif // USE_NTP
    }

    void WebServer::handleGetUptime(AsyncWebServerRequest *request) {
#ifdef USE_NTP
        request->send(200, F(CONTENT_TYPE_PLAIN), F("Response will be sended by WebSocket"));

        if (this->_webSocket != nullptr)
            this->_webSocket->sendStringResultOf(F("getUptime"), NTPManager.getUptimeString());
#else
        request->send(200, F(CONTENT_TYPE_PLAIN), F("NTP disabled !"));
#endif // USE_NTP
    }

    void WebServer::handleGetESPInfos(AsyncWebServerRequest *request) {
        request->send(200, F(CONTENT_TYPE_PLAIN), F("Response will be sended by WebSocket"));

        if (this->_webSocket != nullptr)
            this->_webSocket->sendJsonResultOf(F("getStatus"), ESPInfos.toObj());
    }

    void WebServer::handleDebug(AsyncWebServerRequest *request) {
        SerialDebug.setDebug(!SerialDebug.getDebug());
        String msg = F("Serial debug set to : ");
        msg += SerialDebug.getDebug() ? F("true") : F("false");
        request->send(200, F(CONTENT_TYPE_PLAIN), msg);
    }
}