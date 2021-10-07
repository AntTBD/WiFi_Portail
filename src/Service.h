//
// Created by antbd on 07/10/2021.
//

#ifndef SERVICE_H
#define SERVICE_H

#include "TBD_WiFi_Portail_SerialDebug.h"
#include <ArduinoJson.h>

class Service {
protected:
    TBD_WiFi_Portail_SerialDebug *_serialDebug;

public:
    Service() { this->_serialDebug = nullptr; };

    Service(TBD_WiFi_Portail_SerialDebug &serialDebug) { this->_serialDebug = &serialDebug; };

    ~Service() { delete this->_serialDebug; };

    void begin();

    void loop();

    DynamicJsonDocument toJson() { };
    DynamicJsonDocument toJson2() { };

    DynamicJsonDocument toObj() { };

    String toString() { };

    void updateValues();
};


#endif //SERVICE_H
