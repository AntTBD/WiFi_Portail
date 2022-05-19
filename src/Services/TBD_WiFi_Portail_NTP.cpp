//
// Created by antbd on 07/06/2021.
//

#include "TBD_WiFi_Portail_NTP.h"

#ifdef USE_NTP


namespace WiFi_Portail_API {

    NTPManagerClass NTPManager;

    NTPManagerClass::NTPManagerClass()
    {
        this->_timeZone = 1;
        this->_minutesTimeZone = 0;
        this->_dayLight = true;
        this->_timeout = 1500;
        this->_interval = 63;
        this->_couldSendTime = false;
    }

    NTPManagerClass::~NTPManagerClass() {
        NTP.stop();
        this->_couldSendTime = false;
    }

    // "pool.ntp.org";
    // "time.nist.gov";
    // "time-a.timefreq.bldrdoc.gov";
    // "time-b.timefreq.bldrdoc.gov";
    // "time-c.timefreq.bldrdoc.gov";
    void NTPManagerClass::begin(const String &ntpServerName) {

        SerialDebug_println(F("========= Setup NTP ========="));
        this->_ntpServerName = ntpServerName;

        NTP.setInterval(this->_interval);
        NTP.setNTPTimeout(this->_timeout);
        NTP.onNTPSyncEvent(
                [&](NTPSyncEvent_t event) { this->onNTPSyncEventFunction(event); }
        );

        if(NTP.begin(this->_ntpServerName, this->_timeZone, this->_dayLight,
                             this->_minutesTimeZone )){
            SerialDebug_println(F("NTP Client initialized"));
        }else{
            SerialDebug_println(F("NTP Client initialization failed"));
        }

        //NTP.setDayLight(this->_dayLight);
        //NTP.setNtpServerName(this->_ntpServerName);
        //NTP.setTimeZone(this->_timeZone, this->_minutesTimeZone);

        SerialDebug_printf(F("Interval: %d\n"), NTP.getInterval());
        SerialDebug_printf(F("NTPTimeout: %d\n"), NTP.getNTPTimeout());
        SerialDebug_printf(F("DayLight: %d\n"), NTP.getDayLight());
        SerialDebug_printf(F("NtpServerName: %s\n"), NTP.getNtpServerName().c_str());
        SerialDebug_printf(F("TimeZone: %d\n"), NTP.getTimeZone());
        SerialDebug_printf(F("TimeZoneMinutes: %d\n"), NTP.getTimeZoneMinutes());
        SerialDebug_println(F("============================="));
    }

    void NTPManagerClass::loop() const {
        static long last = 0;
        if ((millis() - last) > 5000) {
            last = millis();
            SerialDebug_println(this->getTimeDateString());
        }
    }

    bool NTPManagerClass::setNtpServerName(const String &ntpServerName) {
        this->_ntpServerName = ntpServerName;
        return NTP.setNtpServerName(this->_ntpServerName);
    }

    String NTPManagerClass::getNtpServerName() const {
        return NTP.getNtpServerName();
    }

    bool NTPManagerClass::setInterval(int interval) {
        this->_interval = interval;
        return NTP.setInterval(this->_interval);
    }

    int NTPManagerClass::getInterval() const {
        return NTP.getInterval();
    }

    bool NTPManagerClass::setTimeZone(int8_t timeZone, int8_t minutes) {
        this->_timeZone = timeZone;
        this->_minutesTimeZone = minutes;
        return NTP.setTimeZone(this->_timeZone, this->_minutesTimeZone);
    }

    int8_t NTPManagerClass::getTimeZoneHours() const {
        return NTP.getTimeZone();
    }

    int8_t NTPManagerClass::getTimeZoneMinutes() const {
        return NTP.getTimeZoneMinutes();
    }

    void NTPManagerClass::setDayLight(bool dayLight) {
        this->_dayLight = dayLight;
        NTP.setDayLight(this->_dayLight);
    }

    bool NTPManagerClass::getDayLight() const {
        return NTP.getDayLight();
    }

    String NTPManagerClass::getTimeDateString() const {
        String timeDateString;
        timeDateString += NTP.getTimeDateString(NTP.getLastNTPSync());
        timeDateString += F(" ");
        timeDateString += NTP.isSummerTime() ? F("Summer Time") : F("Winter Time");
        return timeDateString;
    }

    String NTPManagerClass::getUptimeString() const {
        String uptimeString;
        uptimeString += F(" ");
        uptimeString += NTP.getUptimeString();
        uptimeString += F(" since ");
        uptimeString += NTP.getTimeDateString(NTP.getFirstSync());
        return uptimeString;
    }

    bool NTPManagerClass::couldSendTime() const {
        return this->_couldSendTime;
    }

    void NTPManagerClass::stopSendTime() {
        this->_couldSendTime = false;
    }

    RealTime NTPManagerClass::getRealTime() const {
        return this->_realTime;
    }

    void NTPManagerClass::processSyncEvent(NTPSyncEvent_t ntpEvent) {
        if (ntpEvent < 0) {
            SerialDebug_printf(F("Time Sync error: %d\n"), ntpEvent);
            if (ntpEvent == noResponse)
                SerialDebug_println(F("NTP server not reachable"));
            else if (ntpEvent == invalidAddress)
                SerialDebug_println(F("Invalid NTP server address"));
            else if (ntpEvent == errorSending)
                SerialDebug_println(F("Error sending request"));
            else if (ntpEvent == responseError)
                SerialDebug_println(F("NTP response error"));
        } else {
            if (ntpEvent == timeSyncd) {
                SerialDebug_print(F("Got NTP time: "));
                SerialDebug_println(NTP.getTimeDateString(NTP.getLastNTPSync()));
                this->updateRealTimeValue();

                //            this->sendRealTimeByWebSocket();
                this->_couldSendTime = true;
            }
        }
    }

    void NTPManagerClass::updateRealTimeValue() {
        String time = NTP.getTimeStr();
        String date = NTP.getDateStr();

        this->_realTime.hour = time.substring(0, 2).toInt();
        this->_realTime.min = time.substring(3, 5).toInt();
        this->_realTime.sec = time.substring(6, 8).toInt();
        this->_realTime.day = date.substring(0, 2).toInt();
        this->_realTime.month = date.substring(3, 5).toInt();
        this->_realTime.year = date.substring(6, 10).toInt();
        this->_realTime.timezone = this->getTimeZoneHours() + (this->getTimeZoneMinutes() / 60);
        this->_realTime.summerTime = NTP.isSummerTime();

        SerialDebug_printf(F("Real Time : %s"), this->_realTime.toString().c_str());
    }

    void NTPManagerClass::testIfSyncEventTriggered() {
        if (this->_syncEventTriggered) {
            this->processSyncEvent(this->_ntpEvent);
            this->_syncEventTriggered = false;
        }
    }

    void NTPManagerClass::onNTPSyncEventFunction(NTPSyncEvent_t event) {
        this->_ntpEvent = event;
        this->_syncEventTriggered = true;
    }

/// Convert date to JSON
    DynamicJsonDocument NTPManagerClass::getRealTimeJson() {

        SerialDebug_println(NTP.getTimeDateString());
        this->updateRealTimeValue();

        DynamicJsonDocument doc(JSON_OBJECT_SIZE(2) + 2 * JSON_OBJECT_SIZE(8));
        doc[F("resultof")] = F("getRealTime");
        JsonObject realTime = doc.createNestedObject(F("value"));
        realTime[F("day")] = this->_realTime.day;
        realTime[F("month")] = this->_realTime.month;
        realTime[F("year")] = this->_realTime.year;
        realTime[F("hour")] = this->_realTime.hour;
        realTime[F("min")] = this->_realTime.min;
        realTime[F("sec")] = this->_realTime.sec;
        realTime[F("timezone")] = this->_realTime.timezone;
        realTime[F("summerTime")] = this->_realTime.summerTime ? 1 : 0;

        return doc;
    }

    DynamicJsonDocument NTPManagerClass::getRealTimeJsonObj() {

        SerialDebug_println(NTP.getTimeDateString());
        this->updateRealTimeValue();

        DynamicJsonDocument doc(2 * JSON_OBJECT_SIZE(8));
        doc[F("day")] = this->getRealTime().day;
        doc[F("month")] = this->getRealTime().month;
        doc[F("year")] = this->getRealTime().year;
        doc[F("hour")] = this->getRealTime().hour;
        doc[F("min")] = this->getRealTime().min;
        doc[F("sec")] = this->getRealTime().sec;
        doc[F("timezone")] = this->getRealTime().timezone;
        doc[F("summerTime")] = this->getRealTime().summerTime ? 1 : 0;

        //serializeJsonPretty(doc, Serial);

        return doc;
    }

/// Convert all infos to JSON and then to string
    String NTPManagerClass::getRealTimeString() {
        String jsonStringRealTime;
        jsonStringRealTime.reserve(200);
        serializeJsonPretty(this->getRealTimeJson(),
                            jsonStringRealTime); // on remplie la string avec les infos de JSONInfo sous forme sérialisé
        //Serial.println(jsonStringRealTime);
        return jsonStringRealTime;
    }

/// Convert date to JSON
    DynamicJsonDocument NTPManagerClass::getUptimeJson() {
        DynamicJsonDocument doc(JSON_ARRAY_SIZE(2) + 2 * JSON_OBJECT_SIZE(4));
        doc[F("resultof")] = F("getUptime");
        doc[F("value")] = this->getUptimeString();

        return doc;
    }
}
#endif // USE_NTP