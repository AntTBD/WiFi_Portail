//
// Created by antbd on 16/06/2021.
//

#include "TBD_WiFi_Portail_ESP.h"

/*var message = {
        "softVersion": "V0.2",
        "statusInfos": {
            "device":{
                "hardware": {"name":"Hardware","value":"hardware."},
                "chipId": {"name":"Chip ID","value":"chipId."},
                "freeHeap": {"name":"Free Heap","progress":{"value":"10000","max":"40000","unity":"Bytes"}},
                "heapFragmentation": {"name":"Heap Fragmentation","value":"heapFragmentation."},
                "sketchHash": {"name":"Sketch Hash","value":"sketchHash."},
                "hardware2": {"name":"Hardware","value":"hardware."},
                "chipId2": {"name":"Chip ID","value":"chipId."},
                "freeHeap2": {"name":"Free Heap","value":"freeHeap."},
                "heapFragmentation2": {"name":"Heap Fragmentation","value":"heapFragmentation."},
                "sketchHash2": {"name":"Sketch Hash","value":"sketchHash."},
            },
            "network":{
                "macAddress": {"name":"MAC Address","value":"macAddress."},
                "wifiMode": {"name":"Connection Type","value":1},
                "hardware3": {"name":"Hardware","value":"hardware."},
                "chipId3": {"name":"Chip ID","value":"chipId."},
                "freeHeap3": {"name":"Free Heap","value":"freeHeap."},
                "heapFragmentation3": {"name":"Heap Fragmentation","value":"heapFragmentation."},
                "sketchHash3": {"name":"Sketch Hash","value":"sketchHash."},
            }
        }
};*/

/// Constructor
TBD_WiFi_Portail_ESP::TBD_WiFi_Portail_ESP():Service() {
    /*this->_infosName = {
            "esphead",
            "uptime",
            "chipid",
            "fchipid",
            "idesize",
            "realsize",
            "flashspeed",
            "flashmode",// ///////
            "sdkver",
            "corever",
            "bootver",
            "cpufreq",
            "freeheap",
            "memsketch",
            "allmems",
            "lastreset",
            "wifihead",
            "wifimode",
            "apip",
            "apmac",
            "apssid",
            "apbssid",
            "staip",
            "stagw",
            "stasub",
            "dnss",
            "host",
            "stamac",
            "conx",
            "autoconx"
    };*/
    this->_infosName = new AFArray<String>();
    this->_infosName->add("esphead");
    this->_infosName->add("uptime");
    this->_infosName->add("chipid");
    this->_infosName->add("fchipid");
    this->_infosName->add("idesize");
    this->_infosName->add("realsize");
    this->_infosName->add("flashspeed");
    this->_infosName->add("flashmode");// ///////
    this->_infosName->add("sdkver");
    this->_infosName->add("corever");
    this->_infosName->add("bootver");
    this->_infosName->add("cpufreq");
    this->_infosName->add("freeheap");
    this->_infosName->add("memsketch");
    this->_infosName->add("allmems");
    this->_infosName->add("lastreset");
    this->_infosName->add("wifihead");
    this->_infosName->add("wifimode");
    this->_infosName->add("apip");
    this->_infosName->add("apmac");
    this->_infosName->add("ssid");
    this->_infosName->add("bssid");
    this->_infosName->add("host");
    this->_infosName->add("staip");
    this->_infosName->add("stamac");
    this->_infosName->add("stasub");
    this->_infosName->add("stagw");
    this->_infosName->add("dnss");
    this->_infosName->add("conx");
    this->_infosName->add("autoconx");

    this->_deviceStatus = new AFArray<String>();
    this->_deviceStatus->add("uptime");
    this->_deviceStatus->add("chipid");
    this->_deviceStatus->add("fchipid");
    this->_deviceStatus->add("idesize");
    this->_deviceStatus->add("realsize");
    this->_deviceStatus->add("flashspeed");
    this->_deviceStatus->add("flashmode");// ///////
    this->_deviceStatus->add("sdkver");
    this->_deviceStatus->add("corever");
    this->_deviceStatus->add("bootver");
    this->_deviceStatus->add("cpufreq");
    this->_deviceStatus->add("freeheap");
    this->_deviceStatus->add("sketch");
    this->_deviceStatus->add("filesystem");
    this->_deviceStatus->add("lastreset");

    this->_networkStatus = new AFArray<String>();
    this->_networkStatus->add("wifimode");
    this->_networkStatus->add("apip");
    this->_networkStatus->add("apmac");
    this->_networkStatus->add("ssid");
    this->_networkStatus->add("bssid");
    this->_networkStatus->add("host");
    this->_networkStatus->add("staip");
    this->_networkStatus->add("stamac");
    this->_networkStatus->add("stasub");
    this->_networkStatus->add("stagw");
    this->_networkStatus->add("dnss");
    this->_networkStatus->add("conx");
    this->_networkStatus->add("autoconx");

    this->_ntp = nullptr;
    this->_fileSystem = nullptr;
    //this->_webSocket = nullptr;
}

/// Destructor
TBD_WiFi_Portail_ESP::~TBD_WiFi_Portail_ESP(){
#ifdef USE_NTP
    delete this->_ntp;
#endif // USE_NTP
    delete this->_fileSystem;
    //delete this->_webSocket;

    delete this->_infosName;
    delete this->_deviceStatus;
    delete this->_networkStatus;
}

/// Add NTP class
void TBD_WiFi_Portail_ESP::addNTP(TBD_WiFi_Portail_NTP& ntp){
    this->_ntp = &ntp;
}

/// Add FileSystem class
void TBD_WiFi_Portail_ESP::addFileSystem(TBD_WiFi_Portail_FileSystem& fileSystem){
    this->_fileSystem = &fileSystem;
}

/// Add WebSocket class
/// Never used
/*void TBD_WiFi_Portail_ESP::addWebSocket(TBD_WiFi_Portail_WebSocket& webSocket){
    this->_webSocket = &webSocket;
}*/

/// Never used
/*// -------------------------------- Fonction pour avoir toutes les informations hardware de l'esp8266 -------------------------------------------
DynamicJsonDocument TBD_WiFi_Portail_ESP::getHardwareInfosJson() {
    DynamicJsonDocument docHardwareInfos(
            JSON_OBJECT_SIZE(1) + this->_infosName.size() * JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(this->_infosName.size()) + 2000);
    docHardwareInfos["resultof"] = "getStatus";
    JsonObject hardwareInfos = docHardwareInfos.createNestedObject(F("value"));

    String infosData[2];
    while (this->_infosName.has_next()) {
        String name = this->_infosName.next();
        JsonObject hardwareInfos_datas = hardwareInfos.createNestedObject(name);
        this->getInfoData(name, infosData);

        hardwareInfos_datas[F("name")] = infosData[0];
        hardwareInfos_datas[F("value")] = infosData[1];
    }
    return docHardwareInfos;


}

/// Never used
String TBD_WiFi_Portail_ESP::getHardwareInfosString() {
    String jsonStringHardwareInfos;
    jsonStringHardwareInfos.reserve(2000);
    serializeJson(this->getHardwareInfosJson(), jsonStringHardwareInfos); // on remplie la string avec les infos de JSONInfo sous forme sérialisé
    //Serial.println(jsonStringHardwareInfos);
    return jsonStringHardwareInfos;
}

/// Never used
void TBD_WiFi_Portail_ESP::sendHardwareInfosByWebSocket(){
    if(this->_webSocket != nullptr) {
        DynamicJsonDocument docHardwareInfos(
                JSON_OBJECT_SIZE(1) + this->_infosName.size() * JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(this->_infosName.size()) + 2000);
        docHardwareInfos["resultof"] = "getStatus";
        JsonObject hardwareInfos = docHardwareInfos.createNestedObject(F("value"));

        String infosData[2];
        while (this->_infosName.has_next()) {
            String name = this->_infosName.next();
            JsonObject hardwareInfos_datas = hardwareInfos.createNestedObject(name);
            this->getInfoData(name, infosData);

            hardwareInfos_datas[F("name")] = infosData[0];
            hardwareInfos_datas[F("value")] = infosData[1];
        }
        this->_webSocket->sendJsonByWebsocket(&docHardwareInfos);
    }
}*/
/// Convert all infos to JSON sorted by devices datas and network datas
DynamicJsonDocument TBD_WiFi_Portail_ESP::getHardwareInfosJson2() {
    DynamicJsonDocument docHardwareInfos(
            JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(2)
            + JSON_ARRAY_SIZE((this->_deviceStatus->size()+1)) + (this->_deviceStatus->size()+1)*JSON_OBJECT_SIZE(4)
            + JSON_ARRAY_SIZE((this->_networkStatus->size()+1)) + (this->_networkStatus->size()+1)*JSON_OBJECT_SIZE(4)
            );
    docHardwareInfos[F("resultof")] = F("getStatus");
    JsonObject hardwareInfos = docHardwareInfos.createNestedObject(F("value"));
    hardwareInfos[F("softVersion")] = WIFI_PORTAIL_VERSION;
    JsonObject statusInfos = hardwareInfos.createNestedObject(F("statusInfos"));
    JsonArray deviceInfos = statusInfos.createNestedArray(F("device"));
    while (this->_deviceStatus->has_next()) {
        String name = this->_deviceStatus->next();
        sNameValue info = this->getInfoData2(name);
        deviceInfos.add(serialized(info.ToSting()));
    }
    JsonArray networkInfos = statusInfos.createNestedArray(F("network"));
    while (this->_networkStatus->has_next()) {
        String name = this->_networkStatus->next();
        sNameValue info = this->getInfoData2(name);
        networkInfos.add(serialized(info.ToSting()));
    }

    return docHardwareInfos;
}

/// Convert all infos to JSON and then to string
String TBD_WiFi_Portail_ESP::getHardwareInfosString2() {
    String jsonStringHardwareInfos;
    jsonStringHardwareInfos.reserve(2000);
    serializeJsonPretty(this->getHardwareInfosJson2(), jsonStringHardwareInfos); // on remplie la string avec les infos de JSONInfo sous forme sérialisé
    //Serial.println(jsonStringHardwareInfos);
    return jsonStringHardwareInfos;
}

/// Return infos by string name
sNameValue TBD_WiFi_Portail_ESP::getInfoData2(const String& id) {
    sNameValue info;

    if (id == F("uptime")) {
        info.name = F("Uptime");
#ifdef USE_NTP
        if (this->_ntp != nullptr) info.value = this->_ntp->getUptimeString();
        else {
#endif // USE_NTP
            unsigned long millisecs = millis();
            info.value = (String)((millisecs / (1000 * 60 * 60 * 24)) % 365);
            info.value += F(" Days ");
            info.value += (String)((millisecs / (1000 * 60 * 60)) % 24);
            info.value += F(" Hrs ");
            info.value += (String)((millisecs / (1000 * 60)) % 60);
            info.value += F(" Mins ");
            info.value += (String)((millisecs / 1000) % 60);
            info.value += F(" Secs");
#ifdef USE_NTP
        }
#endif // USE_NTP
    } else if (id == F("chipid")) {
        info.name = F("Chip ID");
        info.value = F("0x");
        info.value += String(ESP.getChipId(), HEX);
    } else if (id == F("fchipid")) {
        info.name = F("Flash Chip ID");
        info.value = (String) ESP.getFlashChipId();
    } else if (id == F("idesize")) {
        info.name = F("Flash IDE  Size");
        if (this->_fileSystem != nullptr) info.value = this->_fileSystem->formatBytes(ESP.getFlashChipSize());
        else info.value = (String) ESP.getFlashChipSize();
    } else if (id == F("realsize")) {
        info.name = F("Flash Real Size");
        if (this->_fileSystem != nullptr) info.value = this->_fileSystem->formatBytes(ESP.getFlashChipRealSize());
        else info.value = (String) ESP.getFlashChipRealSize();
    } else if (id == F("flashspeed")) {
        info.name = F("Flash Speed");
        info.value = (String)(float(ESP.getFlashChipSpeed()) / 1000000);
        info.value += F(" MHz");
    } else if (id == F("flashmode")) {
        info.name = F("Flash Mode");
        FlashMode_t ideMode = ESP.getFlashChipMode();
        info.value = (ideMode == FM_QIO ? "QIO"
                : ideMode == FM_QOUT ? "QOUT"
                : ideMode == FM_DIO ? "DIO"
                : ideMode == FM_DOUT ? "DOUT"
                : "UNKNOWN");
    } else if (id == F("sdkver")) {
        info.name = F("SDK Version");
        info.value = (String) ESP.getSdkVersion();
    } else if (id == F("corever")) {
        info.name = F("Core Version");
        info.value = ESP.getCoreVersion();
        info.value.replace('_', '.');
    } else if (id == F("bootver")) {
        info.name = F("Boot Version");
        info.value = (String) ESP.getBootVersion();
    } else if (id == F("cpufreq")) {
        info.name = F("CPU Frequency");
        info.value = (String) ESP.getCpuFreqMHz();
        info.value += F(" MHz");
    } else if (id == F("freeheap")) {
        info.name = F("Memory - Free Heap (RAM)");
        info.value = (String) ESP.getFreeHeap();
        info.value += F(" bytes available <br>(0% is clean, more than ~50% is not harmless)");
        info.progress.value = (float) (ESP.getHeapFragmentation());
        info.progress.min = 0;
        info.progress.max = 100;
        info.progress.unity = "%";
        //F("(0% is clean, more than ~50% is not harmless)");
    } else if (id == F("sketch")) {
        info.name = F("Memory - Sketch Size");
        if (this->_fileSystem != nullptr) info.value = this->_fileSystem->formatBytes(ESP.getSketchSize());
        else info.value = (String) ESP.getSketchSize();
        info.value += F(" / ");
        if (this->_fileSystem != nullptr)
            info.value += this->_fileSystem->formatBytes(ESP.getSketchSize() + ESP.getFreeSketchSpace());
        else info.value += (String)(ESP.getSketchSize() + ESP.getFreeSketchSpace());
        info.value += F(" (Used / Total)");
        info.progress.value = (float) (ESP.getSketchSize());
        info.progress.min = 0;
        info.progress.max = (float) (ESP.getSketchSize() + ESP.getFreeSketchSpace());
        info.progress.unity = "bytes";
    } else if (id == F("filesystem")) {
        //|--------------|-------|---------------|--|--|--|--|--|
        //^              ^       ^               ^     ^
        //Sketch    OTA update   File system   EEPROM  WiFi config (SDK)

        info.name = F("Memory - FileSystem Size");
        FSInfo fsInfo;
        if (this->_fileSystem != nullptr) this->_fileSystem->fileSystem->info(fsInfo);
        info.value += (String)(fsInfo.usedBytes);
        info.value += F(" / ");
        info.value += (String)(fsInfo.totalBytes);
        info.value += F(" (Used / Total bytes)");
        info.progress.value = (float) (fsInfo.usedBytes);
        info.progress.min = 0;
        info.progress.max = (float) (fsInfo.totalBytes);;
        info.progress.unity = "bytes";
    } else if (id == F("lastreset")) {
        info.name = F("Last reset reason");
        info.value = (String) ESP.getResetReason();
    }
    //---------------------------------------------
    //---------------------------------------------
    //---------------------------------------------
    else if (id == F("wifimode")) {
        info.name = F("Wifi Mode");
        //WIFI_OFF = 0, WIFI_STA = 1, WIFI_AP = 2, WIFI_AP_STA = 3,
        int wifi_mode = WiFi.getMode();
        switch (wifi_mode) {
            case 0: // WIFI_OFF
                info.value = F("WIFI_OFF");
                break;
            case 1: // WIFI_STA
                info.value = F("WIFI_STA");
                break;
            case 2: // WIFI_AP
                info.value = F("WIFI_AP");
                break;
            case 3: // WIFI_AP_STA
                info.value = F("WIFI_AP_STA");
                break;
        }
    } else if (id == F("apip")) {
        info.name = F("Access Point IP");
        info.value = WiFi.softAPIP().toString();
    } else if (id == F("apmac")) {
        info.name = F("Access Point MAC");
        info.value = (String) WiFi.softAPmacAddress();
    } else if (id == F("ssid")) {
        info.name = F("SSID");
        info.value = (String) WiFi.SSID();
    } else if (id == F("bssid")) {
        info.name = F("BSSID");
        info.value = (String) WiFi.BSSIDstr();
    } else if (id == F("staip")) {
        info.name = F("Station IP");
        info.value = WiFi.localIP().toString();
    } else if (id == F("stagw")) {
        info.name = F("Station Gateway");
        info.value = WiFi.gatewayIP().toString();
    } else if (id == F("stasub")) {
        info.name = F("Station Subnet");
        info.value = WiFi.subnetMask().toString();
    } else if (id == F("dnss")) {
        info.name = F("DNS Server");
        info.value = WiFi.dnsIP().toString();
    } else if (id == F("host")) {
        info.name = F("Hostname");
        info.value = WiFi.hostname();
    } else if (id == F("stamac")) {
        info.name = F("Station MAC");
        info.value = WiFi.macAddress();
    } else if (id == F("conx")) {
        info.name = F("Connected");
        info.value = WiFi.isConnected() ? F("Yes") : F("No");
    } else if (id == F("autoconx")) {
        info.name = F("Autoconnect");
        info.value = WiFi.getAutoConnect() ? F("Enabled") : F("Disabled");
    } else {
        info.name = F("Error");
        info.value = F("Error");
    }

    return info;
}
/*
void TBD_WiFi_Portail_ESP::getInfoData(const String& id, String (&tab)[2]) {
    String nom;
    nom.reserve(50);
    String value;
    value.reserve(250);

    if (id == F("esphead")) {
        nom = F("Infos ESP8266");
        value = F("");
    } else if (id == F("wifihead")) {
        nom = F("Wifi");
        value = F("");
    } else if (id == F("uptime")) {
        nom = F("Uptime");
        if(this->_ntp != nullptr) value = this->_ntp->getUptimeString();
        else
        {
            unsigned long millisecs = millis();
            value = (String)((millisecs / (1000 * 60 * 60 * 24)) % 365);
            value += F(" Days ");
            value += (String)((millisecs / (1000 * 60 * 60)) % 24);
            value += F(" Hrs ");
            value += (String)((millisecs / (1000 * 60)) % 60);
            value += F(" Mins ");
            value += (String)((millisecs / 1000) % 60);
            value += F(" Secs");
        }
    } else if (id == F("chipid")) {
        nom = F("Chip ID");
        value = F("0x");
        value += String(ESP.getChipId(), HEX);
    } else if (id == F("fchipid")) {
        nom = F("Flash Chip ID");
        value = (String) ESP.getFlashChipId();
    } else if (id == F("idesize")) {
        nom = F("Flash IDE  Size");
        if(this->_fileSystem != nullptr) value = this->_fileSystem->formatBytes(ESP.getFlashChipSize());
        else value = (String)ESP.getFlashChipSize();
    } else if (id == F("realsize")) {
        nom = F("Flash Real Size");
        if(this->_fileSystem != nullptr) value = this->_fileSystem->formatBytes(ESP.getFlashChipRealSize());
        else value = (String)ESP.getFlashChipRealSize();
    } else if (id == F("flashspeed")) {
        nom = F("Flash Speed");
        value = (String)(float(ESP.getFlashChipSpeed()) / 1000000);
        value += F(" MHz");
    } else if (id == F("flashmode")) {
        nom = F("Flash Mode");
        FlashMode_t ideMode = ESP.getFlashChipMode();
        value = (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN");
    } else if (id == F("sdkver")) {
        nom = F("SDK Version");
        value = (String) ESP.getSdkVersion();
    } else if (id == F("corever")) {
        nom = F("Core Version");
        value = ESP.getCoreVersion();
        value.replace('_', '.');
    } else if (id == F("bootver")) {
        nom = F("Boot Version");
        value = (String) ESP.getBootVersion();
    } else if (id == F("cpufreq")) {
        nom = F("CPU Frequency");
        value = (String) ESP.getCpuFreqMHz();
        value += F(" MHz");
    } else if (id == F("freeheap")) {
        nom = F("Memory - Free Heap (RAM)");
        value = (String) ESP.getFreeHeap();
        value += F(" bytes available<br>");
        value += F("<progress value='");
        value += (String)(ESP.getHeapFragmentation());
        value += F("' max='100'></progress> (0% is clean, more than ~50% is not harmless)");
    } else if (id == F("memsketch")) {
        nom = F("Memory - Sketch Size");
        if(this->_fileSystem != nullptr) value = this->_fileSystem->formatBytes(ESP.getSketchSize());
        else value = (String)ESP.getSketchSize();
        value += F(" / ");
        if(this->_fileSystem != nullptr) value += this->_fileSystem->formatBytes(ESP.getSketchSize() + ESP.getFreeSketchSpace());
        else value += (String)(ESP.getSketchSize() + ESP.getFreeSketchSpace());
        value += F(" (Used / Total bytes)");
//        value += F("<br><progress value='");
//        value += (String)(ESP.getSketchSize()) ;
//        value += F("' max='");
//        value += (String)(ESP.getSketchSize()+ESP.getFreeSketchSpace());
//        value += F("'></progress>");
    } else if (id == F("allmems")) {
        //|--------------|-------|---------------|--|--|--|--|--|
        //^              ^       ^               ^     ^
        //Sketch    OTA update   File system   EEPROM  WiFi config (SDK)
//    ESP.getFlashChipRealSize(); // en bytes

//    ESP.getSketchSize(); // en bytes
//    ESP.getFreeSketchSpace(); // en bytes

//    FSInfo fsInfo;
//    fileSystem->info(fsInfo);
//    DEBUG_PRINT(F("info totalbytes :    "));
//    DEBUG_PRINT((float)(fsInfo.totalBytes / 1024.0));
//    DEBUG_PRINTLN(F(" ko"));
//    DEBUG_PRINT(F("info usedbytes :     "));
//    DEBUG_PRINT((float)(fsInfo.usedBytes/1024.0));
//    DEBUG_PRINTLN(F(" ko"));

//    ESP.getFreeHeap(); // ram en bytes
//    ESP.getHeapFragmentation()); // returns the fragmentation metric (0% is clean, more than ~50% is not harmless)
//    ESP.getMaxFreeBlockSize());     // returns the maximum allocatable ram block regarding heap fragmentation

        nom = F("All Memories");
        value = F("Sketch (bytes) / FileSystem (bytes) / RAM (%)<br>");
        value += F("<progress value='");
        value += (String)(ESP.getSketchSize());
        value += F("' max='");
        value += (String)(ESP.getSketchSize() + ESP.getFreeSketchSpace());
        value += F("'></progress>");
        FSInfo fsInfo;
        if(this->_fileSystem != nullptr) this->_fileSystem->fileSystem->info(fsInfo);
        value += F("<progress value='");
        value += (String)(fsInfo.usedBytes);
        value += F("' max='");
        value += (String)(fsInfo.totalBytes);
        value += F("'></progress>");
        value += F("<progress value='");
        value += (String)(ESP.getHeapFragmentation());
        value += F("' max='100'></progress>");
    } else if (id == F("lastreset")) {
        nom = F("Last reset reason");
        value = (String) ESP.getResetReason();
    } else if (id == F("wifimode")) {
        nom = F("Wifi Mode");
        //WIFI_OFF = 0, WIFI_STA = 1, WIFI_AP = 2, WIFI_AP_STA = 3,
        int wifi_mode = WiFi.getMode();
        switch (wifi_mode) {
            case 0: // WIFI_OFF
                value = F("WIFI_OFF");
                break;
            case 1: // WIFI_STA
                value = F("WIFI_STA");
                break;
            case 2: // WIFI_AP
                value = F("WIFI_AP");
                break;
            case 3: // WIFI_AP_STA
                value = F("WIFI_AP_STA");
                break;
        }
    } else if (id == F("apip")) {
        nom = F("Access Point IP");
        value = WiFi.softAPIP().toString();
    } else if (id == F("apmac")) {
        nom = F("Access Point MAC");
        value = (String) WiFi.softAPmacAddress();
    } else if (id == F("ssid")) {
        nom = F("SSID");
        value = (String) WiFi.SSID();
    } else if (id == F("bssid")) {
        nom = F("BSSID");
        value = (String) WiFi.BSSIDstr();
    } else if (id == F("staip")) {
        nom = F("Station IP");
        value = WiFi.localIP().toString();
    } else if (id == F("stagw")) {
        nom = F("Station Gateway");
        value = WiFi.gatewayIP().toString();
    } else if (id == F("stasub")) {
        nom = F("Station Subnet");
        value = WiFi.subnetMask().toString();
    } else if (id == F("dnss")) {
        nom = F("DNS Server");
        value = WiFi.dnsIP().toString();
    } else if (id == F("host")) {
        nom = F("Hostname");
        value = WiFi.hostname();
    } else if (id == F("stamac")) {
        nom = F("Station MAC");
        value = WiFi.macAddress();
    } else if (id == F("conx")) {
        nom = F("Connected");
        value = WiFi.isConnected() ? F("Yes") : F("No");
    } else if (id == F("autoconx")) {
        nom = F("Autoconnect");
        value = WiFi.getAutoConnect() ? F("Enabled") : F("Disabled");
    } else {
        nom = F("Error");
        value = F("Error");
    }

    tab[0] = nom;
    tab[1] = value;
}*/