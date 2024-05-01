//
// Created by antbd on 05/06/2021.
//

#include "WiFi_Portail_FilesManager.h"

namespace WiFi_Portail_API {
    FilesManagerClass FilesManager;

    FilesManagerClass::FilesManagerClass() {
        this->_listOfConfigFiles = {
                {package,       F("/package.txt")},
                {configAllWifi, F("/configAllWifi.txt")},
                {configFTP,     F("/configFTP.txt")},
                {configOTA,     F("/configOTA.txt")},
                {configNTP,     F("/configNTP.txt")}
        };
        this->_package = new Package();
    }

    FilesManagerClass::~FilesManagerClass() {
        delete this->_package;
    }

    void FilesManagerClass::begin() {

        this->printListOfConfigFiles();

        this->loadAllInformationsFromJsonFiles();
    }

// ---------------------------------------------- Load ALL configFiles --------------------------------------------------
    void FilesManagerClass::loadAllInformationsFromJsonFiles() {
        SerialDebug_println(F("======= Load All Informations in FileSystem =========="));

        for (auto &item: this->_listOfConfigFiles) {
            this->loadConfigFile(item.first);
        }
        SerialDebug_println(F("======================================================"));
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
    void FilesManagerClass::loadConfigFile(int indice) {
        switch (indice) {
            case package:
                this->loadConfigProject(this->getFileName(package), this->_package);
                break;
            case configAllWifi:
                this->loadConfigAllWifi(this->getFileName(configAllWifi), WifiManager.wifiAll);
                break;
#ifdef USE_FTP
            case configFTP:
                this->loadConfigFTP(this->getFileName(configFTP));
                break;
#endif // USE_FTP
#ifdef USE_OTA
            case configOTA:
                this->loadConfigOTA(this->getFileName(configOTA));
                break;
#endif // USE_OTA
#ifdef USE_NTP
            case configNTP:
                this->loadConfigNTP(this->getFileName(configNTP));
                break;
#endif // USE_NTP
            default:
                SerialDebug_println(F("Config File function not found !"));
                break;
        }
    }

    String FilesManagerClass::getFileName(int indice) const {
        if (indice < this->_listOfConfigFiles.size()) {
            return this->_listOfConfigFiles.at(indice);
        }
        else {
            return F("");
        }
    }

// -------------------------------------------- Load config project ------------------------------------------------
    void FilesManagerClass::loadConfigProject(const String &filename, Package *package) {
        DynamicJsonDocument doc(JSON_OBJECT_SIZE(6) + 350);
        JsonObject obj = this->getJsonFromFile(&doc, filename);

        JsonVariant error = obj[F("version")];
        if (error.isNull()) {
            SerialDebug_println(F("JsonObject not corresponding => config not loaded"));
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

        SerialDebug_println(package->toString());
    }

// ---------------------------------------------- Load config Wifi -------------------------------------------------
    void FilesManagerClass::loadConfigAllWifi(const String &filename, WifiAll *wifiAll) {
        DynamicJsonDocument doc(
                JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(6) + JSON_ARRAY_SIZE(1) + 10 * (JSON_OBJECT_SIZE(8) + 25));
        JsonObjectConst obj = this->getJsonFromFile(&doc, filename);

        SerialDebug_println(F("/!\\ Be carefull : you can load 10 STA config max !"));

        JsonVariantConst error = obj[F("hostname")];
        if (error.isNull()) {
            SerialDebug_println(F("JsonObject not corresponding => config not loaded"));
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

        SerialDebug_println(wifiAll->toString());
    }

// ---------------------------------------------- Load config FTP --------------------------------------------------
#ifdef USE_FTP
    void FilesManagerClass::loadConfigFTP(const String &filename) {
        DynamicJsonDocument doc(JSON_OBJECT_SIZE(2) + 100);
        JsonObject obj = this->getJsonFromFile(&doc, filename);

        JsonVariant error = obj[F("user")];
        if (error.isNull()) {
            SerialDebug_println(F("JsonObject not corresponding => config not loaded"));
            return;
        }

        if (obj[F("user")])
            FTPManager.setUser(obj[F("user")].as<String>());
        if (obj[F("password")])
            FTPManager.setPassword(obj[F("password")].as<String>());

        SerialDebug_println(FTPManager.toString());
    }
#endif // USE_FTP

// ---------------------------------------------- Load config OTA --------------------------------------------------
#ifdef USE_OTA
    void FilesManagerClass::loadConfigOTA(const String &filename) {
        DynamicJsonDocument doc(JSON_OBJECT_SIZE(2) + 100);
        JsonObject obj = this->getJsonFromFile(&doc, filename);

        JsonVariant error = obj[F("hostname")];
        if (error.isNull()) {
            SerialDebug_println(F("JsonObject not corresponding => config not loaded"));
            return;
        }

        if (obj[F("hostname")])
            OTAManager.setHostname(obj[F("hostname")].as<String>());
        if (obj[F("password")])
            OTAManager.setPassword(obj[F("password")].as<String>());

        SerialDebug_println(OTAManager.toString());
    }
#endif // USE_OTA

#ifdef USE_NTP
    void FilesManagerClass::loadConfigNTP(const String &filename) {
        DynamicJsonDocument doc(JSON_OBJECT_SIZE(4) + 100);
        JsonObject obj = this->getJsonFromFile(&doc, filename);

        JsonVariant error = obj[F("ntpServer")];
        if (error.isNull()) {
            SerialDebug_println(F("JsonObject not corresponding => config not loaded"));
            return;
        }

        if (obj[F("ntpServer")])
            NTPManager.setNtpServerName(obj[F("ntpServer")].as<String>());
        if (obj[F("timeZone")])
            NTPManager.setTimeZone(obj[F("timeZone")].as<float>());
        if (obj[F("interval")])
            NTPManager.setInterval(obj[F("interval")].as<float>());
        if (obj[F("isSummerTime")])
            NTPManager.setDayLight(obj[F("isSummerTime")].as<bool>());

        SerialDebug_println(NTPManager.toString());
    }
#endif // USE_NTP

// -----------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------

// ---------------------------------------------- Save config NTP --------------------------------------------------
#ifdef USE_NTP
    bool FilesManagerClass::saveConfigNTP(DynamicJsonDocument *result) {
        return this->saveJsonToFile(result, this->getFileName(configNTP));
    }
#endif // USE_NTP
// -----------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------

// --------------------------------- Function to remove file from fileSystem -----------------------------------
    void FilesManagerClass::readFile(const String &filename) {
        SerialDebug_println(F("Open file in read mode"));
        // On verifie si le fichier a ete correctement cree
        File file = FileSystem.fileSystem->open(filename, "r");
        if (!file) {
            // File not found | le fichier de test n'existe pas
            SerialDebug_print(F("Failed to open '"));
            SerialDebug_print(filename);
            SerialDebug_println(F("' file!"));
            //        debug_to_WSEvents((String)F("Failed to open for read: ") + filename, eventType.error);
        } else {
            // Display file content on serial port | Envoie le contenu du fichier test sur le port série
            SerialDebug_print(F("Read '"));
            SerialDebug_print(filename);
            SerialDebug_println(F("' file content: "));
            while (file.available()) {
                SerialDebug_print((char) file.read());
            }
            SerialDebug_println(F("End of file"));
        }

        file.close();
    }

// --------------------------------- Function to remove file from fileSystem -----------------------------------
    void FilesManagerClass::deleteFile(const String &path) {
        SerialDebug_print(F("Deleting file: "));
        SerialDebug_println(path);
        if (FileSystem.fileSystem->remove(path)) {
            SerialDebug_println(F("File deleted"));
        } else {
            SerialDebug_println(F("Delete failed"));
        }
    }

// --------------------------------- Function to serialize in a file of filesystem -----------------------------------
    bool FilesManagerClass::saveJsonToFile(DynamicJsonDocument *doc, const String &filename) {
        // open the file. note that only one file can be open at a time,
        // so you have to close this one before opening another.
        SerialDebug_println(F("Open file in writing mode"));
        //    debug_to_WSEvents(F("Open file in writing mode"));
        File file = FileSystem.fileSystem->open(filename, "w");
        if (file) {
            SerialDebug_print(F("Filename --> "));
            SerialDebug_println(filename);
            SerialDebug_println(F("Start writing... :"));
            //        debug_to_WSEvents(F("Start writing..."));

            serializeJsonPretty(*doc, *SerialDebug.getSerial());
            serializeJsonPretty(*doc, file);
            SerialDebug_print(F("."));
            // close the file:
            file.close();
            SerialDebug_println(F("done."));
            //        debug_to_WSEvents(F("done."));
            return true;
        } else {
            file.close();
            // if the file didn't open, print an error:
            SerialDebug_print(F("Error opening "));
            SerialDebug_println(filename);
            //        debug_to_WSEvents((String)F("Error opening ") + filename, eventType.error);
            return false;
        }
    }

// -------------------------------- Function to deserialize file from filesystem ---------------------------------------
    JsonObject FilesManagerClass::getJsonFromFile(DynamicJsonDocument *doc, const String &filename) {
        // open the file for reading:
        File file = FileSystem.fileSystem->open(filename, "r");
        if (file) {
            DeserializationError error = deserializeJson(*doc, file);
            if (error) {
                // if the file didn't open, print an error:
                SerialDebug_print(F("Error deserialize JSON "));
                SerialDebug_println(error.f_str());
                //            debug_to_WSEvents(F("Error deserialize JSON :"), eventType.error);
                //            debug_to_WSEvents(error.f_str(), eventType.error);
            }
            // close the file:
            file.close();
            return doc->as<JsonObject>();
        } else {
            file.close();
            // if the file didn't open, print an error:
            SerialDebug_print(F("Error opening (or file not exists) : "));
            SerialDebug_println(filename);
            SerialDebug_println(F("Empty json created"));
            //        debug_to_WSEvents((String)F("Error opening (or file not exists) : ") + filename, eventType.error);
            //        debug_to_WSEvents(F("Empty json created"), eventType.error);
            return doc->as<JsonObject>();
        }
    }

    void FilesManagerClass::printListOfConfigFiles() {
        SerialDebug_print(F("List of config files ("));
        SerialDebug_print(this->_listOfConfigFiles.size());
        SerialDebug_println(F(") :"));
        SerialDebug_print(F("[ "));
        for (auto &item: this->_listOfConfigFiles) {
            SerialDebug_print(item.first);
            SerialDebug_print(F(":"));
            SerialDebug_print(item.second);
            SerialDebug_print(F(" "));
        }
        SerialDebug_println(F("]"));
    }
}