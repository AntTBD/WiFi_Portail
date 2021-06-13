//
// Created by antbd on 05/06/2021.
//

#include "TBD_WiFi_Portail_WebServer.h"

TBD_WiFi_Portail_WebServer::TBD_WiFi_Portail_WebServer(TBD_WiFi_Portail_SerialDebug& serialDebug, TBD_WiFi_Portail_FileSystem& fileSystem, TBD_WiFi_Portail_Wifi& wifi, int port):_serialDebug(&serialDebug),  _fileSystem(&fileSystem), _wifi(&wifi), _port(port){
    this->server = new AsyncWebServer(80);
    this->_loginConsole_username = "admin";
    this->_loginConsole_password = "admin";

    this->_webEvents = nullptr;
    this->_webSocket = nullptr;
    this->_ntp = nullptr;
}
TBD_WiFi_Portail_WebServer::~TBD_WiFi_Portail_WebServer() {}

void TBD_WiFi_Portail_WebServer::addRoot(const char *uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest) {

    // define root
    PathMethodOnRequest pathMethodOnRequest(uri, method, onRequest);
    this->_allRoot.add(pathMethodOnRequest);
}


void TBD_WiFi_Portail_WebServer::addWebEvents(TBD_WiFi_Portail_WebEvents& webEvents){
    this->_webEvents = &webEvents;
}
void TBD_WiFi_Portail_WebServer::addWebSocket(TBD_WiFi_Portail_WebSocket& webSocket){
    this->_webSocket = &webSocket;
}


void TBD_WiFi_Portail_WebServer::addNTP(TBD_WiFi_Portail_NTP& ntp){
    this->_ntp = &ntp;
}

void TBD_WiFi_Portail_WebServer::begin() {
    this->_serialDebug->println(F("== Setup Web Server =="));

    // define roots
    this->addRoot(
            "/resetWifi",
            HTTP_GET,
            //std::bind(&TBD_WiFi_Portail_WebServer::handleResetWifi, this,std::placeholders::_1)
            [&](AsyncWebServerRequest *request) {this->handleResetWifi(request);}
            );
    this->addRoot(
            "/scan",
            HTTP_GET,
            //std::bind(&TBD_WiFi_Portail_WebServer::handleScanWifi, this,std::placeholders::_1)
            [&](AsyncWebServerRequest *request) {this->handleScanWifi(request);}
            );
    this->addRoot(
            "/login_console",
            HTTP_POST,
            //std::bind(&TBD_WiFi_Portail_WebServer::handleLoginConsole, this,std::placeholders::_1)
            [&](AsyncWebServerRequest *request) {this->handleLoginConsole(request);}
            );
    this->addRoot(
            "/getRealTime",
            HTTP_GET,
            //std::bind(&TBD_WiFi_Portail_WebServer::handleGetRealTime, this,std::placeholders::_1)
            [&](AsyncWebServerRequest *request) {this->handleGetRealTime(request);}
    );
    this->addRoot(
            "/getUptime",
            HTTP_GET,
            //std::bind(&TBD_WiFi_Portail_WebServer::handleGetUptime, this,std::placeholders::_1)
            [&](AsyncWebServerRequest *request) {this->handleGetUptime(request);}
    );

    this->_serialDebug->print(F("root: (nbr="));
    this->_serialDebug->print(this->_allRoot.size()+3);
    this->_serialDebug->println(F(")"));

    // add preconfigurated root with function
    while (this->_allRoot.has_next()) {
        PathMethodOnRequest pathMethodOnRequestTemp = this->_allRoot.next();
        this->server->on(pathMethodOnRequestTemp.uri, pathMethodOnRequestTemp.method, pathMethodOnRequestTemp.onRequest);
        this->_serialDebug->print(F(" "));
        this->_serialDebug->println(pathMethodOnRequestTemp.uri);
    }

    //Serve different site files in AP or STA mode
    this->server
            ->serveStatic("/", *this->_fileSystem->fileSystem, "/www/")
            .setDefaultFile("index.html")
            .setFilter(ON_STA_FILTER);
    this->_serialDebug->println(F(" /     (in STA Mode)"));
    this->server
            ->serveStatic("/", *this->_fileSystem->fileSystem, "/ap/")
            .setDefaultFile("index.html")
            .setFilter(ON_AP_FILTER);
    this->_serialDebug->println(F(" /     (in AP Mode)"));
    //server->addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);//only when requested from AP
    this->server
            ->serveStatic("/ws/", *this->_fileSystem->fileSystem, "/ws/")
            .setDefaultFile("ws.html");
    //.setAuthentication("_user", "pass");
    this->_serialDebug->println(F(" /ws/"));


    this->server->onNotFound(
            //std::bind(&TBD_WiFi_Portail_WebServer::handleWebRequests, this,std::placeholders::_1)
            [&](AsyncWebServerRequest *request) {this->handleWebRequests(request);}
            ); // if someone requests any other file or page, go to function 'handleWebRequests'
    this->_serialDebug->println(F("onNotFound : handleWebRequests"));

    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    this->_serialDebug->println(F("default header : Access-Control-Allow-Origin : *"));

    // start the HTTP server
    this->server->begin(); // Web server start

    this->_serialDebug->println(F(" -> HTTP server started."));
    this->_serialDebug->println(F("======================"));
}


// -------------------------------------------- Utils to return HTTP codes --------------------------------------------------------


void TBD_WiFi_Portail_WebServer::replyOK(AsyncWebServerRequest *request)
{
    request->send(200, F("text/plain"), F(""));
}

// --------------------------------------------

void TBD_WiFi_Portail_WebServer::replyOKWithMsg(String msg, AsyncWebServerRequest *request)
{
    request->send(200, F("text/plain"), msg);
}

// --------------------------------------------

void TBD_WiFi_Portail_WebServer::replyNotFound(String msg, AsyncWebServerRequest *request)
{
    msg.replace(F("\n"), F("<br>")); //pour affichage sur page web
    String messageHTML = F("<html><head><title>ESP8266 Web Server - Error 404</title><meta charset='UTF-8'><link rel='shortcut icon' href='#'></head><body><div style='text-align:center'><h1>Error 404:</h1><br><h3>Sorry, that page cannot be found!</h3><br></div><p>");
    messageHTML += msg;
    messageHTML += F("</p></body>");
    request->send(404, F("text/html"), messageHTML);
}

// --------------------------------------------

void TBD_WiFi_Portail_WebServer::replyBadRequest(String msg, AsyncWebServerRequest *request)
{
    this->_serialDebug->println(msg);
    request->send(400, F("text/plain"), msg + F("\r\n"));
}

// --------------------------------------------

void TBD_WiFi_Portail_WebServer::replyServerError(String msg, AsyncWebServerRequest *request)
{
    this->_serialDebug->println(msg);
    request->send(500, F("text/plain"), msg + F("\r\n"));
}

// ----------------- Fonction permettant de gérer les pages "not found" (au cas ou le fichier n'existe pas)------------
void TBD_WiFi_Portail_WebServer::handleWebRequests(AsyncWebServerRequest *request)
{
    /*String uri = ESP8266WebServer::urlDecode(server.uri()); // required to read paths with blanks
    // called when the url is not defined here
    // use it to load content from SPIFFS
    */

    // si ne trouve pas le fichier
    String message;
    message.reserve(255);
    message = F("File Not Detected :\n");
    message += F("\n  URI: ");
    message += F("http://");
    message += request->host().c_str();
    message += request->url().c_str();
    message += F("\n  Method: ");
    if(request->method() == HTTP_GET)
        message +=F("GET");
    else if(request->method() == HTTP_POST)
        message +=F("POST");
    else if(request->method() == HTTP_DELETE)
        message +=F("DELETE");
    else if(request->method() == HTTP_PUT)
        message +=F("PUT");
    else if(request->method() == HTTP_PATCH)
        message +=F("PATCH");
    else if(request->method() == HTTP_HEAD)
        message +=F("HEAD");
    else if(request->method() == HTTP_OPTIONS)
        message +=F("OPTIONS");
    else
        message +=F("UNKNOWN");

    if(request->contentLength()){
        message+= F("\n _CONTENT_TYPE: ");
        message += request->contentType().c_str();
        message += F("\n _CONTENT_LENGTH: ");
        message += request->contentLength();
    }

    int headers = request->headers();
    int i;
    for(i=0;i<headers;i++){
        AsyncWebHeader* h = request->getHeader(i);
        message += F("_HEADER[");
        message += h->name().c_str();
        message += F("]: ");
        message += h->value().c_str();
        message += F("\n");
    }

    int params = request->params();
    for(i=0;i<params;i++){
        AsyncWebParameter* p = request->getParam(i);
        if(p->isFile()){
            message += F("_FILE[");
            message += p->name().c_str();
            message += F("]: ");
            message += p->value().c_str();
            message += F(", size: ");
            message += p->size();
            message += F("\n");

        } else if(p->isPost()){
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
    this->_serialDebug->println(message);
    return this->replyNotFound(message, request);
}


// --------------------------------------------

void TBD_WiFi_Portail_WebServer::handleResetWifi(AsyncWebServerRequest *request) {/*
    String messageHTML = F("<html><head><title>ESP8266 Web Server - Reset Wifi</title><meta charset='UTF-8'><link rel='shortcut icon' href='#'></head><body style='text-align:center'><h1>Reset WifiManager:</h1><br><h3>Your ESP is restarting.</h3><br>");
    messageHTML += F("<p>Please look at disponible Wifi Network</p><p><b>SSID :</b> ");
    messageHTML += _wifi->wifiAll.getAP().getSSID()+F("<br><b>Password :</b> ") + _wifi->wifiAll.getAP().getPassword();
    messageHTML += F("</p></body>");
    request->send(404, F("text/html"), messageHTML);

    this->_wifi->resetWifi();*/
}

void TBD_WiFi_Portail_WebServer::handleLoginConsole(AsyncWebServerRequest *request) {
    // HTTP basic authentication
    String remoteIP = IpToString(request->client()->remoteIP());
    if(!request->authenticate(_loginConsole_username, _loginConsole_password)){
        if(this->_webEvents != nullptr) this->_webEvents->debug_to_WSEvents(F("New login attempt"), this->_webEvents->eventType->error);
//        writeEvent("WARN", "websrv", "New login attempt", remoteIP);
        return request->requestAuthentication();
    }
    request->send(200, F("text/plain"), F("Login Success! /ws/ws.js"));
    if(this->_webEvents != nullptr) this->_webEvents->debug_to_WSEvents(F("Login success!"), this->_webEvents->eventType->success);
//    writeEvent("INFO", "websrv", "Login success!", remoteIP);
}

void TBD_WiFi_Portail_WebServer::handleScanWifi(AsyncWebServerRequest *request){
    //First request will return 0 results unless you start scan from somewhere else (loop/setup)
    //Do not request more often than 3-5 seconds
    //https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/scan-class.html#scannetworksasync
    WiFi.scanNetworksAsync(std::bind(&TBD_WiFi_Portail_WebServer::handleScanResult, this,std::placeholders::_1), true); // first param :  the event handler executed when the scan is done
    // second param : set it to true to scan for hidden networks
    request->send(200, F("text/plain"), F("Response will be sended by WebSocket"));

}
void TBD_WiFi_Portail_WebServer::handleScanResult(int networksFound) {
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
    DynamicJsonDocument doc(JSON_ARRAY_SIZE(networksFound) + JSON_OBJECT_SIZE(2) + networksFound*JSON_OBJECT_SIZE(6));
    doc["resultof"] = "ssidlist";
    JsonArray scan = doc.createNestedArray("list");
    for (int i = 0; i < 5 && i < networksFound; ++i) {
        JsonObject item = scan.createNestedObject();
        item["ssid"] = WiFi.SSID(indices[i]);
        item["bssid"] = WiFi.BSSIDstr(indices[i]);
        item["rssi"] = WiFi.RSSI(indices[i]);
        item["channel"] = WiFi.channel(indices[i]);
        item["enctype"] = WiFi.encryptionType(indices[i]);
        item["hidden"] = WiFi.isHidden(indices[i]) ? true : false;
    }

    if(this->_webSocket != nullptr) this->_webSocket->sendJsonByWebsocket(&doc);

    WiFi.scanDelete();
}

void TBD_WiFi_Portail_WebServer::handleGetRealTime(AsyncWebServerRequest *request){
    if(this->_ntp != nullptr) {
        this->_ntp->getTimeDateString();
        request->send(200, F("text/plain"), F("Response will be sended by WebSocket"));
    }else{
        request->send(200, F("text/plain"), F("NTP not added to webServer !"));
    }

}
void TBD_WiFi_Portail_WebServer::handleGetUptime(AsyncWebServerRequest *request){
    if(this->_ntp != nullptr) {
        request->send(200, F("text/plain"), F("Response will be sended by WebSocket"));
        this->_ntp->sendUptimeByWebSocket();
    }else{
        request->send(200, F("text/plain"), F("NTP not added to webServer !"));
    }

}