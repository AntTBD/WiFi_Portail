//
// Created by antbd on 07/10/2021.
//

#ifndef SERVICE_H
#define SERVICE_H

#include <Arduino.h>

#include <ArduinoJson.h>

class Service {
public:
    ~Service(){};

    virtual DynamicJsonDocument toJson() { };
    virtual DynamicJsonDocument toJson2() { };

    virtual DynamicJsonDocument toObj() { };

    virtual String toString() { return F("**Service.toString()**"); };
    virtual String toString2() { return F("**Service.toString2()**"); };

};


#endif //SERVICE_H
