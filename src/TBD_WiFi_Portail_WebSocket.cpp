//
// Created by antbd on 05/06/2021.
//

#include "TBD_WiFi_Portail_WebSocket.h"

TBD_WiFi_Portail_WebSocket::TBD_WiFi_Portail_WebSocket(TBD_WiFi_Portail_SerialDebug& serialDebug,TBD_WiFi_Portail_WebServer& webServer, String root):_serialDebug(&serialDebug), _webServer(&webServer), _root(root) {
    this->webSocket = new AsyncWebSocket(_root);

    this->_webEvents = nullptr;
}
TBD_WiFi_Portail_WebSocket::~TBD_WiFi_Portail_WebSocket() {}

void TBD_WiFi_Portail_WebSocket::addWebEvents(TBD_WiFi_Portail_WebEvents& webEvents){
    this->_webEvents = &webEvents;
}

// ------------------------------------------ Start a WebSocket server -----------------------------------------
void TBD_WiFi_Portail_WebSocket::begin() {
    this->_serialDebug->println(F("== Setup WebSocket =="));

    this->webSocket->enable(true);

    if(this->webSocket->enabled()){
        this->webSocket->onEvent(/*std::bind(&TBD_WiFi_Portail_WebSocket::onWsEvent, this,std::placeholders::_6)*/
                [&] (AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){this->onWsEvent(server, client, type, arg, data, len);});
        this->_webServer->server->addHandler(this->webSocket);
        this->_serialDebug->println(F("WebSocket server started. (/ws)"));
    }else {
        this->_serialDebug->println(F("WebSocket server  start failed"));
    }


    this->_serialDebug->println(F("====================="));
}

// -------------------------------------- loop WebSocket --------------------------------------------
void TBD_WiFi_Portail_WebSocket::loop()
{
    // Limiting the number of web socket clients
    // https://github.com/me-no-dev/ESPAsyncWebServer#limiting-the-number-of-web-socket-clients
    this->webSocket->cleanupClients();
}

// ----------------------------------------- When a WebSocket message is received renvoi du bon type de syntaxe --------------------------------
void TBD_WiFi_Portail_WebSocket::onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
    if(type == WS_EVT_CONNECT){
        this->_serialDebug->printf(F("[WS][%s][%u] new client connected\n"), server->url(), client->id());
        client->printf("Hello Client %u :)", client->id());
        //client->ping();

        this->envoieAllInfosNewClient(client->id());//TBD

    } else if(type == WS_EVT_DISCONNECT){
        this->_serialDebug->printf(F("[WS][%s][%u] disconnect\n"), server->url(), client->id());
    } else if(type == WS_EVT_ERROR){
        this->_serialDebug->printf(F("[WS][%s][%u] error(%u): %s\n"), server->url(), client->id(), *((uint16_t*)arg), (char*)data);
    } else if(type == WS_EVT_PONG){
        this->_serialDebug->printf(F("[WS][%s][%u] pong[%u]: %s\n"), server->url(), client->id(), len, (len)?(char*)data:"");
    } else if(type == WS_EVT_DATA){
        AwsFrameInfo * info = (AwsFrameInfo*)arg;
        String msg = F("");
        if(info->final && info->index == 0 && info->len == len){
            //the whole message is in a single frame and we got all of it's data
            this->_serialDebug->printf(F("[WS][%s][%u] %s-message[%llu]: "), server->url(), client->id(), (info->opcode == WS_TEXT) ? F("text") : F("binary"), info->len);

            if(info->opcode == WS_TEXT){
                for(size_t i=0; i < info->len; i++) {
                    msg += (char) data[i];
                }
            } else {
                char buff[4];
                for(size_t i=0; i < info->len; i++) {
                    sprintf(buff, "%02x ", (uint8_t) data[i]);
                    msg += buff ;
                }
            }
            //this->_serialDebug->printf(F("%s\n"),msg.c_str());
            this->_serialDebug->println(msg);

            //TBD
            if (msg == F("heartbeat"))
            {
                this->_serialDebug->printf(F("[WS][%s][%u] heartbeat\n"), server->url(), client->id());
            }
            else if (msg.substring(0, 7) == F("Connect"))
            {
                this->_serialDebug->printf(F("[WS][%s][%u] connect\n"), server->url(), client->id());
            }
            else
            {
                this->handleReceivedMessage(msg.c_str(), client->id());
                this->_serialDebug->println(F("=== Traitement WebSocket TERMINE ! ==="));
            }
            //fin TBD

            if(info->opcode == WS_TEXT)
                client->text(F("(From server) I got your text message"));
            else
                client->binary(F("(From server) I got your binary message"));
        } else {
            //message is comprised of multiple frames or the frame is split into multiple packets
            if(info->index == 0){
                if(info->num == 0)
                    this->_serialDebug->printf(F("[WS][%s][%u] %s-message start\n"), server->url(), client->id(), (info->message_opcode == WS_TEXT) ? F("text") : F("binary"));
                this->_serialDebug->printf(F("[WS][%s][%u] frame[%u] start[%llu]\n"), server->url(), client->id(), info->num, info->len);
            }

            this->_serialDebug->printf(F("ws[%s][%u] frame[%u] %s[%llu - %llu]: "), server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT) ? F("text") : F("binary"), info->index, info->index + len);

            if(info->opcode == WS_TEXT){
                for(size_t i=0; i < len; i++) {
                    msg += (char) data[i];
                }
            } else {
                char buff[4];
                for(size_t i=0; i < len; i++) {
                    sprintf(buff, "%02x ", (uint8_t) data[i]);
                    msg += buff ;
                }
            }
            this->_serialDebug->println(msg);

            if((info->index + len) == info->len){
                this->_serialDebug->printf(F("[WS][%s][%u] frame[%u] end[%llu]\n"), server->url(), client->id(), info->num, info->len);
                if(info->final){
                    this->_serialDebug->printf(F("[WS][%s][%u] %s-message end\n"), server->url(), client->id(), (info->message_opcode == WS_TEXT) ? F("text") : F("binary"));
                    if(info->message_opcode == WS_TEXT)
                        client->text(F("(From server) I got your text message"));
                    else
                        client->binary(F("(From server) I got your binary message"));
                }
            }
        }
    }
}


// ------------------------------------- analyse la demande et renvoi les bonnes infos -------------------------------
void TBD_WiFi_Portail_WebSocket::handleReceivedMessage(String message, uint32_t numClient)
{
    this->_serialDebug->println(F("handleReceivedMessage"));//TBD
}


// -------------------------------------- close WebSocket --------------------------------------------
void TBD_WiFi_Portail_WebSocket::close_WebSocket()
{
    this->_serialDebug->println(F("[WS] Websocket close"));
    // Advertise connected clients what's going on
    if(this->_webEvents != nullptr) this->_webEvents->debug_to_WSEvents(F("[WS] Websocket close"));
    // Disable client connections
    this->webSocket->enable(false);

    // Close them
    this->webSocket->closeAll();
}

//-----------------------------------------------------------------------

void TBD_WiFi_Portail_WebSocket::sendJsonByWebsocket(DynamicJsonDocument *doc) {
    size_t len = measureJson(*doc);
    AsyncWebSocketMessageBuffer *buffer = this->webSocket->makeBuffer(len); //  creates a buffer (len + 1) for you.
    if (buffer) {
        //root.printTo((char *)buffer->get(), len + 1);
        serializeJson(*doc, (char *)buffer->get(), len + 1);
        //String response = String();
        //serializeJson(*doc, response);

        this->ws_send_to_all_client(buffer);
        //response = String();
    }
}

/*void TBD_WiFi_Portail_WebSocket::sendDataWs(AsyncWebSocketClient * client)
{
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["a"] = "abc";
    root["b"] = "abcd";
    root["c"] = "abcde";
    root["d"] = "abcdef";
    root["e"] = "abcdefg";
    size_t len = root.measureLength();
    AsyncWebSocketMessageBuffer * buffer = this->webSocket->makeBuffer(len); //  creates a buffer (len + 1) for you.
    if (buffer) {
        root.printTo((char *)buffer->get(), len + 1);
        if (client) {
            client->text(buffer);
        } else {
            this->webSocket->textAll(buffer);
        }
    }
}*/

// -------------------------------------------------------------------------
void TBD_WiFi_Portail_WebSocket::ws_send_to_client(uint32_t client_id, const char* message) {
    if(this->webSocket->enabled())
        this->webSocket->text(client_id, message); // send data to client
}

void TBD_WiFi_Portail_WebSocket::ws_send_to_all_client(char* message) {
    if(this->webSocket->enabled())
        this->webSocket->textAll(message); // send data to all client
}
void TBD_WiFi_Portail_WebSocket::ws_send_to_all_client(AsyncWebSocketMessageBuffer * buffer) {
    if(this->webSocket->enabled())
        this->webSocket->textAll(buffer); // send data to all client
}


// --------------------------------- envoi des informations au nouveau client ---------------------------------------
void TBD_WiFi_Portail_WebSocket::envoieAllInfosNewClient(uint32_t client_id)
{
    // ws_send_to_client(client_id, handlePositionsPredefine().c_str()); // send data to client
    // ws_send_to_client(client_id, handleInfos().c_str()); // send data to client
    // ws_send_to_client(client_id, handleHardwareInfos().c_str()); // send data to client
}

void TBD_WiFi_Portail_WebSocket::envoieHardwareInfosToClient(uint32_t client_id)
{
    // ws_send_to_client(client_id, handleHardwareInfos().c_str()); // send data to client
}

void TBD_WiFi_Portail_WebSocket::envoieValuesToClient(uint32_t client_id)
{
    // ws_send_to_client(client_id, handleInfos().c_str()); // send data to client
}

// ---------------------------------------------------------------
void TBD_WiFi_Portail_WebSocket::sendStringResultOf(String resultOf, String result){
    DynamicJsonDocument doc(JSON_OBJECT_SIZE(2) + strlen(result.c_str()));
    doc["resultof"] = resultOf;
    doc["result"] = result;

    sendJsonByWebsocket(&doc);
}