//
// Created by antbd on 05/06/2021.
//

#include "TBD_WiFi_Portail_WebSocket.h"

TBD_WiFi_Portail_WebSocket::TBD_WiFi_Portail_WebSocket(TBD_WiFi_Portail_SerialDebug &serialDebug, TBD_WiFi_Portail_WebServer &webServer /*, TBD_WiFi_Portail_Wifi& wifi*/, const String &root) : _serialDebug(&serialDebug),
                                                                                                                                                                                                 _webServer(&webServer),
                                                                                                                                                                                                 _root(root) /*,
    _wifi(&wifi)*/
{
    this->webSocket = new AsyncWebSocket(_root);

    this->_webEvents = nullptr;
    this->_ntp = nullptr;
    this->_espInfos = nullptr;
    this->_allCommand = new AFArray<PathMethodOnRequestWebSocket>();
}
TBD_WiFi_Portail_WebSocket::~TBD_WiFi_Portail_WebSocket()
{
    delete this->_serialDebug;
    delete this->_webServer;
    delete this->_webEvents;
#ifdef USE_NTP
    delete this->_ntp;
#endif // USE_NTP
    delete this->_espInfos;
    //delete this->_wifi;

    delete this->webSocket;
    delete this->_allCommand;
}

void TBD_WiFi_Portail_WebSocket::addWebEvents(TBD_WiFi_Portail_WebEvents &webEvents)
{
    this->_webEvents = &webEvents;
}

void TBD_WiFi_Portail_WebSocket::addNTP(Service &ntp)
{
    this->_ntp = &ntp;
}

void TBD_WiFi_Portail_WebSocket::addESPInfos(Service &espInfos)
{
    this->_espInfos = &espInfos;
}
void TBD_WiFi_Portail_WebSocket::addCommand(const char *commandNameIn, const char *commandNameOut, HandlerFunctionWebSocket onCommand)
{
    // define command
    PathMethodOnRequestWebSocket pathMethodOnRequestWebSocket(commandNameIn, commandNameOut, onCommand);
    this->_allCommand->add(pathMethodOnRequestWebSocket);
}

// ------------------------------------------ Start a WebSocket server -----------------------------------------
void TBD_WiFi_Portail_WebSocket::begin()
{
    this->_serialDebug->println(F("== Setup WebSocket =="));

    this->webSocket->enable(true);

    if (this->webSocket->enabled())
    {
        this->webSocket->onEvent(/*std::bind(&TBD_WiFi_Portail_WebSocket::onWsEvent, this,std::placeholders::_6)*/
                                 [&](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
                                 { this->onWsEvent(server, client, type, arg, data, len); });
        this->_webServer->server->addHandler(this->webSocket);
        this->_serialDebug->println(F("WebSocket server started. (/ws)"));

        // define commands
        this->addCommand(
            "status",
            "getStatus",
            //std::bind(&TBD_WiFi_Portail_WebSocket::handleCommandStatus, this,std::placeholders::_2)
            [&](int numClient, String value)
            { this->handleCommandStatus(numClient, value); });
        this->addCommand(
            "scan",
            "getScan",
            //std::bind(&TBD_WiFi_Portail_WebSocket::handleCommandScan, this,std::placeholders::_2)
            [&](int numClient, String value)
            { this->handleCommandScan(numClient, value); });
        this->addCommand(
            "realTime",
            "getRealTime",
            //std::bind(&TBD_WiFi_Portail_WebSocket::handleCommandRealTime, this,std::placeholders::_2)
            [&](int numClient, String value)
            { this->handleCommandRealTime(numClient, value); });
        this->addCommand(
            "network",
            "getNetwork",
            //std::bind(&TBD_WiFi_Portail_WebSocket::handleCommandNetwork, this,std::placeholders::_2)
            [&](int numClient, String value)
            { this->handleCommandNetwork(numClient, value); });

        this->_serialDebug->print(F("commands: (nbr="));
        this->_serialDebug->print(this->_allCommand->size());
        this->_serialDebug->println(F(")"));
        while (this->_allCommand->has_next())
        {
            PathMethodOnRequestWebSocket pathMethodOnRequestWebSocketTemp = this->_allCommand->next();

            this->_serialDebug->println((String)F("  ") + pathMethodOnRequestWebSocketTemp.commandNameIn);
        }
    }
    else
    {
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
    ///
    /*
    // check if could send time from ntp
    if (this->_ntp != nullptr) {
        if (this->_ntp->couldSendTime()){
            //this->sendStringResultOf("getRealTime", this->_ntp->getTimeDateString());
            this->ws_send_to_all_client(this->_ntp->getRealTimeString());// {"resultof":"getRealTime","value":{"day":28,"month":6,"year":2021,"hour":0,"min":21,"sec":49}}
            this->_ntp->stopSendTime();
        }
    }*/
}

// ----------------------------------------- When a WebSocket message is received renvoi du bon type de syntaxe --------------------------------
void TBD_WiFi_Portail_WebSocket::onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    if (type == WS_EVT_CONNECT)
    {
        this->_serialDebug->printf(F("[WS][%s][%u] new client connected\n"), server->url(), client->id());
        client->printf("Hello Client %u :)", client->id());
        //client->ping();

        this->envoieAllInfosNewClient(client->id()); //TBD
    }
    else if (type == WS_EVT_DISCONNECT)
    {
        this->_serialDebug->printf(F("[WS][%s][%u] disconnect\n"), server->url(), client->id());
    }
    else if (type == WS_EVT_ERROR)
    {
        this->_serialDebug->printf(F("[WS][%s][%u] error(%u): %s\n"), server->url(), client->id(), *((uint16_t *)arg), (char *)data);
    }
    else if (type == WS_EVT_PONG)
    {
        this->_serialDebug->printf(F("[WS][%s][%u] pong[%u]: %s\n"), server->url(), client->id(), len, (len) ? (char *)data : "");
    }
    else if (type == WS_EVT_DATA)
    {
        AwsFrameInfo *info = (AwsFrameInfo *)arg;
        String msg = F("");
        if (info->final && info->index == 0 && info->len == len)
        {
            //the whole message is in a single frame and we got all of it's data
            this->_serialDebug->printf(F("[WS][%s][%u] %s-message[%llu]: "), server->url(), client->id(), (info->opcode == WS_TEXT) ? F("text") : F("binary"), info->len);

            if (info->opcode == WS_TEXT)
            {
                for (size_t i = 0; i < info->len; i++)
                {
                    msg += (char)data[i];
                }
            }
            else
            {
                char buff[4];
                for (size_t i = 0; i < info->len; i++)
                {
                    sprintf(buff, "%02x ", (uint8_t)data[i]);
                    msg += buff;
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

            if (info->opcode == WS_TEXT)
                client->text(F("(From server) I got your text message"));
            else
                client->binary(F("(From server) I got your binary message"));
        }
        else
        {
            //message is comprised of multiple frames or the frame is split into multiple packets
            if (info->index == 0)
            {
                if (info->num == 0)
                    this->_serialDebug->printf(F("[WS][%s][%u] %s-message start\n"), server->url(), client->id(), (info->message_opcode == WS_TEXT) ? F("text") : F("binary"));
                this->_serialDebug->printf(F("[WS][%s][%u] frame[%u] start[%llu]\n"), server->url(), client->id(), info->num, info->len);
            }

            this->_serialDebug->printf(F("ws[%s][%u] frame[%u] %s[%llu - %llu]: "), server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT) ? F("text") : F("binary"), info->index, info->index + len);

            if (info->opcode == WS_TEXT)
            {
                for (size_t i = 0; i < len; i++)
                {
                    msg += (char)data[i];
                }
            }
            else
            {
                char buff[4];
                for (size_t i = 0; i < len; i++)
                {
                    sprintf(buff, "%02x ", (uint8_t)data[i]);
                    msg += buff;
                }
            }
            this->_serialDebug->println(msg);

            if ((info->index + len) == info->len)
            {
                this->_serialDebug->printf(F("[WS][%s][%u] frame[%u] end[%llu]\n"), server->url(), client->id(), info->num, info->len);
                if (info->final)
                {
                    this->_serialDebug->printf(F("[WS][%s][%u] %s-message end\n"), server->url(), client->id(), (info->message_opcode == WS_TEXT) ? F("text") : F("binary"));
                    if (info->message_opcode == WS_TEXT)
                        client->text(F("(From server) I got your text message"));
                    else
                        client->binary(F("(From server) I got your binary message"));
                }
            }
        }
    }
}

// ------------------------------------- analyse la demande et renvoi les bonnes infos -------------------------------
void TBD_WiFi_Portail_WebSocket::handleReceivedMessage(const String &message, uint32_t numClient)
{
    this->_serialDebug->println(F("handleReceivedMessage"));           //TBD
    DynamicJsonDocument JSONBuffer(JSON_OBJECT_SIZE(2) + 200);         // Memory pool
    DeserializationError error = deserializeJson(JSONBuffer, message); // Parse message

    if (error) // Check for errors in parsing
    {
        if (this->_webEvents != nullptr)
            this->_webEvents->debug_to_WSEvents(F("Parsing ws message failed"), "error");
        else
            this->_serialDebug->println(F("error : Parsing ws message failed"));
        return;
    }

    //{"command":"/goToPosition","value":"STOP"}

    String command = JSONBuffer[F("command")].as<String>(); // Get command
    String value = JSONBuffer[F("value")].as<String>();     // Get value

    bool commandFound = false;
    // check if command was recognized and exist in list
    while (this->_allCommand->has_next() && commandFound == false)
    {
        PathMethodOnRequestWebSocket pathMethodOnRequestWebSocketTemp = this->_allCommand->next();
        if (command == pathMethodOnRequestWebSocketTemp.commandNameIn)
        {
            commandFound = true;
            pathMethodOnRequestWebSocketTemp.onCommand(numClient, value);
            break; // no need to check all function if command existe
        }
    }

    if (!commandFound)
    {
        this->_serialDebug->println(F("Command not recognized"));
        this->ws_send_to_client(numClient, F("Command not regognized"));
    }
}

// -------------------------------------- close WebSocket --------------------------------------------
void TBD_WiFi_Portail_WebSocket::close_WebSocket()
{
    this->_serialDebug->println(F("[WS] Websocket close"));
    // Advertise connected clients what's going on
    if (this->_webEvents != nullptr)
        this->_webEvents->debug_to_WSEvents(F("[WS] Websocket close"));
    // Disable client connections
    this->webSocket->enable(false);

    // Close them
    this->webSocket->closeAll();
}

//-----------------------------------------------------------------------

void TBD_WiFi_Portail_WebSocket::sendJsonByWebsocket(const JsonDocument &doc)
{
    size_t len = measureJsonPretty(doc);
    AsyncWebSocketMessageBuffer *buffer = this->webSocket->makeBuffer(len); //  creates a buffer (len + 1) for you.
    if (buffer)
    {
        //root.printTo((char *)buffer->get(), len + 1);
        serializeJsonPretty(doc, (char *)buffer->get(), len + 1);
        //String response = String();
        //serializeJson(*doc, response);

        this->ws_send_to_all_client(buffer);
        //response = String();
    }
}

//-----------------------------------------------------------------------

void TBD_WiFi_Portail_WebSocket::sendJsonByWebsocket2(DynamicJsonDocument doc)
{
    size_t len = measureJsonPretty(doc);
    AsyncWebSocketMessageBuffer *buffer = this->webSocket->makeBuffer(len); //  creates a buffer (len + 1) for you.
    if (buffer)
    {
        //root.printTo((char *)buffer->get(), len + 1);
        serializeJsonPretty(doc, (char *)buffer->get(), len + 1);
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
void TBD_WiFi_Portail_WebSocket::ws_send_to_client(uint32_t client_id, String message)
{
    this->_serialDebug->println(message);
    if (this->webSocket->enabled())
        this->webSocket->text(client_id, message); // send data to client
}

void TBD_WiFi_Portail_WebSocket::ws_send_to_all_client(String message)
{
    if (this->webSocket->enabled())
        this->webSocket->textAll(message); // send data to all client
}
void TBD_WiFi_Portail_WebSocket::ws_send_to_all_client(AsyncWebSocketMessageBuffer *buffer)
{
    if (this->webSocket->enabled())
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
void TBD_WiFi_Portail_WebSocket::sendStringResultOf(const String &resultOf, const String &result)
{
    DynamicJsonDocument doc(JSON_OBJECT_SIZE(2) + strlen(result.c_str()));
    doc[F("resultof")] = resultOf;
    doc[F("value")] = result;

    sendJsonByWebsocket(doc);
}
/*
void updateObject(JsonObject oldObject, JsonObject newObject) {
    for (JsonPair kv : newObject)
        updateVariant(oldObject[kv.key().c_str()], kv.value());
}

void updateArray(JsonArray oldArray, JsonArray newArray) {
    int index = 0;
    for (auto value : newArray) {
        updateVariant(oldArray[index], newArray[index]);
        index++;
    }
}

void updateVariant(JsonVariant oldVariant, JsonVariant newVariant) {
    if (newVariant.is<JsonObject>())
        updateObject(oldVariant, newVariant);
    else if (newVariant.is<JsonArray>())
        updateArray(oldVariant, newVariant);
    else
        oldVariant.set(newVariant);
}
void merge(JsonObject dest, JsonObjectConst src)
{
    for (auto kvp : src)
    {
        dest[kvp.key()] = kvp.value();
    }
}*/

// ---------------------------------------------------------------
void TBD_WiFi_Portail_WebSocket::sendJsonResultOf(const String &resultOf, const DynamicJsonDocument &result)
{
    //this->_serialDebug->println(F("sendJsonResultOf"));
    //this->_serialDebug->println(result.memoryUsage());
    //this->_serialDebug->println(measureJsonPretty(result));

    DynamicJsonDocument doc(JSON_OBJECT_SIZE(2) + measureJsonPretty(result) + 100);
    doc[F("resultof")] = resultOf;
    doc[F("value")] = result.as<JsonObjectConst>();

    //serializeJsonPretty(doc, Serial);

    sendJsonByWebsocket(doc);
}

//--------------------------------------------- Status --------------------------------------------------
void TBD_WiFi_Portail_WebSocket::handleCommandStatus(int numClient, String value)
{
    this->_serialDebug->println(F("get status"));
    if (this->_espInfos != nullptr)
    {
        //this->_espInfos->sendHardwareInfosByWebSocket();
        this->ws_send_to_client(numClient, this->_espInfos->toString());
    }
    else
    {
        this->ws_send_to_client(numClient, F("ESP Infos not added to WebSocket !"));
    }
}
//--------------------------------------------- Scan --------------------------------------------------
void TBD_WiFi_Portail_WebSocket::handleCommandScan(int numClient, String value)
{
    this->_serialDebug->println(F("getScan"));
    if (this->_webServer != nullptr)
    {
        //WiFi.scanNetworksAsync(std::bind(&TBD_WiFi_Portail_WebServer::this->_webServer->handleScanResult, this,std::placeholders::_1), true); // first param :  the event handler executed when the scan is done
        WiFi.scanNetworksAsync([&](int networksFound)
                               { this->_webServer->handleScanResult(networksFound); },
                               true);
    }
    else
    {
        this->ws_send_to_client(numClient, F("WebServer not added to WebSocket !"));
    }
}

//--------------------------------------------- RealTime --------------------------------------------------
void TBD_WiFi_Portail_WebSocket::handleCommandRealTime(int numClient, String value)
{
    this->_serialDebug->println(F("get realTime"));
#ifdef USE_NTP
    if (this->_ntp != nullptr)
    {
        this->sendJsonResultOf("getRealTime", this->_ntp->toObj());
        ///            this->ws_send_to_all_client(this->_ntp->getRealTimeString());
        //this->ws_send_to_client(numClient, this->_ntp->getRealTimeString());

        //this->sendStringResultOf("getRealTime", this->_ntp->getTimeDateString());
        //this->ws_send_to_client(numClient, this->_ntp->getTimeDateString());// {"resultof":"getRealTime","value":{"day":28,"month":6,"year":2021,"hour":0,"min":21,"sec":49}}
    }
    else
    {
        this->ws_send_to_client(numClient, F("NTP not added to WebSocket !"));
    }
#endif // USE_NTP
}

//--------------------------------------------- Network --------------------------------------------------
void TBD_WiFi_Portail_WebSocket::handleCommandNetwork(int numClient, String value)
{
    this->_serialDebug->println(F("get network"));
    if (this->_webServer->_wifi != nullptr)
    {
        this->sendJsonResultOf("getNetwork", this->_webServer->_wifi->getNetworkInfosJson());
        //this->ws_send_to_all_client(this->_webServer->_wifi->getNetworkInfosString());
    }
    else
    {
        this->ws_send_to_client(numClient, F("Wifi not added to WebSocket !"));
    }
}