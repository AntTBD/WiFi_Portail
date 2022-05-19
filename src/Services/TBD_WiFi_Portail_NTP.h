//
// Created by antbd on 07/06/2021.
//

#ifndef TBD_WIFI_PORTAIL_NTP_H
#define TBD_WIFI_PORTAIL_NTP_H

#include "Service.h"

#ifdef USE_NTP

// from https://github.com/gmag11/NtpClient/blob/master/examples/NTPClientBasic/NTPClientBasic.ino
#include <ESPAsyncUDP.h>
#include <TimeLib.h>      //TimeLib library is needed https://github.com/PaulStoffregen/Time
#include <NtpClientLib.h> //Include NtpClient library header
/*
#warning Be sure to modify the NTPClientLib.h library in the first lines: \
    "using namespace std;\n \
    using namespace placeholders;" \
    by this: \
    "//using namespace std;\n \
    using namespace std::placeholders;" \
    and, DO NOT USE OTHER "using namespace std" in your code
*/

namespace WiFi_Portail_API {


// https://projetsdiy.fr/esp8266-web-recuperer-heure-internet-ntp-ntpclientlib-ntpclient/

    struct RealTime {
        int day;
        int month;
        int year;
        int hour;
        int min;
        int sec;
        float timezone;
        bool summerTime;

        String toString() const {
            String s;
            s += this->day;
            s += F("/");
            s += this->month;
            s += F("/");
            s += this->year;
            s += F(" ");
            s += this->hour;
            s += F(":");
            s += this->min;
            s += F(":");
            s += this->sec;
            s += F(" UTC");
            s += this->timezone < 0 ? F("") : F("+");
            s += this->timezone;
            s += F(" ");
            s += this->summerTime ? F("Summer Time") : F("Winter Time");
            return s;
        }
    };

    class NTPManagerClass : public Service {
    public:
        NTPManagerClass();

        ~NTPManagerClass();

        void begin(const String &ntpServerName = "pool.ntp.org");

        void loop() const;

        bool setNtpServerName(const String &ntpServerName);

        String getNtpServerName() const;

        bool setInterval(int interval);

        int getInterval() const;

        bool setTimeZone(int8_t timeZone, int8_t minutes = 0);

        int8_t getTimeZoneHours() const;

        int8_t getTimeZoneMinutes() const;

        void setDayLight(bool dayLight);

        bool getDayLight() const;

        String getTimeDateString() const;

        String getUptimeString() const;

        bool couldSendTime() const;

        void stopSendTime();

        RealTime getRealTime() const;

        void processSyncEvent(NTPSyncEvent_t ntpEvent);

        void testIfSyncEventTriggered();

        void onNTPSyncEventFunction(NTPSyncEvent_t event);

        void updateRealTimeValue();

        DynamicJsonDocument getRealTimeJson();

        DynamicJsonDocument getRealTimeJsonObj();

        String getRealTimeString();

        DynamicJsonDocument getUptimeJson();

        DynamicJsonDocument toJson() override { return this->getRealTimeJson(); };

        DynamicJsonDocument toJson2() override { return this->getUptimeJson(); };

        DynamicJsonDocument toObj() override { return this->getRealTimeJsonObj(); };

        String toString() override { return this->getRealTimeString(); };

        String toString2() override { return this->getUptimeString(); };

    private:

        boolean _syncEventTriggered = false; // True if a time even has been triggered
        boolean _couldSendTime = false;
        NTPSyncEvent_t _ntpEvent; // Last triggered event

        RealTime _realTime;

        String _ntpServerName;
        int _timeZone; // 0=UTC+0h
        int _minutesTimeZone;
        bool _dayLight;
        int _timeout;
        int _interval;
    };

    extern NTPManagerClass NTPManager;
}
#endif // USE_NTP

#endif //TBD_WIFI_PORTAIL_NTP_H
