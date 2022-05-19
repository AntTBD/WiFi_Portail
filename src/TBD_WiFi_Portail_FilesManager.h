//
// Created by antbd on 05/06/2021.
//

#ifndef TBD_WIFI_PORTAIL_FILESMANAGER_H
#define TBD_WIFI_PORTAIL_FILESMANAGER_H

#include "TBD_WiFi_Portail.h"
#ifdef USE_FTP
#include "TBD_WiFi_Portail_FTP.h"
#endif // USE_FTP
#include "TBD_WiFi_Portail_Package.h"
#ifdef USE_OTA
#include "TBD_WiFi_Portail_OTA.h"
#endif // USE_OTA

#include <ArduinoJson.h>
// JSON JavaScript Object Notation
// https://arduinojson.org/v6/doc/upgrade/ equivalence last part: Summary - Serialisation
// https://arduinojson.org/v6/assistant/
// path and name of the config file where all config are stored

#include <iostream>
#include <map>

namespace WiFi_Portail_API {

    class FilesManagerClass {
    public:

        enum File_Type {
            package,
            configAllWifi,
            configFTP,
            configOTA
        };

        FilesManagerClass();

        ~FilesManagerClass();

        void begin();

        void loadAllInformationsFromJsonFiles();

        void loadConfigFile(int indice);

        void loadConfigProject(const String &filename, Package *package);

        void loadConfigAllWifi(const String &filename, WifiAll *allWifi);

#ifdef USE_FTP
        void loadConfigFTP(const String &filename);
#endif // USE_FTP

#ifdef USE_OTA
        void loadConfigOTA(const String &filename);
#endif // USE_OTA

        void saveConfigAllWifi();

        void printListOfConfigFiles();

    private:
        Package *_package;

        std::map<int, String> _listOfConfigFiles;

        void readFile(const String &filename);

        void deleteFile(const String &path);

        bool saveJSonToAFile(DynamicJsonDocument *doc, const String &filename);

        JsonObject getJSonFromFile(DynamicJsonDocument *doc, const String &filename);
    };

    extern FilesManagerClass FilesManager;
}
#endif //TBD_WIFI_PORTAIL_FILESMANAGER_H
