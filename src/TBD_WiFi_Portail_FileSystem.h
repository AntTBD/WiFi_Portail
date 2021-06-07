//
// Created by antbd on 30/05/2021.
//

#ifndef TBD_WIFI_PORTAIL_FILESYSTEM_H
#define TBD_WIFI_PORTAIL_FILESYSTEM_H

#include <Arduino.h>
#include "TBD_WiFi_Portail.h"
#include "TBD_WiFi_Portail_SerialDebug.h"


// CHANGE TO LittleFS https://byfeel.info/esp8266-systeme-de-fichier-littlefs/

// https://projetsdiy.fr/esp8266-lire-ecrire-modifier-fichier-librairie-littlefs/
// It is possible to create subfolders without exceeding the limit of 31 useful characters for the file path including the file name and its extension.


// Select the FileSystem by uncommenting one of the lines below
//#define USE_SPIFFS
#define USE_LITTLEFS
//#define USE_SDFS

#if defined USE_SPIFFS
    #include <FS.h>
//    FS *fileSystem = &SPIFFS;
//    SPIFFSConfig fileSystemConfig = SPIFFSConfig();
    #define FileFS SPIFFS
    #define FS_Name "SPIFFS"
#elif defined USE_LITTLEFS
    #include <FS.h>
    #include <LittleFS.h>
//    FS *fileSystem = &LittleFS;
//    LittleFSConfig fileSystemConfig = LittleFSConfig();
    #define SPIFFS LittleFS
    #define FileFS LittleFS
    #define FS_Name "LittleFS"
#elif defined USE_SDFS
    #include <SDFS.h>
//    FS *fileSystem = &SDFS;
//    SDFSConfig fileSystemConfig = SDFSConfig();
    // fileSystemConfig.setCSPin(chipSelectPin);
#else
  #error Please select a filesystem first by uncommenting one of the "#define USE_xxx" lines at the beginning of the sketch.
#endif

class TBD_WiFi_Portail_FileSystem {
public :
    TBD_WiFi_Portail_FileSystem(TBD_WiFi_Portail_SerialDebug& serialDebug, bool resetFileSystemAtStartup = false);
    ~TBD_WiFi_Portail_FileSystem();
    bool begin();

    uint16_t getListOfAllFilesInMemory();
    uint16_t listDir(String indent, String path);
    void infos_FileSytem();
    void filesytem_end();
    String formatBytes(size_t bytes);


#if defined USE_SPIFFS
    FS *fileSystem;
    SPIFFSConfig _fileSystemConfig;
#elif defined USE_LITTLEFS
    FS *fileSystem;
    LittleFSConfig _fileSystemConfig;
#elif defined USE_SDFS
    FS *fileSystem;
    SDFSConfig _fileSystemConfig;
#endif

private:
    bool _resetFileSystemAtStartup;
    TBD_WiFi_Portail_SerialDebug* _serialDebug;


};


#endif //TBD_WIFI_PORTAIL_FILESYSTEM_H
