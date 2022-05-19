//
// Created by antbd on 05/06/2021.
//

#include "TBD_WiFi_Portail_Package.h"

namespace WiFi_Portail_API {
    Package::Package() : nameProject(F("")), versionProject(F("")), description(F("")), author(F("")), lastModification(F("")), license(F("")) {}

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
        String result;
        result += F("====== configProject ======");
        result += F("\n");
        result += F("Name:                ");
        result += this->getNameProject();
        result += F("\n");
        result += F("Version:             ");
        result += this->getVersionProject();
        result += F("\n");
        result += F("Description:         ");
        result += this->getDescription();
        result += F("\n");
        result += F("Author:              ");
        result += this->getAuthor();
        result += F("\n");
        result += F("Last modification:   ");
        result += this->getLastModification();
        result += F("\n");
        result += F("License:             ");
        result += this->getLicense();
        result += F("\n");
        result += F("===========================");
        return result;
    };
}