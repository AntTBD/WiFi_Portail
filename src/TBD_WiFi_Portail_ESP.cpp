//
// Created by antbd on 16/06/2021.
//

#include "TBD_WiFi_Portail_ESP.h"

TBD_WiFi_Portail_ESP::TBD_WiFi_Portail_ESP() {
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
    this->_infosName += "esphead";
    this->_infosName += "uptime";
    this->_infosName += "chipid";
    this->_infosName += "fchipid";
    this->_infosName += "idesize";
    this->_infosName += "realsize";
    this->_infosName += "flashspeed";
    this->_infosName += "flashmode";// ///////
    this->_infosName += "sdkver";
    this->_infosName += "corever";
    this->_infosName += "bootver";
    this->_infosName += "cpufreq";
    this->_infosName += "freeheap";
    this->_infosName += "memsketch";
    this->_infosName += "allmems";
    this->_infosName += "lastreset";
    this->_infosName += "wifihead";
    this->_infosName += "wifimode";
    this->_infosName += "apip";
    this->_infosName += "apmac";
    this->_infosName += "apssid";
    this->_infosName += "apbssid";
    this->_infosName += "staip";
    this->_infosName += "stagw";
    this->_infosName += "stasub";
    this->_infosName += "dnss";
    this->_infosName += "host";
    this->_infosName += "stamac";
    this->_infosName += "conx";
    this->_infosName += "autoconx";
    this->_ntp = nullptr;
    this->_fileSystem = nullptr;
    this->_webSocket = nullptr;
}
TBD_WiFi_Portail_ESP::~TBD_WiFi_Portail_ESP(){}

void TBD_WiFi_Portail_ESP::addNTP(TBD_WiFi_Portail_NTP& ntp){
    this->_ntp = &ntp;
}
void TBD_WiFi_Portail_ESP::addFileSystem(TBD_WiFi_Portail_FileSystem& fileSystem){
    this->_fileSystem = &fileSystem;
}
void TBD_WiFi_Portail_ESP::addWebSocket(TBD_WiFi_Portail_WebSocket& webSocket){
    this->_webSocket = &webSocket;
}
// -------------------------------- Fonction pour avoir toutes les informations hardware de l'esp8266 -------------------------------------------
DynamicJsonDocument TBD_WiFi_Portail_ESP::getHardwareInfosJson() {
    DynamicJsonDocument docHardwareInfos(
            JSON_OBJECT_SIZE(1) + this->_infosName.size() * JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(this->_infosName.size()) + 2000);
    docHardwareInfos["resultof"] = "getHardwareInfos";
    JsonObject hardwareInfos = docHardwareInfos.createNestedObject(F("hardwareInfos"));

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

String TBD_WiFi_Portail_ESP::getHardwareInfosString() {
    String jsonStringHardwareInfos;
    jsonStringHardwareInfos.reserve(2000);
    serializeJson(this->getHardwareInfosJson(), jsonStringHardwareInfos); // on remplie la string avec les infos de JSONInfo sous forme sérialisé
    //Serial.println(jsonStringHardwareInfos);
    return jsonStringHardwareInfos;
}

void TBD_WiFi_Portail_ESP::sendHardwareInfosByWebSocket(){
    if(this->_webSocket != nullptr) {
        DynamicJsonDocument docHardwareInfos(
                JSON_OBJECT_SIZE(1) + this->_infosName.size() * JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(this->_infosName.size()) + 2000);
        docHardwareInfos["resultof"] = "getHardwareInfos";
        JsonObject hardwareInfos = docHardwareInfos.createNestedObject(F("hardwareInfos"));

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
}
void TBD_WiFi_Portail_ESP::getInfoData(String id, String (&tab)[2]) {
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
        /*value += F("<br><progress value='");
        value += (String)(ESP.getSketchSize()) ;
        value += F("' max='");
        value += (String)(ESP.getSketchSize()+ESP.getFreeSketchSpace());
        value += F("'></progress>");*/
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
    } else if (id == F("apssid")) {
        nom = F("SSID");
        value = (String) WiFi.SSID();
    } else if (id == F("apbssid")) {
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
}