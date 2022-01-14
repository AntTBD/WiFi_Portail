//
// Created by antbd on 05/06/2021.
//

#include "TBD_WiFi_Portail_Package.h"

namespace WiFi_Portail_API {
    Package::Package() : nameProject(""), versionProject(""), description(""), author(""), lastModification(""), license("") {}

    Package::~Package() {}

    void Package::setNameProject(const String &nameProject) {
        this->nameProject = nameProject;
    }

    String Package::getNameProject() const {
        return this->nameProject;
    }

    void Package::setVersionProject(const String &versionProject) {
        this->versionProject = versionProject;
    }

    String Package::getVersionProject() const {
        return this->versionProject;
    }

    void Package::setDescription(const String &description) {
        this->description = description;
    }

    String Package::getDescription() const {
        return this->description;
    }

    void Package::setAuthor(const String &author) {
        this->author = author;
    }

    String Package::getAuthor() const {
        return this->author;
    }

    void Package::setLastModification(const String &lastModification) {
        this->lastModification = lastModification;
    }

    String Package::getLastModification() const {
        return this->lastModification;
    }

    void Package::setLicense(const String &license) {
        this->license = license;
    }

    String Package::getLicense() const {
        return this->license;
    }

    String Package::toString() const {
        return (String) F("====== configProject ======") + F("\n")
               + F("Name :                ") + this->getNameProject() + F("\n")
               + F("Version :             ") + this->getVersionProject() + F("\n")
               + F("Description :         ") + this->getDescription() + F("\n")
               + F("Author :              ") + this->getAuthor() + F("\n")
               + F("Last modification :   ") + this->getLastModification() + F("\n")
               + F("License :             ") + this->getLicense() + F("\n")
               + F("===========================");
    };
}