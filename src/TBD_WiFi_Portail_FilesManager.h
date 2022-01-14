//
// Created by antbd on 05/06/2021.
//

#ifndef TBD_WIFI_PORTAIL_FILESMANAGER_H
#define TBD_WIFI_PORTAIL_FILESMANAGER_H

#include <Arduino.h>
#include "TBD_WiFi_Portail.h"
#include "TBD_WiFi_Portail_SerialDebug.h"
#include "TBD_WiFi_Portail_FileSystem.h"
#include "TBD_WiFi_Portail_Wifi.h"
#include "TBD_WiFi_Portail_FTP.h"
#include "TBD_WiFi_Portail_Package.h"
#include "TBD_WiFi_Portail_OTA.h"

#include <ArduinoJson.h>
// JSON JavaScript Object Notation
// https://arduinojson.org/v6/doc/upgrade/ equivalence last part: Summary - Serialisation
// https://arduinojson.org/v6/assistant/
// path and name of the config file where all config are stored

#include <iostream>
#include <map>

namespace WiFi_Portail_API {

    enum File_Type {
        package,
        configAllWifi,
        configFTP,
        configOTA
    };

    class FilesManager {
    public:
        FilesManager(SerialDebug &serialDebug, FileSystem &fileSystem);

        ~FilesManager();

        void add(WifiManager &wifiManager);

        void add(FTP &ftpSrvLogin);

        void add(OTA &ota);

        void begin();

        void loadAllInformationsFromJsonFiles();

        void loadConfigFile(int indice);

        void loadConfigProject(const String &filename, Package *package);

        void loadConfigAllWifi(const String &filename, WifiAll *allWifi);

        void loadConfigFTP(const String &filename, FTP *ftpSrvLogin);

        void loadConfigOTA(const String &filename, OTA *ota);

        void saveConfigAllWifi();

        void printListOfConfigFiles();

    private:
        SerialDebug *_serialDebug;
        FileSystem *_fileSystem;
        Package *_package;
        WifiManager *_wifiManager;
        FTP *_ftpSrvLogin;
        OTA *_ota;

        std::map<int, String> _listOfConfigFiles;

        void readFile(const String &filename);

        void deleteFile(const String &path);

        bool saveJSonToAFile(DynamicJsonDocument *doc, const String &filename);

        JsonObject getJSonFromFile(DynamicJsonDocument *doc, const String &filename);
    };
}
#endif //TBD_WIFI_PORTAIL_FILESMANAGER_H
