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

enum File_Type
{
    package,
    configAllWifi,
    configFTP,
    configOTA
};

class TBD_WiFi_Portail_FilesManager
{
public:
    TBD_WiFi_Portail_FilesManager(TBD_WiFi_Portail_SerialDebug &serialDebug, TBD_WiFi_Portail_FileSystem &fileSystem);
    ~TBD_WiFi_Portail_FilesManager();

    void add(TBD_WiFi_Portail_Wifi &allWifi);
    void add(TBD_WiFi_Portail_FTP &ftpSrvLogin);
    void add(TBD_WiFi_Portail_OTA &ota);
    void begin();

    void loadAllInformationsFromJsonFiles();
    void loadConfigFile(int indice);
    void loadConfigProject(const String &filename, TBD_WiFi_Portail_Package *package);
    void loadConfigAllWifi(const String &filename, WifiAll *allWifi);
    void loadConfigFTP(const String &filename, TBD_WiFi_Portail_FTP *ftpSrvLogin);
    void loadConfigOTA(const String &filename, TBD_WiFi_Portail_OTA *ota);

    void saveConfigAllWifi();

    void printListOfConfigFiles();

private:
    TBD_WiFi_Portail_SerialDebug *_serialDebug;
    TBD_WiFi_Portail_FileSystem *_fileSystem;
    TBD_WiFi_Portail_Package *_package;
    TBD_WiFi_Portail_Wifi *_allWifi;
    TBD_WiFi_Portail_FTP *_ftpSrvLogin;
    TBD_WiFi_Portail_OTA *_ota;

    std::map<int, String> _listOfConfigFiles;

    void readFile(const String &filename);
    void deleteFile(const String &path);
    bool saveJSonToAFile(DynamicJsonDocument *doc, const String &filename);
    JsonObject getJSonFromFile(DynamicJsonDocument *doc, const String &filename);
};
#endif //TBD_WIFI_PORTAIL_FILESMANAGER_H
