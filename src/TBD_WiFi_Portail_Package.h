//
// Created by antbd on 05/06/2021.
//

#ifndef TBD_WIFI_PORTAIL_PACKAGE_H
#define TBD_WIFI_PORTAIL_PACKAGE_H

#include <Arduino.h>
#include "TBD_WiFi_Portail.h"

class TBD_WiFi_Portail_Package{
public:
    TBD_WiFi_Portail_Package() : nameProject(""), versionProject(""), description(""), author(""), lastModification(""), license("") {}
    ~TBD_WiFi_Portail_Package() {}

    String nameProject;
    String versionProject;
    String description;
    String author;
    String lastModification;
    String license;

    String toString(){
        return (String)F("====== configProject ======") + F("\n")
               + F("Name :                ") + String(this->nameProject)       + F("\n")
               + F("Version :             ") + String(this->versionProject)    + F("\n")
               + F("Description :         ") + String(this->description)       + F("\n")
               + F("Author :              ") + String(this->author)            + F("\n")
               + F("Last modification :   ") + String(this->lastModification)  + F("\n")
               + F("License :             ") + String(this->license)           + F("\n")
               + F("===========================");
    };
};

#endif //TBD_WIFI_PORTAIL_PACKAGE_H
