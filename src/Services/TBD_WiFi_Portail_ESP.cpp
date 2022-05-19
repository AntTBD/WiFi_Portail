//
// Created by antbd on 16/06/2021.
//

#include "TBD_WiFi_Portail_ESP.h"

namespace WiFi_Portail_API {

    ESPInfosClass ESPInfos;

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
    ESPInfosClass::ESPInfosClass() : Service() {

        this->_deviceStatus = new std::vector<String>();
        this->_deviceStatus->insert(this->_deviceStatus->end(), {
                F("uptime"),
                F("chipid"),
                F("fchipid"),
                F("idesize"),
                F("realsize"),
                F("flashspeed"),
                F("flashmode"),// ///////
                F("sdkver"),
                F("corever"),
                F("bootver"),
                F("cpufreq"),
                F("freeheap"),
                F("heapfrag"),
                F("sketch"),
                F("filesystem"),
                F("lastreset")
        });

        this->_networkStatus = new std::vector<String>();
        this->_networkStatus->insert(this->_networkStatus->end(), {
                F("wifimode"),
                F("apip"),
                F("apmac"),
                F("ssid"),
                F("bssid"),
                F("host"),
                F("staip"),
                F("stamac"),
                F("stasub"),
                F("stagw"),
                F("dnss"),
                F("conx"),
                F("autoconx")
        });
    }

/// Destructor
    ESPInfosClass::~ESPInfosClass() {

        this->_deviceStatus->clear();
        delete this->_deviceStatus;
        this->_networkStatus->clear();
        delete this->_networkStatus;
    }


/// Convert all infos to JSON sorted by devices datas and network datas
    DynamicJsonDocument ESPInfosClass::getHardwareInfosJson2() {
        DynamicJsonDocument docHardwareInfos(
                JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(2)
                + JSON_ARRAY_SIZE((this->_deviceStatus->size() + 1)) +
                (this->_deviceStatus->size() + 1) * JSON_OBJECT_SIZE(4)
                + JSON_ARRAY_SIZE((this->_networkStatus->size() + 1)) +
                (this->_networkStatus->size() + 1) * JSON_OBJECT_SIZE(4)
        );
        docHardwareInfos[F("resultof")] = F("getStatus");
        JsonObject hardwareInfos = docHardwareInfos.createNestedObject(F("value"));
        hardwareInfos[F("softVersion")] = WIFI_PORTAIL_VERSION;
        JsonObject statusInfos = hardwareInfos.createNestedObject(F("statusInfos"));
        JsonArray deviceInfos = statusInfos.createNestedArray(F("device"));
        for (String &name: *this->_deviceStatus) {
            sNameValue info = this->getInfoData2(name);
            deviceInfos.add(serialized(info.ToSting()));
        }
        JsonArray networkInfos = statusInfos.createNestedArray(F("network"));
        for (String &name: *this->_networkStatus) {
            sNameValue info = this->getInfoData2(name);
            networkInfos.add(serialized(info.ToSting()));
        }

        return docHardwareInfos;
    }

    DynamicJsonDocument ESPInfosClass::getHardwareInfosJsonObj() {
        DynamicJsonDocument doc(
                JSON_OBJECT_SIZE(2)
                + JSON_ARRAY_SIZE((this->_deviceStatus->size() + 1)) +
                (this->_deviceStatus->size() + 1) * (JSON_OBJECT_SIZE(4))
                + JSON_ARRAY_SIZE((this->_networkStatus->size() + 1)) +
                (this->_networkStatus->size() + 1) * (JSON_OBJECT_SIZE(4))
        );

        doc[F("softVersion")] = WIFI_PORTAIL_VERSION;
        JsonObject statusInfos = doc.createNestedObject(F("statusInfos"));
        JsonArray deviceInfos = statusInfos.createNestedArray(F("device"));
        for (String &name: *this->_deviceStatus) {
            sNameValue info = this->getInfoData2(name);
            deviceInfos.add(serialized(info.ToSting()));
        }
        JsonArray networkInfos = statusInfos.createNestedArray(F("network"));
        for (String &name: *this->_networkStatus) {
            sNameValue info = this->getInfoData2(name);
            networkInfos.add(serialized(info.ToSting()));
        }

        return doc;
    }

/// Convert all infos to JSON and then to string
    String ESPInfosClass::getHardwareInfosString2() {
        String jsonStringHardwareInfos;
        jsonStringHardwareInfos.reserve(2000);
        serializeJsonPretty(this->getHardwareInfosJson2(),
                            jsonStringHardwareInfos); // on remplie la string avec les infos de JSONInfo sous forme sérialisé
        Serial.println(jsonStringHardwareInfos);
        return jsonStringHardwareInfos;
    }

/// Return infos by string name
    sNameValue ESPInfosClass::getInfoData2(const String &id) {
        sNameValue info;

        if (id == F("uptime")) {
            info.name = F("Uptime");
#ifdef USE_NTP
            info.value = NTPManager.toString2();
#else // USE_NTP
            unsigned long millisecs = millis();
            info.value = (String)((millisecs / (1000 * 60 * 60 * 24)) % 365);
            info.value += F(" Days ");
            info.value += (String)((millisecs / (1000 * 60 * 60)) % 24);
            info.value += F(" Hrs ");
            info.value += (String)((millisecs / (1000 * 60)) % 60);
            info.value += F(" Mins ");
            info.value += (String)((millisecs / 1000) % 60);
            info.value += F(" Secs");
#endif // USE_NTP
        } else if (id == F("chipid")) {
            info.name = F("Chip ID");
            info.value = F("0x");
            info.value += String(ESP.getChipId(), HEX);
        } else if (id == F("fchipid")) {
            info.name = F("Flash Chip ID");
            info.value = (String) ESP.getFlashChipId();
        } else if (id == F("idesize")) {
            info.name = F("Flash IDE Size");
            info.value = FileSystem.formatBytes(ESP.getFlashChipSize());
        } else if (id == F("realsize")) {
            info.name = F("Flash Real Size");
            info.value = FileSystem.formatBytes(ESP.getFlashChipRealSize());
        } else if (id == F("flashspeed")) {
            info.name = F("Flash Speed");
            info.value = (String)(float(ESP.getFlashChipSpeed()) / 1000000);
            info.value += F(" MHz");
        } else if (id == F("flashmode")) {
            info.name = F("Flash Mode");
            FlashMode_t ideMode = ESP.getFlashChipMode();
            info.value = (ideMode == FM_QIO ? F("QIO - Quad I/O Fast Read")
                                            : ideMode == FM_QOUT ? F("QOUT - Quad Output Fast Read")
                                                                 : ideMode == FM_DIO ? F("DIO - Dual I/O Fast Read")
                                                                                     : ideMode == FM_DOUT ? F("DOUT - Dual Output Fast Read")
                                                                                                          : F("UNKNOWN"));
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
                info.value = FileSystem.formatBytes(ESP.getFreeHeap()) + F(" / ") +
                        FileSystem.formatBytes(32768);// 32768 = IRAM from compiler
            info.value += F(" <i>(Available / Total)</i>");
            info.progress.value = (float) (ESP.getFreeHeap());
            info.progress.min = 0;
            info.progress.max = 32768;// 32768 = IRAM from compiler
            info.progress.unity = "bytes";
        } else if (id == F("heapfrag")) {
            info.name = F("Memory - Heap Fragmentation");
            info.value = F(
                    "(0% is clean, more than ~50% is not harmless)"); // https://cpp4arduino.com/2018/11/06/what-is-heap-fragmentation.html#measuring-the-fragmentation
            info.progress.value = (float) (ESP.getHeapFragmentation());
            info.progress.min = 100;
            info.progress.max = 0;
            info.progress.unity = F("%");
        } else if (id == F("sketch")) {
            info.name = F("Memory - Sketch Size");
            info.value = FileSystem.formatBytes(ESP.getSketchSize());
            info.value += F(" / ");
            info.value += FileSystem.formatBytes(ESP.getSketchSize() + ESP.getFreeSketchSpace());
            info.value += F(" <i>(Used / Total)</i>");
#ifdef USE_OTA
            if (ESP.getSketchSize() > ESP.getFreeSketchSpace())
                info.value += F("<br><small style=\"color:red\">Sketch too big for OTA transfer!</small>");
#endif // USE_OTA
            info.progress.value = (float) (ESP.getSketchSize());
            info.progress.min = (float) (ESP.getSketchSize() + ESP.getFreeSketchSpace());
            info.progress.max = 0;
            info.progress.unity = F("bytes");
        } else if (id == F("filesystem")) {
            //|--------------|-------|---------------|--|--|--|--|--|
            //^              ^       ^               ^     ^
            //Sketch    OTA update   File system   EEPROM  WiFi config (SDK)

            info.name = F("Memory - FileSystem Size");
            FSInfo fsInfo;
            FileSystem.fileSystem->info(fsInfo);
            info.value += FileSystem.formatBytes(fsInfo.usedBytes);
            info.value += F(" / ");
            info.value += FileSystem.formatBytes(fsInfo.totalBytes);
            info.value += F(" <i>(Used / Total)</i>");
            info.progress.value = (float) (fsInfo.usedBytes);
            info.progress.min = (float) (fsInfo.totalBytes);
            info.progress.max = 0;
            info.progress.unity = F("bytes");
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

    DynamicJsonDocument ESPInfosClass::toJson() { return this->getHardwareInfosJson2(); };

    DynamicJsonDocument ESPInfosClass::toObj() { return this->getHardwareInfosJsonObj(); };

    String ESPInfosClass::toString() { return this->getHardwareInfosString2(); };
}