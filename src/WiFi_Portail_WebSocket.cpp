//
// Created by antbd on 05/06/2021.
//

#include "WiFi_Portail_WebSocket.h"

namespace WiFi_Portail_API {
    WebSocket::WebSocket(WebServer &webServer, const String &root) :
    _webServer(&webServer), _root(root)
    {
        this->webSocket = new AsyncWebSocket(_root);

        this->_webEvents = nullptr;
        this->_allCommands = new std::vector<PathMethodOnRequestWebSocket>();
    }

    WebSocket::~WebSocket() {
        delete this->_webServer;
        delete this->_webEvents;

        delete this->webSocket;

        this->_allCommands->clear();
        delete this->_allCommands;
    }

    void WebSocket::addWebEvents(WebEvents &webEvents) {
        this->_webEvents = &webEvents;
    }

    void
    WebSocket::addCommand(const char *commandNameIn, const char *commandNameOut, HandlerFunctionWebSocket onCommand) {
        // define command
        PathMethodOnRequestWebSocket pathMethodOnRequestWebSocket(commandNameIn, commandNameOut, onCommand);
        this->_allCommands->push_back(pathMethodOnRequestWebSocket);

    }

    // ------------------------------------------ Start a WebSocket server -----------------------------------------
    void WebSocket::begin() {
        SerialDebug.println(F("== Setup WebSocket =="));

        this->webSocket->enable(true);

        if (this->webSocket->enabled()) {
            this->webSocket->onEvent(/*std::bind(&WebSocket::onWsEvent, this,std::placeholders::_6)*/
                    [&](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg,
                        uint8_t *data, size_t len) { this->onWsEvent(server, client, type, arg, data, len); });
            this->_webServer->server->addHandler(this->webSocket);
            SerialDebug.println(F("WebSocket server started. (/ws)"));

            // define commands
            this->addCommand(
                    "status",
                    "getStatus",
                    //std::bind(&WebSocket::handleCommandStatus, this,std::placeholders::_2)
                    [&](AsyncWebSocketClient *client, String value) { this->handleCommandStatus(client, value); });
            this->addCommand(
                    "scan",
                    "getScan",
                    //std::bind(&WebSocket::handleCommandScan, this,std::placeholders::_2)
                    [&](AsyncWebSocketClient *client, String value) { this->handleCommandScan(client, value); });
            this->addCommand(
                    "realTime",
                    "getRealTime",
                    //std::bind(&WebSocket::handleCommandRealTime, this,std::placeholders::_2)
                    [&](AsyncWebSocketClient *client, String value) { this->handleCommandRealTime(client, value); });
            this->addCommand(
                    "setntp",
                    "getRealTime",
                    //std::bind(&WebSocket::handleCommandRealTime, this,std::placeholders::_2)
                    [&](AsyncWebSocketClient *client, String value) { this->handleCommandRealTime(client, value); });
            this->addCommand(
                    "network",
                    "getNetwork",
                    //std::bind(&WebSocket::handleCommandNetwork, this,std::placeholders::_2)
                    [&](AsyncWebSocketClient *client, String value) { this->handleCommandNetwork(client, value); });
            this->addCommand(
                    "files",
                    "getFiles",
                    //std::bind(&WebSocket::handleCommandFiles, this,std::placeholders::_2)
                    [&](AsyncWebSocketClient *client, String value) { this->handleCommandFiles(client, value); });

            SerialDebug.print(F("commands: (nbr="));
            SerialDebug.print(this->_allCommands->size());
            SerialDebug.println(F(")"));

            for (PathMethodOnRequestWebSocket &command: *this->_allCommands) {
                SerialDebug.println((String) F("  ") + command.commandNameIn);
            }
        } else {
            SerialDebug.println(F("WebSocket server  start failed"));
        }

        SerialDebug.println(F("====================="));
    }

    // -------------------------------------- loop WebSocket --------------------------------------------
    void WebSocket::loop() {
        // Limiting the number of web socket clients
        // https://github.com/me-no-dev/ESPAsyncWebServer#limiting-the-number-of-web-socket-clients
        this->webSocket->cleanupClients();
    }

    // ----------------------------------------- When a WebSocket message is received renvoi du bon type de syntaxe --------------------------------
    void WebSocket::onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
        if (type == WS_EVT_CONNECT) {
            SerialDebug.printf(F("[WS][%s][%u] new client connected\n"), server->url(), client->id());
            client->printf("Hello Client %u :)", client->id());
            //client->ping();

            //this->envoieAllInfosNewClient(client->id()); // TODO : TBD
        } else if (type == WS_EVT_DISCONNECT) {
            SerialDebug.printf(F("[WS][%s][%u] disconnect\n"), server->url(), client->id());
        } else if (type == WS_EVT_ERROR) {
            SerialDebug.printf(F("[WS][%s][%u] error(%u): %s\n"), server->url(), client->id(), *((uint16_t *) arg), (char *) data);
        } else if (type == WS_EVT_PONG) {
            SerialDebug.printf(F("[WS][%s][%u] pong[%u]: %s\n"), server->url(), client->id(), len, (len) ? (char *) data : "");
        } else if (type == WS_EVT_DATA) {
            AwsFrameInfo *info = (AwsFrameInfo *) arg;
            String msg = F("");
            if (info->final && info->index == 0 && info->len == len) {
                //the whole message is in a single frame and we got all of it's data
                SerialDebug.printf(F("[WS][%s][%u] %s-message[%llu]: "), server->url(), client->id(), (info->opcode == WS_TEXT) ? F("text") : F("binary"), info->len);

                if (info->opcode == WS_TEXT) {
                    for (size_t i = 0; i < info->len; i++) {
                        msg += (char) data[i];
                    }
                } else {
                    char buff[4];
                    for (size_t i = 0; i < info->len; i++) {
                        sprintf(buff, "%02x ", (uint8_t) data[i]);
                        msg += buff;
                    }
                }

                if (info->opcode == WS_TEXT)
                    client->text(F("(From server) I got your text message"));
                else
                    client->binary(F("(From server) I got your binary message"));

                //SerialDebug.printf(F("%s\n"),msg.c_str());
                SerialDebug.println(msg);

                //TBD
                if (msg == F("heartbeat")) {
                    SerialDebug.printf(F("[WS][%s][%u] heartbeat\n"), server->url(), client->id());
                } else if (msg.substring(0, 7) == F("Connect")) {
                    SerialDebug.printf(F("[WS][%s][%u] connect\n"), server->url(), client->id());
                } else {
                    this->handleReceivedMessage(msg.c_str(), client);
                    SerialDebug.println(F("=== Traitement WebSocket TERMINE ! ==="));
                }
                //fin TBD

            } else {
                //message is comprised of multiple frames or the frame is split into multiple packets
                if (info->index == 0) {
                    if (info->num == 0)
                        SerialDebug.printf(F("[WS][%s][%u] %s-message start\n"), server->url(), client->id(), (info->message_opcode == WS_TEXT) ? F("text") : F("binary"));
                    SerialDebug.printf(F("[WS][%s][%u] frame[%u] start[%llu]\n"), server->url(), client->id(), info->num, info->len);
                }

                SerialDebug.printf(F("ws[%s][%u] frame[%u] %s[%llu - %llu]: "), server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT) ? F("text") : F("binary"), info->index, info->index + len);

                if (info->opcode == WS_TEXT) {
                    for (size_t i = 0; i < len; i++) {
                        msg += (char) data[i];
                    }
                } else {
                    char buff[4];
                    for (size_t i = 0; i < len; i++) {
                        sprintf(buff, "%02x ", (uint8_t) data[i]);
                        msg += buff;
                    }
                }
                SerialDebug.println(msg);

                if ((info->index + len) == info->len) {
                    SerialDebug.printf(F("[WS][%s][%u] frame[%u] end[%llu]\n"), server->url(), client->id(), info->num, info->len);
                    if (info->final) {
                        SerialDebug.printf(F("[WS][%s][%u] %s-message end\n"), server->url(), client->id(), (info->message_opcode == WS_TEXT) ? F("text") : F("binary"));
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
    void WebSocket::handleReceivedMessage(const String &message, AsyncWebSocketClient *client) {
        SerialDebug.println(F("handleReceivedMessage"));           //TBD
        DynamicJsonDocument JSONBuffer(JSON_OBJECT_SIZE(2) + 200);         // Memory pool
        DeserializationError error = deserializeJson(JSONBuffer, message); // Parse message

        if (error) // Check for errors in parsing
        {
            if (this->_webEvents != nullptr)
                this->_webEvents->debug_to_WSEvents(F("Parsing ws message failed"), "error");
            else
                SerialDebug.println(F("error : Parsing ws message failed"));
            return;
        }

        //{"command":"/goToPosition","value":"STOP"}

        String command = JSONBuffer[F("command")].as<String>(); // Get command
        String value = JSONBuffer[F("value")].as<String>();     // Get value

        bool commandFound = false;
        // check if command was recognized and exist in list
        for (PathMethodOnRequestWebSocket &commandTemp: *this->_allCommands) {
            if (commandFound == false && command == commandTemp.commandNameIn) {
                commandFound = true;
                commandTemp.onCommand(client, value);
                break; // no need to check all function if command existe
            }
        }

        if (!commandFound) {
            SerialDebug.println((String) F("Command not recognized ") + command);
            this->send((String) F("Command not regognized ") + command, client);
        }
    }

    // -------------------------------------- close WebSocket --------------------------------------------
    void WebSocket::close_WebSocket() {
        SerialDebug.println(F("[WS] Websocket close"));
        // Advertise connected clients what's going on
        if (this->_webEvents != nullptr)
            this->_webEvents->debug_to_WSEvents(F("[WS] Websocket close"));
        // Disable client connections
        this->webSocket->enable(false);

        // Close them
        this->webSocket->closeAll();
    }

    //-----------------------------------------------------------------------

    // send json to one client if defined else to all clients
    void WebSocket::send(const JsonDocument &doc, AsyncWebSocketClient *client) {
        if (this->webSocket->enabled()) {
#ifdef WEBSOCKETSENDJSONPRETTY
            size_t len = measureJsonPretty(doc);
            AsyncWebSocketMessageBuffer *buffer = this->webSocket->makeBuffer(len); //  creates a buffer (len + 1) for you.

            if (buffer) {
                serializeJsonPretty(doc, (char *) buffer->get(), len + 1);
#else
            size_t len = measureJson(doc);
            AsyncWebSocketMessageBuffer *buffer = this->webSocket->makeBuffer(len); //  creates a buffer (len + 1) for you.

            if (buffer) {
                serializeJson(doc, (char *) buffer->get(), len + 1);
#endif
                String print = F("send json: ");
                //print += (char *) buffer->get();
                print += F("\nsize: ");
                print += len;
                SerialDebug.println(print);
                if (client != nullptr) {
                    client->text(buffer); // send data to one client
                } else {
                    this->webSocket->textAll(buffer);// send data to all client
                }
            }
        }
    }

    // send string to one client if defined else to all clients
    void WebSocket::send(const String &data, AsyncWebSocketClient *client) {
        if (this->webSocket->enabled()) {
            if (client != nullptr) {
                client->text(data); // send data to one client
            } else {
                this->webSocket->textAll(data);// send data to all client
            }
        }
    }

    // ---------------------------------------------------------------
    // convert to json with name of the request and result value
    void WebSocket::sendStringResultOf(const String &resultOfName, const String &result, AsyncWebSocketClient *client) {
        //SerialDebug.println("sendStringResultOf: " +result + "\nsize: "+strlen(result.c_str()));
        DynamicJsonDocument doc(JSON_OBJECT_SIZE(2) + strlen(result.c_str()) + 10);
        doc[F("resultof")] = resultOfName.c_str();
        doc[F("value")] = result.c_str();

        //serializeJsonPretty(doc, Serial);

        this->send(doc, client);
    }

    void WebSocket::sendJsonResultOf(const String &resultOfName, const DynamicJsonDocument &result, AsyncWebSocketClient *client) {
        //SerialDebug.println(F("sendJsonResultOf"));
        //SerialDebug.println(result.memoryUsage());
        //SerialDebug.println(measureJsonPretty(result));
        //serializeJsonPretty(result, Serial);

        DynamicJsonDocument doc(JSON_OBJECT_SIZE(2) + result.memoryUsage() + 100);
        doc[F("resultof")] = resultOfName.c_str();
        doc[F("value")] = result.as<JsonObjectConst>();

        //serializeJsonPretty(doc, Serial);

        this->send(doc, client);
    }
    //-----------------------------------------------------------------------

    /*
    // --------------------------------- envoi des informations au nouveau client ---------------------------------------
    void WebSocket::envoieAllInfosNewClient(uint32_t client_id)
    {
        // ws_send_to_client(client_id, handlePositionsPredefine().c_str()); // send data to client
        // ws_send_to_client(client_id, handleInfos().c_str()); // send data to client
        // ws_send_to_client(client_id, handleHardwareInfos().c_str()); // send data to client
    }

    void WebSocket::envoieHardwareInfosToClient(uint32_t client_id)
    {
        // ws_send_to_client(client_id, handleHardwareInfos().c_str()); // send data to client
    }

    void WebSocket::envoieValuesToClient(uint32_t client_id)
    {
        // ws_send_to_client(client_id, handleInfos().c_str()); // send data to client
    }
    */


    //--------------------------------------------- Status --------------------------------------------------
    void WebSocket::handleCommandStatus(AsyncWebSocketClient * client, String value) {
        SerialDebug.println(F("get status"));
            this->sendJsonResultOf(F("getStatus"),
            ESPInfos.toObj(), client);

    }

    //--------------------------------------------- Scan --------------------------------------------------
    void WebSocket::handleCommandScan(AsyncWebSocketClient *client, String value) {
        SerialDebug.println(F("getScan"));
        if (this->_webServer != nullptr) {
            WiFi.scanNetworksAsync(
                    [&](int networksFound) {
                        if (this->_webServer != nullptr)
                            this->sendJsonResultOf(F("ssidlist"), this->_webServer->handleScanResult(networksFound));
                    },
                    true);    // second param : set it to true to scan for hidden networks
        } else {
            this->send(F("WebServer not added to WebSocket !"), client);
        }
    }

    //--------------------------------------------- RealTime --------------------------------------------------
    void WebSocket::handleCommandRealTime(AsyncWebSocketClient *client, String value) {
        SerialDebug.println(F("get realTime"));
#ifdef USE_NTP
        this->sendJsonResultOf(F("getRealTime"), NTPManager.getRealTimeJsonObj()/*, client*/);// send to all client
#else
        this->send(F("NTP not added to WebSocket !"), client);
#endif // USE_NTP
    }

    void WebSocket::handleCommandSetNtp(AsyncWebSocketClient *client, String value) {
        SerialDebug.println(F("set ntp"));
#ifdef USE_NTP



        this->sendJsonResultOf(F("getRealTime"), NTPManager.getRealTimeJsonObj());// send to all client
#else
        this->send(F("NTP not added to WebSocket !"), client);
#endif // USE_NTP
    }

    //--------------------------------------------- Network --------------------------------------------------
    void WebSocket::handleCommandNetwork(AsyncWebSocketClient *client, String value) {
        SerialDebug_println(F("get network"));
        this->sendJsonResultOf(F("getNetwork"), WifiManager.getNetworkInfosJson(), client);
    }

    //--------------------------------------------- Files --------------------------------------------------
    void WebSocket::handleCommandFiles(AsyncWebSocketClient *client, String value) {
        SerialDebug_println(F("get files"));
        String temp = FileSystem.getFilesListJson();
        DynamicJsonDocument docTemp(JSON_OBJECT_SIZE(1) + JSON_ARRAY_SIZE(1) + strlen(temp.c_str()) * 2 + 10);
        deserializeJson(docTemp, temp);
        this->sendJsonResultOf(F("getFiles"), docTemp, client);
    }

}