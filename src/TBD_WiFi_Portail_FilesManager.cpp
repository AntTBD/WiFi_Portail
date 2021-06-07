//
// Created by antbd on 05/06/2021.
//

#include "TBD_WiFi_Portail_FilesManager.h"


TBD_WiFi_Portail_FilesManager::TBD_WiFi_Portail_FilesManager(TBD_WiFi_Portail_SerialDebug& serialDebug, TBD_WiFi_Portail_FileSystem& fileSystem): _serialDebug(&serialDebug), _fileSystem(&fileSystem){
    this->_listOfConfigFiles = {{package,       "/package.txt"},
                                {configAllWifi, "/configAllWifi.txt"},
                                {configFTP,     "/configFTP.txt"}
    };
    this->_package = new TBD_WiFi_Portail_Package();
    this->_allWifi = nullptr;
    this->_ftpSrvLogin = nullptr;
}
TBD_WiFi_Portail_FilesManager::~TBD_WiFi_Portail_FilesManager() {}

void TBD_WiFi_Portail_FilesManager::add(TBD_WiFi_Portail_Wifi& allWifi) {
    this->_allWifi = &allWifi;
}
void TBD_WiFi_Portail_FilesManager::add(TBD_WiFi_Portail_FTP& ftpSrvLogin) {
    this->_ftpSrvLogin = &ftpSrvLogin;
}

void TBD_WiFi_Portail_FilesManager::begin() {

    this->printListOfConfigFiles();

    this->loadAllInformationsFromJsonFiles();
}

// ---------------------------------------------- Load ALL configFiles --------------------------------------------------
void TBD_WiFi_Portail_FilesManager::loadAllInformationsFromJsonFiles() {
    this->_serialDebug->println(F("======= Load All Informations in FileSystem =========="));

    for (auto &item : this->_listOfConfigFiles) {
        this->loadConfigFile(item.first);
    }
    this->_serialDebug->println(F("======================================================"));
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void TBD_WiFi_Portail_FilesManager::loadConfigFile(int indice) {
    switch (indice) {
        case package:
            this->loadConfigProject(this->_listOfConfigFiles.at(package), this->_package);
            break;
        case configAllWifi:
            if(this->_allWifi!= nullptr) {
                this->loadConfigAllWifi(this->_listOfConfigFiles.at(configAllWifi), this->_allWifi->wifiAll);
            }
            break;
        case configFTP:
            if(this->_ftpSrvLogin!= nullptr) {
                this->loadConfigFTP(this->_listOfConfigFiles.at(configFTP), this->_ftpSrvLogin);
            }
            break;
        default:
            this->_serialDebug->println(F("Config File function not found !"));
            break;
    }
}

// -------------------------------------------- Load config project ------------------------------------------------
void TBD_WiFi_Portail_FilesManager::loadConfigProject(String filename, TBD_WiFi_Portail_Package* package) {
    DynamicJsonDocument doc(JSON_OBJECT_SIZE(6) + 350);
    JsonObject obj = this->getJSonFromFile(&doc, filename);

    JsonVariant error = obj[F("version")];
    if (error.isNull())
    {
        this->_serialDebug->println(F("JsonObject not corresponding => config not loaded"));
        return;
    }

    if(obj[F("name")])              package->nameProject      = obj[F("name")].as<const char*>();
    if(obj[F("version")])           package->versionProject   = obj[F("version")].as<const char*>();
    if(obj[F("description")])       package->description      = obj[F("description")].as<const char*>();
    if(obj[F("author")])            package->author           = obj[F("author")].as<const char*>();
    if(obj[F("lastModification")])  package->lastModification = obj[F("lastModification")].as<const char*>();
    if(obj[F("license")])           package->license          = obj[F("license")].as<const char*>();

    this->_serialDebug->println(package->toString());
}

// ---------------------------------------------- Load config Wifi -------------------------------------------------
void TBD_WiFi_Portail_FilesManager::loadConfigAllWifi(String filename, WifiAll& wifiAll) {
    DynamicJsonDocument doc(JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(6) + 10 * (JSON_OBJECT_SIZE(8) + 2000));
    JsonObject obj = this->getJSonFromFile(&doc, filename);

    JsonVariant error = obj[F("hostname")];
    if (error.isNull())
    {
        this->_serialDebug->println(F("JsonObject not corresponding => config not loaded"));
        return;
    }

    if(obj[F("hostname")])         wifiAll.setHostname(obj[F("hostname")].as<String>());//+ "__" + (String)ESP.getFlashChipId()
    if(obj[F("mdnsName")])         wifiAll.setMdnsName(obj[F("mdnsName")].as<String>());
    if(obj[F("mode")])             wifiAll.setMode(obj[F("mode")].as<WiFiMode_t>());
    if(obj[F("resetWifi")])        wifiAll.setResetWifi(obj[F("resetWifi")].as<bool>());

    if(obj[F("AP")]){
        JsonObject oneAP = doc[F("AP")];
        AP apTemp = AP();
        if(oneAP[F("ssid")])        apTemp.setSSID(oneAP[F("ssid")].as<String>());
        if(oneAP[F("password")])    apTemp.setPassword(oneAP[F("password")].as<String>());
        if(oneAP[F("hide")])        apTemp.setHide(oneAP[F("hide")].as<bool>());
        if(oneAP[F("ip")])          apTemp.setIp(oneAP[F("ip")].as<String>());
        if(oneAP[F("mask")])        apTemp.setMask(oneAP[F("mask")].as<String>());
        if(oneAP[F("autoDisable")]) apTemp.setAutoDisable(oneAP[F("autoDisable")].as<int>());
        wifiAll.setAP(apTemp);
    }

    wifiAll.allSTA.reset();
    if(obj[F("allSTA")]){
        JsonArray arrayAllSTA = obj[F("allSTA")];//.to<JsonArray>();
        for(JsonObject oneSTA : arrayAllSTA) {
            STA staTemp = STA();
            if(oneSTA[F("ssid")])        staTemp.setSSID(oneSTA[F("ssid")].as<String>());
            if(oneSTA[F("password")])    staTemp.setPassword(oneSTA[F("password")].as<String>());
            if(oneSTA[F("useDHCP")])     staTemp.setUseDHCP(oneSTA[F("useDHCP")].as<bool>());
            if(oneSTA[F("ip")])          staTemp.setIp(oneSTA[F("ip")].as<String>());
            if(oneSTA[F("mask")])        staTemp.setMask(oneSTA[F("mask")].as<String>());
            if(oneSTA[F("DNSServer")])   staTemp.setDNSServer(oneSTA[F("DNSServer")].as<String>());
            if(oneSTA[F("gateway")])     staTemp.setGateway(oneSTA[F("gateway")].as<String>());
            if(oneSTA[F("autoDisable")]) staTemp.setAutoDisable(oneSTA[F("autoDisable")].as<int>());
            wifiAll.allSTA.add(staTemp);
        }
    }

    this->_serialDebug->println(wifiAll.toString());
}

// ---------------------------------------------- Load config FTP --------------------------------------------------
void TBD_WiFi_Portail_FilesManager::loadConfigFTP(String filename, TBD_WiFi_Portail_FTP* ftpSrvLogin) {
    DynamicJsonDocument doc(JSON_OBJECT_SIZE(2) + 100);
    JsonObject obj = this->getJSonFromFile(&doc, filename);

    JsonVariant error = obj[F("user")];
    if (error.isNull())
    {
        this->_serialDebug->println(F("JsonObject not corresponding => config not loaded"));
        return;
    }

    if(obj[F("user")])      ftpSrvLogin->setUser(obj[F("user")].as<String>());
    if(obj[F("password")])  ftpSrvLogin->setPassword(obj[F("password")].as<String>());

    this->_serialDebug->println(ftpSrvLogin->toString());
}

// -----------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------

// --------------------------------- Function to remove file from fileSystem -----------------------------------
void TBD_WiFi_Portail_FilesManager::readFile(String filename)
{
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
            this->_serialDebug->print((char)file.read());
        }
        this->_serialDebug->println(F("End of file"));
    }

    file.close();
}

// --------------------------------- Function to remove file from fileSystem -----------------------------------
void TBD_WiFi_Portail_FilesManager::deleteFile(String path)
{
    this->_serialDebug->print(F("Deleting file: "));
    this->_serialDebug->println(path);
    if (this->_fileSystem->fileSystem->remove(path))
    {
        this->_serialDebug->println(F("File deleted"));
    }
    else
    {
        this->_serialDebug->println(F("Delete failed"));
    }
}

// --------------------------------- Function to serialize in a file of filesystem -----------------------------------
bool TBD_WiFi_Portail_FilesManager::saveJSonToAFile(DynamicJsonDocument *doc, String filename)
{
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    this->_serialDebug->println(F("Open file in writing mode"));
//    debug_to_WSEvents(F("Open file in writing mode"));
    File file = this->_fileSystem->fileSystem->open(filename, "w");
    if (file)
    {
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
    }
    else
    {
        file.close();
        // if the file didn't open, print an error:
        this->_serialDebug->print(F("Error opening "));
        this->_serialDebug->println(filename);
//        debug_to_WSEvents((String)F("Error opening ") + filename, eventType.error);
        return false;
    }
}

// -------------------------------- Function to deserialize file from filesystem ---------------------------------------
JsonObject TBD_WiFi_Portail_FilesManager::getJSonFromFile(DynamicJsonDocument *doc, String filename)
{
    // open the file for reading:
    File file = this->_fileSystem->fileSystem->open(filename, "r");
    if (file)
    {
        DeserializationError error = deserializeJson(*doc, file);
        if (error)
        {
            // if the file didn't open, print an error:
            this->_serialDebug->print(F("Error deserialize JSON "));
            this->_serialDebug->println(error.f_str());
//            debug_to_WSEvents(F("Error deserialize JSON :"), eventType.error);
//            debug_to_WSEvents(error.f_str(), eventType.error);

        }
        // close the file:
        file.close();
        return doc->as<JsonObject>();
    }
    else
    {
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


void TBD_WiFi_Portail_FilesManager::printListOfConfigFiles()
{
    this->_serialDebug->print(F("List of config files ("));
    this->_serialDebug->print(this->_listOfConfigFiles.size());
    this->_serialDebug->println(F(") :"));
    this->_serialDebug->print(F("[ "));
    for (auto &item : this->_listOfConfigFiles) {
        this->_serialDebug->print(item.first);
        this->_serialDebug->print(F(":"));
        this->_serialDebug->print(item.second);
        this->_serialDebug->print(F(" "));
    }
    this->_serialDebug->println(F("]"));
}