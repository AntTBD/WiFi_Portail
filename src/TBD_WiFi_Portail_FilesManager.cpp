//
// Created by antbd on 05/06/2021.
//

#include "TBD_WiFi_Portail_FilesManager.h"

namespace WiFi_Portail_API {


    FilesManager::FilesManager(SerialDebug &serialDebug, FileSystem &fileSystem) :
        _serialDebug(&serialDebug), _fileSystem(&fileSystem){
        this->_listOfConfigFiles = {{package,       "/package.txt"},
                                    {configAllWifi, "/configAllWifi.txt"},
                                    {configFTP,     "/configFTP.txt"},
                                    {configOTA,     "/configOTA.txt"}};
        this->_package = new Package();
        this->_wifiManager = nullptr;
        this->_ftpSrvLogin = nullptr;
        this->_ota = nullptr;
    }

    FilesManager::~FilesManager() {
        delete this->_serialDebug;
        delete this->_fileSystem;
        delete this->_package;
        delete this->_wifiManager;
#ifdef USE_FTP
        delete this->_ftpSrvLogin;
#endif // USE_FTP
#ifdef USE_OTA
        delete this->_ota;
#endif // USE_OTA
    }

    void FilesManager::add(WifiManager &wifiManager) {
        this->_wifiManager = &wifiManager;
    }

    void WiFi_Portail_API::FilesManager::add(FTP &ftpSrvLogin) {
        this->_ftpSrvLogin = &ftpSrvLogin;
    }

    void FilesManager::add(OTA &ota) {
        this->_ota = &ota;
    }

    void FilesManager::begin() {

        this->printListOfConfigFiles();

        this->loadAllInformationsFromJsonFiles();
    }

// ---------------------------------------------- Load ALL configFiles --------------------------------------------------
    void FilesManager::loadAllInformationsFromJsonFiles() {
        this->_serialDebug->println(F("======= Load All Informations in FileSystem =========="));

        for (auto &item: this->_listOfConfigFiles) {
            this->loadConfigFile(item.first);
        }
        this->_serialDebug->println(F("======================================================"));
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
    void FilesManager::loadConfigFile(int indice) {
        switch (indice) {
            case package:
                this->loadConfigProject(this->_listOfConfigFiles.at(package), this->_package);
                break;
            case configAllWifi:
                if (this->_wifiManager != nullptr) {
                    this->loadConfigAllWifi(this->_listOfConfigFiles.at(configAllWifi), this->_wifiManager->wifiAll);
                }
                break;
            case configFTP:
                if (this->_ftpSrvLogin != nullptr) {
                    this->loadConfigFTP(this->_listOfConfigFiles.at(configFTP), this->_ftpSrvLogin);
                }
                break;
            case configOTA:
                if (this->_ota != nullptr) {
                    this->loadConfigOTA(this->_listOfConfigFiles.at(configOTA), this->_ota);
                }
                break;
            default:
                this->_serialDebug->println(F("Config File function not found !"));
                break;
        }
    }

// -------------------------------------------- Load config project ------------------------------------------------
    void FilesManager::loadConfigProject(const String &filename, Package *package) {
        DynamicJsonDocument doc(JSON_OBJECT_SIZE(6) + 350);
        JsonObject obj = this->getJSonFromFile(&doc, filename);

        JsonVariant error = obj[F("version")];
        if (error.isNull()) {
            this->_serialDebug->println(F("JsonObject not corresponding => config not loaded"));
            return;
        }

        if (obj[F("name")])
            package->setNameProject(obj[F("name")].as<const char *>());
        if (obj[F("version")])
            package->setVersionProject(obj[F("version")].as<const char *>());
        if (obj[F("description")])
            package->setDescription(obj[F("description")].as<const char *>());
        if (obj[F("author")])
            package->setAuthor(obj[F("author")].as<const char *>());
        if (obj[F("lastModification")])
            package->setLastModification(obj[F("lastModification")].as<const char *>());
        if (obj[F("license")])
            package->setLicense(obj[F("license")].as<const char *>());

        this->_serialDebug->println(package->toString());
    }

// ---------------------------------------------- Load config Wifi -------------------------------------------------
    void FilesManager::loadConfigAllWifi(const String &filename, WifiAll *wifiAll) {
        DynamicJsonDocument doc(
                JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(6) + JSON_ARRAY_SIZE(1) + 10 * (JSON_OBJECT_SIZE(8) + 25));
        JsonObjectConst obj = this->getJSonFromFile(&doc, filename);

        this->_serialDebug->println(F("/!\\ Be carefull : you can load 10 STA config max !"));

        JsonVariantConst error = obj[F("hostname")];
        if (error.isNull()) {
            this->_serialDebug->println(F("JsonObject not corresponding => config not loaded"));
            return;
        }

        if (obj[F("hostname")])
            wifiAll->setHostname(obj[F("hostname")].as<String>()); //+ "__" + (String)ESP.getFlashChipId()
        if (obj[F("mdnsName")])
            wifiAll->setMdnsName(obj[F("mdnsName")].as<String>());
        if (obj[F("mode")])
            wifiAll->setMode(obj[F("mode")].as<WiFiMode_t>());
        if (obj[F("resetWifi")])
            wifiAll->setResetWifi(obj[F("resetWifi")].as<bool>());

        if (obj[F("AP")]) {
            JsonObjectConst oneAP = doc[F("AP")];
            AP *apTemp = new AP();
            if (oneAP[F("ssid")])
                apTemp->setSSID(oneAP[F("ssid")].as<String>());
            if (oneAP[F("password")])
                apTemp->setPassword(oneAP[F("password")].as<String>());
            if (oneAP[F("hide")])
                apTemp->setHide(oneAP[F("hide")].as<bool>());
            if (oneAP[F("ip")])
                apTemp->setIp(oneAP[F("ip")].as<String>());
            if (oneAP[F("mask")])
                apTemp->setMask(oneAP[F("mask")].as<String>());
            if (oneAP[F("autoDisable")])
                apTemp->setAutoDisable(oneAP[F("autoDisable")].as<int>());
            wifiAll->setAP(*apTemp);
            delete apTemp;
        }

        wifiAll->resetSTA();
        if (obj[F("allSTA")]) {
            JsonArrayConst arrayAllSTA = obj[F("allSTA")]; //.to<JsonArray>();
            for (JsonObjectConst oneSTA: arrayAllSTA) {
                STA *staTemp = new STA();
                if (oneSTA[F("ssid")])
                    staTemp->setSSID(oneSTA[F("ssid")].as<String>());
                if (oneSTA[F("password")])
                    staTemp->setPassword(oneSTA[F("password")].as<String>());
                if (oneSTA[F("useDHCP")])
                    staTemp->setUseDHCP(oneSTA[F("useDHCP")].as<bool>());
                if (oneSTA[F("ip")])
                    staTemp->setIp(oneSTA[F("ip")].as<String>());
                if (oneSTA[F("mask")])
                    staTemp->setMask(oneSTA[F("mask")].as<String>());
                if (oneSTA[F("DNSServer")])
                    staTemp->setDNSServer(oneSTA[F("DNSServer")].as<String>());
                if (oneSTA[F("gateway")])
                    staTemp->setGateway(oneSTA[F("gateway")].as<String>());
                if (oneSTA[F("autoDisable")])
                    staTemp->setAutoDisable(oneSTA[F("autoDisable")].as<int>());
                wifiAll->addSTA(*staTemp);
                delete staTemp;
            }
        }

        this->_serialDebug->println(wifiAll->toString());
    }

// ---------------------------------------------- Load config FTP --------------------------------------------------
    void FilesManager::loadConfigFTP(const String &filename, FTP *ftpSrvLogin) {
#ifdef USE_FTP
        if (ftpSrvLogin != nullptr) {
            DynamicJsonDocument doc(JSON_OBJECT_SIZE(2) + 100);
            JsonObject obj = this->getJSonFromFile(&doc, filename);

            JsonVariant error = obj[F("user")];
            if (error.isNull()) {
                this->_serialDebug->println(F("JsonObject not corresponding => config not loaded"));
                return;
            }

            if (obj[F("user")])
                ftpSrvLogin->setUser(obj[F("user")].as<String>());
            if (obj[F("password")])
                ftpSrvLogin->setPassword(obj[F("password")].as<String>());

            this->_serialDebug->println(ftpSrvLogin->toString());
        }
#endif // USE_FTP
    }

// ---------------------------------------------- Load config OTA --------------------------------------------------
    void FilesManager::loadConfigOTA(const String &filename, OTA *ota) {
#ifdef USE_OTA
        if (ota != nullptr) {
            DynamicJsonDocument doc(JSON_OBJECT_SIZE(2) + 100);
            JsonObject obj = this->getJSonFromFile(&doc, filename);

            JsonVariant error = obj[F("hostname")];
            if (error.isNull()) {
                this->_serialDebug->println(F("JsonObject not corresponding => config not loaded"));
                return;
            }

            if (obj[F("hostname")])
                ota->setHostname(obj[F("hostname")].as<String>());
            if (obj[F("password")])
                ota->setPassword(obj[F("password")].as<String>());

            this->_serialDebug->println(ota->toString());
        }
#endif // USE_OTA
    }

// -----------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------

// --------------------------------- Function to remove file from fileSystem -----------------------------------
    void FilesManager::readFile(const String &filename) {
        this->_serialDebug->println(F("Open file in read mode"));
        // On verifie si le fichier a ete correctement cree
        File file = this->_fileSystem->fileSystem->open(filename, "r");
        if (!file) {
            // File not found | le fichier de test n'existe pas
            this->_serialDebug->print(F("Failed to open '"));
            this->_serialDebug->print(filename);
            this->_serialDebug->println(F("' file!"));
            //        debug_to_WSEvents((String)F("Failed to open for read: ") + filename, eventType.error);
        } else {
            // Display file content on serial port | Envoie le contenu du fichier test sur le port série
            this->_serialDebug->print(F("Read '"));
            this->_serialDebug->print(filename);
            this->_serialDebug->println(F("' file content: "));
            while (file.available()) {
                this->_serialDebug->print((char) file.read());
            }
            this->_serialDebug->println(F("End of file"));
        }

        file.close();
    }

// --------------------------------- Function to remove file from fileSystem -----------------------------------
    void FilesManager::deleteFile(const String &path) {
        this->_serialDebug->print(F("Deleting file: "));
        this->_serialDebug->println(path);
        if (this->_fileSystem->fileSystem->remove(path)) {
            this->_serialDebug->println(F("File deleted"));
        } else {
            this->_serialDebug->println(F("Delete failed"));
        }
    }

// --------------------------------- Function to serialize in a file of filesystem -----------------------------------
    bool FilesManager::saveJSonToAFile(DynamicJsonDocument *doc, const String &filename) {
        // open the file. note that only one file can be open at a time,
        // so you have to close this one before opening another.
        this->_serialDebug->println(F("Open file in writing mode"));
        //    debug_to_WSEvents(F("Open file in writing mode"));
        File file = this->_fileSystem->fileSystem->open(filename, "w");
        if (file) {
            this->_serialDebug->print(F("Filename --> "));
            this->_serialDebug->println(filename);
            this->_serialDebug->println(F("Start writing... :"));
            //        debug_to_WSEvents(F("Start writing..."));

            serializeJsonPretty(*doc, *this->_serialDebug->getSerial());
            serializeJsonPretty(*doc, file);
            this->_serialDebug->print(F("."));
            // close the file:
            file.close();
            this->_serialDebug->println(F("done."));
            //        debug_to_WSEvents(F("done."));
            return true;
        } else {
            file.close();
            // if the file didn't open, print an error:
            this->_serialDebug->print(F("Error opening "));
            this->_serialDebug->println(filename);
            //        debug_to_WSEvents((String)F("Error opening ") + filename, eventType.error);
            return false;
        }
    }

// -------------------------------- Function to deserialize file from filesystem ---------------------------------------
    JsonObject FilesManager::getJSonFromFile(DynamicJsonDocument *doc, const String &filename) {
        // open the file for reading:
        File file = this->_fileSystem->fileSystem->open(filename, "r");
        if (file) {
            DeserializationError error = deserializeJson(*doc, file);
            if (error) {
                // if the file didn't open, print an error:
                this->_serialDebug->print(F("Error deserialize JSON "));
                this->_serialDebug->println(error.f_str());
                //            debug_to_WSEvents(F("Error deserialize JSON :"), eventType.error);
                //            debug_to_WSEvents(error.f_str(), eventType.error);
            }
            // close the file:
            file.close();
            return doc->as<JsonObject>();
        } else {
            file.close();
            // if the file didn't open, print an error:
            this->_serialDebug->print(F("Error opening (or file not exists) : "));
            this->_serialDebug->println(filename);
            this->_serialDebug->println(F("Empty json created"));
            //        debug_to_WSEvents((String)F("Error opening (or file not exists) : ") + filename, eventType.error);
            //        debug_to_WSEvents(F("Empty json created"), eventType.error);
            return doc->as<JsonObject>();
        }
    }

    void FilesManager::printListOfConfigFiles() {
        this->_serialDebug->print(F("List of config files ("));
        this->_serialDebug->print(this->_listOfConfigFiles.size());
        this->_serialDebug->println(F(") :"));
        this->_serialDebug->print(F("[ "));
        for (auto &item: this->_listOfConfigFiles) {
            this->_serialDebug->print(item.first);
            this->_serialDebug->print(F(":"));
            this->_serialDebug->print(item.second);
            this->_serialDebug->print(F(" "));
        }
        this->_serialDebug->println(F("]"));
    }
}