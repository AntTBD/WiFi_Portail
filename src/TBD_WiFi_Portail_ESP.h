//
// Created by antbd on 16/06/2021.
//

#ifndef TBD_WIFI_PORTAIL_ESP_H
#define TBD_WIFI_PORTAIL_ESP_H

#include <Arduino.h>
#include "TBD_WiFi_Portail.h"
#include "Service.h"
#include "TBD_WiFi_Portail_FileSystem.h"
#include <ESP8266WiFi.h>

#include <vector>

namespace WiFi_Portail_API {

    typedef struct Progress {
        float value;
        float min;
        float max;
        String unity;

        Progress() : value(-1.0), min(0.0), max(100.0), unity(F("")) {}

        ~Progress() {}

        String ToSting() const {
            return (String) F("{\"value\":\"") + String(this->value) + F("\", \"min\":\"") + String(this->min) +
                   F("\", \"max\":\"") + String(this->max) + F("\", \"unity\":\"") + this->unity + F("\"}");
        }
    } sProgress;

    typedef struct NameValue {
        String name;
        String value;
        sProgress progress;

        NameValue() : name(F("")), value(F("")) { /*progress = new sProgress ();*/
            name.reserve(40);
            value.reserve(100);
        }

        ~NameValue() {}

        String ToSting() const {
            String result;
            result.reserve(350);
            result = (String) F("{\"name\":\"") + this->name + F("\"");
            if (this->value != "") {
                result += (String) F(", \"value\":\"") + this->value + F("\"");
            }
            if (this->progress.value != -1.0) {
                result += (String) F(", \"progress\":") + this->progress.ToSting();
            }
            result += (String) F("}");
            return result;
        }
    } sNameValue;
/*struct nameProgress {
    String name;
    progress progress;

    String ToSting() { return F("{\"name\":\"") + name + F("\", \"progress\":") + progress + F('}'); }
};

struct device_status{
    nameValue uptime;
    nameValue chipid;
    nameValue fchipid;
    nameValue idesize;
    nameValue realsize;
    nameValue flashspeed;
    nameValue flashmode;
    nameValue sdkver;
    nameValue corever;
    nameValue bootver;
    nameValue cpufreq;
    nameProgress freeheap;
    nameValue memsketch;
    nameProgress allmems;
    nameValue lastreset;
};
struct wifi_status{
    nameValue wifimode;
    nameValue apip;
    nameValue apmac;
    nameValue apssid;
    nameValue apbssid;
    nameValue staip;
    nameValue stagw;
    nameValue stasub;
    nameValue dnss;
    nameValue host;
    nameValue stamac;
    nameValue conx;
    nameValue autoconx;
};
struct status_infos{
    // device_status deviceStatus;
    // wifi_status wifiStatus;
    AFArray<nameValue> deviceStatus;
    AFArray<nameValue> wifiStatus;

};
struct allStatus{
    String softVersion;
    status_infos statusInfos;
};*/

/// Class for all information from ESP
    class ESPInfos : public Service {
    public:
        ESPInfos();

        ~ESPInfos();

        void addNTP(Service &ntp);

        void addFileSystem(FileSystem &fileSystem);

        DynamicJsonDocument getHardwareInfosJson2();

        DynamicJsonDocument getHardwareInfosJsonObj();

        String getHardwareInfosString2();


        DynamicJsonDocument toJson() override { return this->getHardwareInfosJson2(); };

        DynamicJsonDocument toObj() override { return this->getHardwareInfosJsonObj(); };

        String toString() override { return this->getHardwareInfosString2(); };

    private:
        std::vector <String> *_deviceStatus;
        std::vector <String> *_networkStatus;

        uint16_t nbInfos;

        Service *_ntp;
        FileSystem *_fileSystem;

        //void getInfoData(const String& id, String (&tab)[2]);
        sNameValue getInfoData2(const String &id);
    };
}
#endif //TBD_WIFI_PORTAIL_ESP_H
