//
// Created by antbd on 05/06/2021.
//

#include "TBD_WiFi_Portail_WebEvents.h"

namespace WiFi_Portail_API {

    WebEvents::WebEvents( WebServer &webServer, const String &eventsRoot, const String &eventsAdminRoot) :
            _webServer(&webServer), _eventsRoot(eventsRoot), _eventsAdminRoot(eventsAdminRoot) {

        this->events = new AsyncEventSource(this->_eventsRoot);
        this->eventsAdmin = new AsyncEventSource(this->_eventsAdminRoot);
        this->_adminUser = F("userAdmin");
        this->_adminPass = F("passAdmin");
        this->eventType = new sEventType();
    }

    WebEvents::~WebEvents() {
        delete this->_webServer;
        delete this->eventType;

        delete this->events;
        delete this->eventsAdmin;

    }

    void WebEvents::setEventsAdmin_UserPass(const String &user, const String &password) {
        this->_adminUser = user;
        this->_adminPass = password;
    }


    void WebEvents::begin() // Start a WebEvents server
    {
        SerialDebug_println(F("== Setup WebEvents =="));

        this->events->onConnect([&](AsyncEventSourceClient *client) {
            if (client->lastId()) {
                SerialDebug_print(F("[Events] Client reconnected! Last message ID that it get is: "));
                SerialDebug_println((String) client->lastId());
            }
            //send event with message "hello!", id current millis
            // and set reconnect delay to 1 second
            client->send("Hello from IKEA PS 2014!", NULL, millis(), 500);//1000
            String msg = F("New client connected ");
            msg += client->lastId();
            debug_to_WSEvents(msg, this->eventType->debug);
        });
        this->_webServer->server->addHandler(this->events);
        SerialDebug_printf(F(" -> Events server      started. ( %s )\n"), this->_eventsRoot.c_str());

        this->eventsAdmin->onConnect([&](AsyncEventSourceClient *client) {
            if (client->lastId()) {
                SerialDebug_print(
                        F("[EventsAdmin] Admin client reconnected! Last message ID that it get is: "));
                SerialDebug_println((String) client->lastId());
            }
            //send event with message "hello!", id current millis
            // and set reconnect delay to 1 second
            client->send("Hello from IKEA PS 2014! (Admin)", NULL, millis(), 500);//1000
            String msg = F("New admin client connected ");
            msg += client->lastId();
            debug_to_WSEvents(msg, this->eventType->debug);
        });
        //HTTP Authenticate before switch to Websocket protocol
        //eventsAdmin->setAuthentication("userAdmin", "passAdmin");
        this->_webServer->server->addHandler(this->eventsAdmin);
        SerialDebug_printf(F(" -> EventsAdmin server started. ( %s ) user: %s / pass: %s \n"),
                                   this->_eventsAdminRoot.c_str(), this->_adminUser.c_str(), this->_adminPass.c_str());

        SerialDebug_println(F("====================="));
    }

// --------------------------------- send messsage to debug via websocket events ---------------------------------------
    void WebEvents::debug_to_WSEvents(const String &message, const String &type, int niveau) {
        SerialDebug_print(F("debug_to_WSEvents : "));
        SerialDebug_println(message);
        if (type == this->eventType->info) {
            this->events->send(message.c_str(), type.c_str(), millis()); //send data to all clients
        } else {
            this->eventsAdmin->send(message.c_str(), type.c_str(), millis()); //send data to all admin clients
        }
    }


// --------------------------------- send informations to all clients ---------------------------------------
    void WebEvents::envoieInfosBroadcast() {
        // debug_to_WSEvents(handleInfos(),F("infos")); // send data to all client
    }

// -------------------------------- send positions to all clients ------------------------------------------
    void WebEvents::envoiePositionsBroadcast() {
        // debug_to_WSEvents(handlePositionsPredefine(),F("infos")); // send data to all client
    }

// --------------------------------- send all informations to a new client ---------------------------------------
    void WebEvents::envoieAllInfosBroadcast() {
        // debug_to_WSEvents(handlePositionsPredefine(),F("infos")); // send data to all client

        // debug_to_WSEvents(handleInfos(),F("infos")); // send data to all client

        // debug_to_WSEvents(handleHardwareInfos(),F("infos")); // send data to all client

        // //debug_to_WSEvents(handlePinsInfos(),F("infos")); // send data to all client
    }
}