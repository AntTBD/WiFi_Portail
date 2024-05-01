//
// Created by antbd on 07/06/2021.
//

#include "WiFi_Portail_NTP.h"

#ifdef USE_NTP


namespace WiFi_Portail_API {

    NTPManagerClass NTPManager;

    NTPManagerClass::NTPManagerClass()
    {
        this->_couldSendTime = false;

        NTP.setNTPTimeout(1500);
        NTP.setInterval(63);
        NTP.setDayLight(true);
        NTP.setTimeZone(1, 0);

        NTP.onNTPSyncEvent(
                [&](NTPSyncEvent_t event) { this->onNTPSyncEventFunction(event); }
        );
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

        if(NTP.begin(ntpServerName, this->getTimeZoneHours(), this->getDayLight(), this->getTimeZoneMinutes() )){
            SerialDebug_println(F("NTP Client initialized"));
        }else{
            SerialDebug_println(F("NTP Client initialization failed"));
        }

        SerialDebug_println(this->toString());
        SerialDebug_println(F("============================="));
    }

    String NTPManagerClass::toString() const {
        String str;
        str += F("NTPManagerClass: ");
        str += F("Interval(): ");
        str += NTP.getInterval();
        str += F("NTPTimeout(): ");
        str += NTP.getNTPTimeout();
        str += F("DayLight(): ");
        str += NTP.getDayLight() ? F("true") : F("false");
        str += F("NtpServerName(): ");
        str += NTP.getNtpServerName().c_str();
        str += F("TimeZone(): ");
        str += NTP.getTimeZone();
        str += F("TimeZoneMinutes(): ");
        str += NTP.getTimeZoneMinutes();
        return str;
    }

    void NTPManagerClass::loop() const {
        static long last = 0;
        if ((millis() - last) > 5000) {
            last = millis();
            SerialDebug_println(this->getTimeDateString());
        }
    }

    bool NTPManagerClass::setNtpServerName(const String &ntpServerName) {
        return NTP.setNtpServerName(ntpServerName);
    }

    String NTPManagerClass::getNtpServerName() const {
        return NTP.getNtpServerName();
    }

    bool NTPManagerClass::setInterval(int interval) {
        return NTP.setInterval(interval);
    }

    int NTPManagerClass::getInterval() const {
        return NTP.getInterval();
    }

    bool NTPManagerClass::setTimeZone(int8_t hours, int8_t minutes) {
        return NTP.setTimeZone(hours, minutes);
    }

    bool NTPManagerClass::setTimeZone(float timeZone) {
        return this->setTimeZone((int)timeZone, timeZone - (int)timeZone);
    }

    int8_t NTPManagerClass::getTimeZone() const {
        return this->getTimeZoneHours() + (this->getTimeZoneMinutes() / 60);
    }

    int8_t NTPManagerClass::getTimeZoneHours() const {
        return NTP.getTimeZone();
    }

    int8_t NTPManagerClass::getTimeZoneMinutes() const {
        return NTP.getTimeZoneMinutes();
    }

    void NTPManagerClass::setDayLight(bool dayLight) {
        NTP.setDayLight(dayLight);
    }

    bool NTPManagerClass::getDayLight() const {
        return NTP.getDayLight();
    }

    bool NTPManagerClass::isSummerTime() const {
        return NTP.isSummerTime();
    }

    String NTPManagerClass::getTimeDateString() const {
        String timeDateString;
        timeDateString += NTP.getTimeDateString(NTP.getLastNTPSync());
        timeDateString += F(" ");
        timeDateString += this->isSummerTime() ? F("Summer Time") : F("Winter Time");
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
        } else if (ntpEvent == timeSyncd) {
                SerialDebug_print(F("Got NTP time: "));
                SerialDebug_println(NTP.getTimeDateString(NTP.getLastNTPSync()));
                this->updateRealTimeValue();

                //            this->sendRealTimeByWebSocket();
                this->_couldSendTime = true;
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
        this->_realTime.timezone = this->getTimeZone();
        this->_realTime.summerTime = this->isSummerTime();

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
        RealTime realTime = this->getRealTime();

        DynamicJsonDocument doc(JSON_OBJECT_SIZE(2) + 2 * JSON_OBJECT_SIZE(8));
        doc[F("resultof")] = F("getRealTime");
        JsonObject realTimeJson = doc.createNestedObject(F("value"));
        realTimeJson[F("day")] = realTime.day;
        realTimeJson[F("month")] = realTime.month;
        realTimeJson[F("year")] = realTime.year;
        realTimeJson[F("hour")] = realTime.hour;
        realTimeJson[F("min")] = realTime.min;
        realTimeJson[F("sec")] = realTime.sec;
        realTimeJson[F("timezone")] = realTime.timezone;
        realTimeJson[F("summerTime")] = realTime.summerTime ? 1 : 0;

        return doc;
    }

    DynamicJsonDocument NTPManagerClass::getRealTimeJsonObj() {

        SerialDebug_println(NTP.getTimeDateString());
        this->updateRealTimeValue();

        RealTime realTime = this->getRealTime();

        DynamicJsonDocument doc(2 * JSON_OBJECT_SIZE(8));
        doc[F("day")] = realTime.day;
        doc[F("month")] = realTime.month;
        doc[F("year")] = realTime.year;
        doc[F("hour")] = realTime.hour;
        doc[F("min")] = realTime.min;
        doc[F("sec")] = realTime.sec;
        doc[F("timezone")] = realTime.timezone;
        doc[F("summerTime")] = realTime.summerTime ? 1 : 0;

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

    bool NTPManagerClass::saveConfigInFile() {

        DynamicJsonDocument doc(2 * JSON_OBJECT_SIZE(4));
        doc[F("ntpServer")] = this->getNtpServerName();
        doc[F("timeZone")] = this->getTimeZone();
        doc[F("interval")] = this->getInterval();
        doc[F("isSummerTime")] = this->getDayLight();

        return FilesManager.saveConfigNTP(&doc);
    }

    void NTPManagerClass::setSettingsFromJson(const String &settingsJson) {
        DynamicJsonDocument doc(JSON_OBJECT_SIZE(1) + settingsJson.length());
        DeserializationError error = deserializeJson(doc, settingsJson);

        if (error) {
            SerialDebug_println(F("deserializeJson() failed: "));
            SerialDebug_println(error.c_str());
            return;
        }
        JsonObject json = doc.as<JsonObject>();
        if (json.containsKey(F("ntpServer"))) {
            this->setNtpServerName(json[F("ntpServer")].as<String>());
        }
        if (json.containsKey(F("timeZone"))) {
            this->setTimeZone(json[F("timeZone")].as<float>());
        }
        if (json.containsKey(F("interval"))) {
            this->setInterval(json[F("interval")].as<float>());
        }
        if (json.containsKey(F("isSummerTime"))) {
            this->setDayLight(json[F("isSummerTime")].as<bool>());
        }
        this->saveConfigInFile();
    }
}
#endif // USE_NTP