//
// Created by antbd on 30/05/2021.
//

#include "WiFi_Portail_FileSystem.h"

namespace WiFi_Portail_API {
    FileSystemClass FileSystem;


// ---------------------------------- Start file system ------------------------------------------
    FileSystemClass::FileSystemClass() {

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

        this->listOfFiles = std::vector<_Directory *>();
    }

    FileSystemClass::~FileSystemClass() {
        delete this->fileSystem;
        //delete this->_fileSystemConfig;

        this->listOfFiles.clear();
    }

    bool FileSystemClass::begin(bool resetFileSystemAtStartup) {
        SerialDebug_println(F("== Setup File Sytem =="));
        this->_resetFileSystemAtStartup = resetFileSystemAtStartup;

        this->fileSystem->setConfig(this->_fileSystemConfig);
        bool fsOK = this->fileSystem->begin();
        SerialDebug_println(fsOK ? F("File system initialized.") : F(
                "Error file System:    File system init failed! (Failed to mount file system)"));

        if (!fsOK) {
            while (true) {
                SerialDebug_print(F("."));
                delay(1000);
            }
        }

        bool formatOK = false;
        if (this->_resetFileSystemAtStartup) {
            SerialDebug_println(F("\nFormatting FILE SYSTEM !\nThose files will be deleted:"));
            uint16_t nbrOfDir = this->getListOfAllFilesInMemory();

            SerialDebug_println(F("\nStarting formatting FILE SYSTEM"));
            uint32_t startTime = millis();

            formatOK = this->fileSystem->format(); // formatting of FILE SYSTEM memory
            if (formatOK) {
                SerialDebug_print(F("Formatting completed, it took "));
                SerialDebug_print(millis() - startTime);
                SerialDebug_println(F(" ms!"));
            } else {
                SerialDebug_println(F("!!! Error during formatting !!!"));
            }
        }

        SerialDebug_println(F("======================"));

        this->infos_FileSytem(); // print informations of the memory

        return (fsOK && formatOK);
    }

// --------------------------------------- List of files in memory -----------------------------------
    uint16_t FileSystemClass::getListOfAllFilesInMemory() {
        SerialDebug_println(F("List of files in memory:"));

        _Directory *newDir = new _Directory();
        newDir->name = F("/");
        this->listOfFiles.push_back(newDir);

        uint16_t fileCount = this->listDir(F("  "), F("/"), this->listOfFiles.at(0));
        SerialDebug_print(fileCount);
        SerialDebug_println(F(" files or folders in total"));

        return fileCount;
    }

// --------------------------------------- liste des fichiers et autres dossier dans un dossier -----------------------------------
// It is possible to create subfolders without exceeding the limit of 31 useful characters for the file path including the file name and its extension.
    uint16_t FileSystemClass::listDir(const String &indent, const String &path, _Directory *newDir) {
        uint16_t dirCount = 0;
        Dir dir = this->fileSystem->openDir(path);

        while (dir.next()) {
            ++dirCount;
            if (dir.isDirectory()) {
                // add dir in list
                auto *newDir2 = new _Directory();
                newDir2->name = dir.fileName().c_str();

                SerialDebug_print(indent.c_str());
                SerialDebug_print(F("+ "));
                SerialDebug_print(dir.fileName().c_str());
                SerialDebug_println(F(" [Dir]"));
                dirCount += this->listDir(indent + F("   "), path + dir.fileName() + F("/"), newDir2);

                newDir->childrenDirectories.push_back(newDir2);
            } else {
                // add file to the list
                auto *newFile = new _File();
                newFile->name = dir.fileName().c_str();
                newFile->size = dir.fileSize();

                SerialDebug_print(indent.c_str());
                SerialDebug_print(F("  "));
                SerialDebug_printf(F("%-16s"), dir.fileName().c_str());
                SerialDebug_print(F(" ("));
                //SerialDebug_printf("%ld",(uint32_t)dir.fileSize());
                //SerialDebug_println(F(" Bytes)"));
                SerialDebug_print(Utils.formatBytes(dir.fileSize()));
                SerialDebug_println(F(")"));

                newDir->childrenFiles.push_back(newFile);
            }
        }
        return dirCount;
    }

// ----------------------------------------- serialised list of directories/files ----------------------------
    String FileSystemClass::getFilesListJson() const {
        String result = F("{\"list\":");
        result += F("[");
        int i = 0;
        for (_Directory *dir: this->listOfFiles) {
            i++;
            result += dir->serialized();
            if (i != this->listOfFiles.size()) {
                result += F(",");
            }
        }
        result += F("]}");
        return result;
    }

// ----------------------------------------- info on FileSytem memory for DEBUG ----------------------------
    void FileSystemClass::infos_FileSytem() {
        SerialDebug_println();
        SerialDebug_println(F("==========================================="));
        SerialDebug_println(F("       FileSytem ESP8266 INFORMATIONS"));
        SerialDebug_println(F("==========================================="));

        FSInfo fsInfo;
        this->fileSystem->info(fsInfo);             // info fs
        float total = (fsInfo.totalBytes / 1024.0); // fsInfo.totalBytes
        SerialDebug_print(F("info totalbytes :    "));
        //  SerialDebug_print(total);
        //  SerialDebug_println(F(" ko"));
        SerialDebug_println(Utils.formatBytes(total));
        SerialDebug_print(F("info usedbytes :     "));
        //  SerialDebug_print((float)(fsInfo.usedBytes/1024.0));
        //  SerialDebug_println(F(" ko"));
        SerialDebug_println(Utils.formatBytes(fsInfo.usedBytes));
        SerialDebug_print(F("info blocksize :     "));
        SerialDebug_println(fsInfo.blockSize);
        SerialDebug_print(F("info pagesize:       "));
        SerialDebug_println(fsInfo.pageSize);
        SerialDebug_print(F("info maxOpenFiles :  "));
        SerialDebug_println(fsInfo.maxOpenFiles);
        SerialDebug_print(F("info maxPathLength : "));
        SerialDebug_print(fsInfo.maxPathLength);
        SerialDebug_println(F(" characters"));

        SerialDebug_println();

        uint16_t nbrOfDir = this->getListOfAllFilesInMemory(); // get list of files in memory

        SerialDebug_println(F("\n===========================================\n"));

        if (nbrOfDir == 0) {
            SerialDebug_println(F("No files in FileSystem !!!"));
            SerialDebug_println(F("Waiting..."));
            for (;;) {
                SerialDebug_print(F("."));
                delay(1000);
            }
        }
    }

// ----------------------------- close filesystem ------------------------------------------------
    void FileSystemClass::filesytem_end() {
        //debug_to_WSEvents(F("End of fileSystem"), eventType.warn);//anto
        this->fileSystem->end();
    }


}