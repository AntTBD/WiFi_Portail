//
// Created by antbd on 05/06/2021.
//

#include "TBD_WiFi_Portail_Package.h"

TBD_WiFi_Portail_Package::TBD_WiFi_Portail_Package() : nameProject(""), versionProject(""), description(""), author(""), lastModification(""), license("") {}

TBD_WiFi_Portail_Package::~TBD_WiFi_Portail_Package() {}

void TBD_WiFi_Portail_Package::setNameProject(const String& nameProject) {
    this->nameProject = nameProject;
}

String TBD_WiFi_Portail_Package::getNameProject() const {
    return this->nameProject;
}

void TBD_WiFi_Portail_Package::setVersionProject(const String& versionProject) {
    this->versionProject = versionProject;
}

String TBD_WiFi_Portail_Package::getVersionProject() const {
    return this->versionProject;
}

void TBD_WiFi_Portail_Package::setDescription(const String& description) {
    this->description = description;
}

String TBD_WiFi_Portail_Package::getDescription() const {
    return this->description;
}

void TBD_WiFi_Portail_Package::setAuthor(const String& author) {
    this->author = author;
}

String TBD_WiFi_Portail_Package::getAuthor() const {
    return this->author;
}

void TBD_WiFi_Portail_Package::setLastModification(const String& lastModification) {
    this->lastModification = lastModification;
}

String TBD_WiFi_Portail_Package::getLastModification() const {
    return this->lastModification;
}

void TBD_WiFi_Portail_Package::setLicense(const String& license) {
    this->license = license;
}

String TBD_WiFi_Portail_Package::getLicense() const {
    return this->license;
}

String TBD_WiFi_Portail_Package::toString() const {
    return (String)F("====== configProject ======") + F("\n")
           + F("Name :                ") + this->getNameProject()       + F("\n")
           + F("Version :             ") + this->getVersionProject()    + F("\n")
           + F("Description :         ") + this->getDescription()       + F("\n")
           + F("Author :              ") + this->getAuthor()            + F("\n")
           + F("Last modification :   ") + this->getLastModification()  + F("\n")
           + F("License :             ") + this->getLicense()           + F("\n")
           + F("===========================");
};