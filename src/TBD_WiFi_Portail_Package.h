//
// Created by antbd on 05/06/2021.
//

#ifndef TBD_WIFI_PORTAIL_PACKAGE_H
#define TBD_WIFI_PORTAIL_PACKAGE_H

#include <Arduino.h>
#include "TBD_WiFi_Portail.h"

class TBD_WiFi_Portail_Package
{
public:
    TBD_WiFi_Portail_Package();
    ~TBD_WiFi_Portail_Package();

    void setNameProject(const String &nameProject);
    String getNameProject() const;
    void setVersionProject(const String &versionProject);
    String getVersionProject() const;
    void setDescription(const String &description);
    String getDescription() const;
    void setAuthor(const String &author);
    String getAuthor() const;
    void setLastModification(const String &lastModification);
    String getLastModification() const;
    void setLicense(const String &license);
    String getLicense() const;

    String toString() const;

private:
    String nameProject;
    String versionProject;
    String description;
    String author;
    String lastModification;
    String license;
};

#endif //TBD_WIFI_PORTAIL_PACKAGE_H
