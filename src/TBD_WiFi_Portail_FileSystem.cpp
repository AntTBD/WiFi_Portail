//
// Created by antbd on 30/05/2021.
//

#include "TBD_WiFi_Portail_FileSystem.h"

// ---------------------------------- Start file system ------------------------------------------
TBD_WiFi_Portail_FileSystem::TBD_WiFi_Portail_FileSystem(TBD_WiFi_Portail_SerialDebug& serialDebug, bool resetFileSystemAtStartup) : _resetFileSystemAtStartup(resetFileSystemAtStartup), _serialDebug(&serialDebug) {

    #if defined USE_SPIFFS
        this->fileSystem = &SPIFFS;
        this->_fileSystemConfig = SPIFFSConfig();
    #elif defined USE_LITTLEFS
        this->fileSystem = &LittleFS;
        this->_fileSystemConfig = LittleFSConfig();
    #elif defined USE_SDFS
        this->fileSystem = &SDFS;
        this->_fileSystemConfig = SDFSConfig();
    #endif
}

TBD_WiFi_Portail_FileSystem::~TBD_WiFi_Portail_FileSystem() {}

bool TBD_WiFi_Portail_FileSystem::begin() {
    this->_serialDebug->println(F("== Setup File Sytem =="));


    this->fileSystem->setConfig(this->_fileSystemConfig);
    bool fsOK = this->fileSystem->begin();
    this->_serialDebug->println(fsOK ? F("File system initialized.") : F("Error file System:    File system init failed! (Failed to mount file system)"));

    if (!fsOK)
    {
        while(true){
            this->_serialDebug->print(F("."));
            delay(1000);
        }
    }

    bool formatOK;
    if(this->_resetFileSystemAtStartup){
        this->_serialDebug->println(F("\nFormatting FILE SYSTEM !\nThose files will be deleted:"));
        uint16_t nbrOfDir = this->getListOfAllFilesInMemory();

        this->_serialDebug->println(F("\nStarting formatting FILE SYSTEM"));
        uint32_t startTime = millis();

        formatOK = this->fileSystem->format();// formatting of FILE SYSTEM memory
        if (formatOK)
        {
            this->_serialDebug->print(F("Formatting completed, it took "));
            this->_serialDebug->print(millis() - startTime);
            this->_serialDebug->println(F(" ms!"));
        }
        else
        {
            this->_serialDebug->println(F("!!! Error during formatting !!!"));
        }
    }

    this->_serialDebug->println(F("======================"));

    this->infos_FileSytem(); // print informations of the memory

    return (fsOK && formatOK);
}



// --------------------------------------- List of files in memory -----------------------------------
uint16_t TBD_WiFi_Portail_FileSystem::getListOfAllFilesInMemory()
{
    this->_serialDebug->println(F("List of files in memory:"));
    uint16_t fileCount = this->listDir(F("  "), F("/"));
    this->_serialDebug->print(fileCount);
    this->_serialDebug->println(F(" files or folders in total"));

    return fileCount;
}

// --------------------------------------- liste des fichiers et autres dossier dans un dossier -----------------------------------
// It is possible to create subfolders without exceeding the limit of 31 useful characters for the file path including the file name and its extension.
uint16_t TBD_WiFi_Portail_FileSystem::listDir(String indent, String path)
{
    uint16_t dirCount = 0;
    Dir dir = this->fileSystem->openDir(path);
    while (dir.next())
    {
        ++dirCount;
        if (dir.isDirectory())
        {
            this->_serialDebug->print(indent.c_str());
            this->_serialDebug->print(F("+ "));
            this->_serialDebug->print(dir.fileName().c_str());
            this->_serialDebug->println(F(" [Dir]"));
            dirCount += this->listDir(indent + F("   "), path + dir.fileName() + F("/"));
        } else {
            this->_serialDebug->print(indent.c_str());
            this->_serialDebug->print(F("  "));
            this->_serialDebug->printf(F("%-16s"),dir.fileName().c_str());
            this->_serialDebug->print(F(" ("));
            //this->_serialDebug->printf("%ld",(uint32_t)dir.fileSize());
            //this->_serialDebug->println(F(" Bytes)"));
            this->_serialDebug->print(this->formatBytes(dir.fileSize()));
            this->_serialDebug->println(F(")"));
        }
    }
    return dirCount;
}

// ----------------------------------------- info on FileSytem memory for DEBUG ----------------------------
void TBD_WiFi_Portail_FileSystem::infos_FileSytem()
{
    this->_serialDebug->println();
    this->_serialDebug->println(F("==========================================="));
    this->_serialDebug->println(F("       FileSytem ESP8266 INFORMATIONS"));
    this->_serialDebug->println(F("==========================================="));

    FSInfo fsInfo;
    this->fileSystem->info(fsInfo);                 // info fs
    float total = (fsInfo.totalBytes / 1024.0); // fsInfo.totalBytes
    this->_serialDebug->print(F("info totalbytes :    "));
//  this->_serialDebug->print(total);
//  this->_serialDebug->println(F(" ko"));
    this->_serialDebug->println(this->formatBytes(total));
    this->_serialDebug->print(F("info usedbytes :     "));
//  this->_serialDebug->print((float)(fsInfo.usedBytes/1024.0));
//  this->_serialDebug->println(F(" ko"));
    this->_serialDebug->println(this->formatBytes(fsInfo.usedBytes));
    this->_serialDebug->print(F("info blocksize :     "));
    this->_serialDebug->println(fsInfo.blockSize);
    this->_serialDebug->print(F("info pagesize:       "));
    this->_serialDebug->println(fsInfo.pageSize);
    this->_serialDebug->print(F("info maxOpenFiles :  "));
    this->_serialDebug->println(fsInfo.maxOpenFiles);
    this->_serialDebug->print(F("info maxPathLength : "));
    this->_serialDebug->print(fsInfo.maxPathLength);
    this->_serialDebug->println(F(" characters"));

    this->_serialDebug->println();

    uint16_t nbrOfDir = this->getListOfAllFilesInMemory(); // get list of files in memory

    this->_serialDebug->println(F("\n===========================================\n"));

    if (nbrOfDir == 0) {
        this->_serialDebug->println(F("No files in FileSystem !!!"));
        this->_serialDebug->println(F("Waiting..."));
        while (true) {
            this->_serialDebug->print(F("."));
            delay(1000);
        }
    }
}

// ----------------------------- close filesystem ------------------------------------------------
void TBD_WiFi_Portail_FileSystem::filesytem_end()
{
    //debug_to_WSEvents(F("End of fileSystem"), eventType.warn);//anto
    this->fileSystem->end();
}



// ----------------------- format bytes ----------------------------------------------------
String TBD_WiFi_Portail_FileSystem::formatBytes(size_t bytes)
{
    if (bytes < 1024)
    {
        return String(bytes) + F(" B");
    }
    else if (bytes < (1024 * 1024))
    {
        return String(bytes / 1024.0) + F(" KB");
    }
    else if (bytes < (1024 * 1024 * 1024))
    {
        return String(bytes / 1024.0 / 1024.0) + F(" MB");
    }
    else
    {
        return String(bytes / 1024.0 / 1024.0 / 1024.0) + F(" GB");
    }
}