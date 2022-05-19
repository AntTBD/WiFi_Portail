//
// Created by antbd on 16/06/2021.
//

#ifndef TBD_WIFI_PORTAIL_ESP_H
#define TBD_WIFI_PORTAIL_ESP_H

#include "Service.h"
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
            String str;
            str += F("{\"value\":\"");
            str += this->value;
            str += F("\",\"min\":\"");
            str += this->min;
            str += F("\",\"max\":\"");
            str += this->max;
            str += F("\",\"unity\":\"");
            str += this->unity;
            str += F("\"}");
            return str;
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
            result += F("{\"name\":\"");
            result += this->name;
            result += F("\"");
            if (this->value != "") {
                result += F(", \"value\":\"");
                result += this->value;
                result += F("\"");
            }
            if (this->progress.value != -1.0) {
                result += F(", \"progress\":");
                result += this->progress.ToSting();
            }
            result += F("}");
            return result;
        }
    } sNameValue;


/// Class for all information from ESP
    class ESPInfosClass : public Service {
    public:
        ESPInfosClass();

        ~ESPInfosClass();

        DynamicJsonDocument getHardwareInfosJson2();

        DynamicJsonDocument getHardwareInfosJsonObj();

        String getHardwareInfosString2();


        virtual DynamicJsonDocument toJson() override;

        virtual DynamicJsonDocument toObj() override;

        virtual String toString() override;

    private:

        std::vector <String> *_deviceStatus;
        std::vector <String> *_networkStatus;

        uint16_t nbInfos;

        //void getInfoData(const String& id, String (&tab)[2]);
        sNameValue getInfoData2(const String &id);
    };

    extern ESPInfosClass ESPInfos;
}
#endif //TBD_WIFI_PORTAIL_ESP_H
