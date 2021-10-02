//
// Created by antbd on 05/06/2021.
//

#include "TBD_WiFi_Portail_WebEvents.h"


TBD_WiFi_Portail_WebEvents::TBD_WiFi_Portail_WebEvents(TBD_WiFi_Portail_SerialDebug& serialDebug, TBD_WiFi_Portail_WebServer& webServer, const String& eventsRoot, const String& eventsAdminRoot):
    _serialDebug(&serialDebug),
    _webServer(&webServer),
    _eventsRoot(eventsRoot),
    _eventsAdminRoot(eventsAdminRoot) {

    this->events = new AsyncEventSource(this->_eventsRoot);
    this->eventsAdmin = new AsyncEventSource(this->_eventsAdminRoot);
    this->_adminUser = "userAdmin";
    this->_adminPass = "passAdmin";
    this->eventType = new sEventType();
}

TBD_WiFi_Portail_WebEvents::~TBD_WiFi_Portail_WebEvents(){
    delete this->_serialDebug;
    delete this->_webServer;
    delete this->eventType;

    delete this->events;
    delete this->eventsAdmin;

}

void TBD_WiFi_Portail_WebEvents::setEventsAdmin_UserPass(const String& user, const String& password)
{
    this->_adminUser = user;
    this->_adminPass = password;
}


void TBD_WiFi_Portail_WebEvents::begin() // Start a WebEvents server
{
    this->_serialDebug->println(F("== Setup WebEvents =="));

    this->events->onConnect([&](AsyncEventSourceClient *client){
        if(client->lastId()){
            this->_serialDebug->print(F("[Events] Client reconnected! Last message ID that it get is: "));
            this->_serialDebug->println((String)client->lastId());
        }
        //send event with message "hello!", id current millis
        // and set reconnect delay to 1 second
        client->send("Hello from IKEA PS 2014!", NULL, millis(), 500);//1000
        debug_to_WSEvents((String)F("New client connected ")+(String)client->lastId(), this->eventType->debug);
    });
    this->_webServer->server->addHandler(this->events);
    this->_serialDebug->printf(F(" -> Events server      started. ( %s )\n"), this->_eventsRoot.c_str());

    this->eventsAdmin->onConnect([&](AsyncEventSourceClient *client){
        if(client->lastId()){
            this->_serialDebug->print(F("[EventsAdmin] Admin client reconnected! Last message ID that it get is: "));
            this->_serialDebug->println((String)client->lastId());
        }
        //send event with message "hello!", id current millis
        // and set reconnect delay to 1 second
        client->send("Hello from IKEA PS 2014! (Admin)", NULL, millis(), 500);//1000
        debug_to_WSEvents((String)F("New Admin client connected ")+(String)client->lastId(), this->eventType->debug);
    });
    //HTTP Authenticate before switch to Websocket protocol
    //eventsAdmin->setAuthentication("userAdmin", "passAdmin");
    this->_webServer->server->addHandler(this->eventsAdmin);
    this->_serialDebug->printf(F(" -> EventsAdmin server started. ( %s ) user: %s / pass: %s \n"), this->_eventsAdminRoot.c_str(), this->_adminUser.c_str(), this->_adminPass.c_str());

    this->_serialDebug->println(F("====================="));
}

// --------------------------------- send messsage to debug via websocket events ---------------------------------------
void TBD_WiFi_Portail_WebEvents::debug_to_WSEvents(const String& message, const String& type, int niveau){
    this->_serialDebug->print(F("debug_to_WSEvents : "));
    this->_serialDebug->println(message);
    if(type == this->eventType->info) {
        this->events->send(message.c_str(), type.c_str(), millis()); //send data to all clients
    } else {
        this->eventsAdmin->send(message.c_str(), type.c_str(), millis()); //send data to all admin clients
    }
}


// --------------------------------- send informations to all clients ---------------------------------------
void TBD_WiFi_Portail_WebEvents::envoieInfosBroadcast()
{
    // debug_to_WSEvents(handleInfos(),F("infos")); // send data to all client
}

// -------------------------------- send positions to all clients ------------------------------------------
void TBD_WiFi_Portail_WebEvents::envoiePositionsBroadcast()
{
    // debug_to_WSEvents(handlePositionsPredefine(),F("infos")); // send data to all client
}

// --------------------------------- send all informations to a new client ---------------------------------------
void TBD_WiFi_Portail_WebEvents::envoieAllInfosBroadcast()
{
    // debug_to_WSEvents(handlePositionsPredefine(),F("infos")); // send data to all client

    // debug_to_WSEvents(handleInfos(),F("infos")); // send data to all client

    // debug_to_WSEvents(handleHardwareInfos(),F("infos")); // send data to all client

    // //debug_to_WSEvents(handlePinsInfos(),F("infos")); // send data to all client
}