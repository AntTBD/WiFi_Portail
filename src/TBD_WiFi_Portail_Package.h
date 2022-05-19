//
// Created by antbd on 05/06/2021.
//

#ifndef TBD_WIFI_PORTAIL_PACKAGE_H
#define TBD_WIFI_PORTAIL_PACKAGE_H

#include "TBD_WiFi_Portail.h"
#include "Incopiable.h"

namespace WiFi_Portail_API {
    class Package : Singleton<Package> {
    public:
        Package();

        ~Package();

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
        // sans ceci, le parent ne peut instancier son enfant
        friend class Singleton<Package>;

        String nameProject;
        String versionProject;
        String description;
        String author;
        String lastModification;
        String license;
    };
}
#endif //TBD_WIFI_PORTAIL_PACKAGE_H
