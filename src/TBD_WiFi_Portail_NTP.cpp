//
// Created by antbd on 07/06/2021.
//

#include "TBD_WiFi_Portail_NTP.h"

#ifdef USE_NTP

TBD_WiFi_Portail_NTP::TBD_WiFi_Portail_NTP(TBD_WiFi_Portail_SerialDebug &serialDebug, const String &ntpServerName) : _serialDebug(&serialDebug)
{
    this->_ntpServerName = ntpServerName;
    // "pool.ntp.org";
    // "time.nist.gov";
    // "time-a.timefreq.bldrdoc.gov";
    // "time-b.timefreq.bldrdoc.gov";
    // "time-c.timefreq.bldrdoc.gov";
    this->_timeZone = 1;
    this->_minutesTimeZone = 0;
    this->_dayLight = true;
    this->_timeout = 1500;
    this->_interval = 63;
    this->_ntp = &NTP;
    this->_udp = new AsyncUDP();
    //this->_webSocket = nullptr;
    this->_couldSendTime = false;
}

TBD_WiFi_Portail_NTP::~TBD_WiFi_Portail_NTP()
{
    delete this->_serialDebug;
    //delete this->_webSocket;
    this->_ntp->stop();
    this->_couldSendTime = false;
}
/*
void TBD_WiFi_Portail_NTP::addWebSocket(TBD_WiFi_Portail_WebSocket& webSocket){
    this->_webSocket = &webSocket;
}*/

void TBD_WiFi_Portail_NTP::begin()
{
    this->_serialDebug->println(F("========= Setup NTP ========="));
    this->_ntp->setInterval(this->_interval);
    this->_ntp->setNTPTimeout(this->_timeout);
    this->_ntp->onNTPSyncEvent(
        //std::bind(&TBD_WiFi_Portail_NTP::onNTPSyncEventFunction, this,std::placeholders::_1)
        [&](NTPSyncEvent_t event)
        { this->onNTPSyncEventFunction(event); }
        //this->onNTPSyncEventFunction
    );
    this->_ntp->begin(this->_ntpServerName, this->_timeZone, this->_dayLight, this->_minutesTimeZone /*, this->_udp*/);
    //this->_ntp->setDayLight(this->_dayLight);
    //this->_ntp->setNtpServerName(this->_ntpServerName);
    //this->_ntp->setTimeZone(this->_timeZone, this->_minutesTimeZone);

    this->_serialDebug->printf(F("Interval: %d\n"), this->_ntp->getInterval());
    this->_serialDebug->printf(F("NTPTimeout: %d\n"), this->_ntp->getNTPTimeout());
    this->_serialDebug->printf(F("DayLight: %d\n"), this->_ntp->getDayLight());
    this->_serialDebug->printf(F("NtpServerName: %s\n"), this->_ntp->getNtpServerName().c_str());
    this->_serialDebug->printf(F("TimeZone: %d\n"), this->_ntp->getTimeZone());
    this->_serialDebug->printf(F("TimeZoneMinutes: %d\n"), this->_ntp->getTimeZoneMinutes());
    this->_serialDebug->println(F("============================="));
}

void TBD_WiFi_Portail_NTP::loop()
{
    static long last = 0;
    if ((millis() - last) > 5000)
    {
        last = millis();
        this->_serialDebug->println(this->getTimeDateString());
    }
}

bool TBD_WiFi_Portail_NTP::setNtpServerName(const String &ntpServerName)
{
    this->_ntpServerName = ntpServerName;
    return this->_ntp->setNtpServerName(this->_ntpServerName);
}
String TBD_WiFi_Portail_NTP::getNtpServerName() const
{
    return this->_ntp->getNtpServerName();
}

bool TBD_WiFi_Portail_NTP::setInterval(int interval)
{
    this->_interval = interval;
    return this->_ntp->setInterval(this->_interval);
}
int TBD_WiFi_Portail_NTP::getInterval() const
{
    return this->_ntp->getInterval();
}

bool TBD_WiFi_Portail_NTP::setTimeZone(int8_t timeZone, int8_t minutes)
{
    this->_timeZone = timeZone;
    this->_minutesTimeZone = minutes;
    return this->_ntp->setTimeZone(this->_timeZone, this->_minutesTimeZone);
}
int8_t TBD_WiFi_Portail_NTP::getTimeZoneHours() const
{
    return this->_ntp->getTimeZone();
}
int8_t TBD_WiFi_Portail_NTP::getTimeZoneMinutes() const
{
    return this->_ntp->getTimeZoneMinutes();
}

void TBD_WiFi_Portail_NTP::setDayLight(bool dayLight)
{
    this->_dayLight = dayLight;
    this->_ntp->setDayLight(this->_dayLight);
}
bool TBD_WiFi_Portail_NTP::getDayLight() const
{
    return this->_ntp->getDayLight();
}

String TBD_WiFi_Portail_NTP::getTimeDateString() const
{
    return (String)F(" ") + this->_ntp->getTimeDateString(this->_ntp->getLastNTPSync()) + F(" ") + (this->_ntp->isSummerTime() ? F("Summer Time") : F("Winter Time"));
}

String TBD_WiFi_Portail_NTP::getUptimeString() const
{
    return (String)F("") + this->_ntp->getUptimeString() +
           (String)F(" since ") + this->_ntp->getTimeDateString(this->_ntp->getFirstSync());
}

bool TBD_WiFi_Portail_NTP::couldSendTime() const
{
    return this->_couldSendTime;
}

void TBD_WiFi_Portail_NTP::stopSendTime()
{
    this->_couldSendTime = false;
}
RealTime TBD_WiFi_Portail_NTP::getRealTime() const
{
    return this->_realTime;
}

void TBD_WiFi_Portail_NTP::processSyncEvent(NTPSyncEvent_t ntpEvent)
{
    if (ntpEvent < 0)
    {
        this->_serialDebug->printf(F("Time Sync error: %d\n"), ntpEvent);
        if (ntpEvent == noResponse)
            this->_serialDebug->println(F("NTP server not reachable"));
        else if (ntpEvent == invalidAddress)
            this->_serialDebug->println(F("Invalid NTP server address"));
        else if (ntpEvent == errorSending)
            this->_serialDebug->println(F("Error sending request"));
        else if (ntpEvent == responseError)
            this->_serialDebug->println(F("NTP response error"));
    }
    else
    {
        if (ntpEvent == timeSyncd)
        {
            this->_serialDebug->print(F("Got NTP time: "));
            this->_serialDebug->println(this->_ntp->getTimeDateString(this->_ntp->getLastNTPSync()));
            this->updateRealTimeValue();

            //            this->sendRealTimeByWebSocket();
            this->_couldSendTime = true;
        }
    }
}

void TBD_WiFi_Portail_NTP::updateRealTimeValue()
{
    String time = this->_ntp->getTimeStr();
    String date = this->_ntp->getDateStr();

    this->_realTime.hour = time.substring(0, 2).toInt();
    this->_realTime.min = time.substring(3, 5).toInt();
    this->_realTime.sec = time.substring(6, 8).toInt();
    this->_realTime.day = date.substring(0, 2).toInt();
    this->_realTime.month = date.substring(3, 5).toInt();
    this->_realTime.year = date.substring(6, 10).toInt();
    this->_realTime.timezone = this->getTimeZoneHours() + (this->getTimeZoneMinutes() / 60);
    this->_realTime.summerTime = this->_ntp->isSummerTime();

    this->_serialDebug->println((String)F("Real Time : ") + this->_realTime.toString());
}

void TBD_WiFi_Portail_NTP::testIfSyncEventTriggered()
{
    if (this->_syncEventTriggered)
    {
        this->processSyncEvent(this->_ntpEvent);
        this->_syncEventTriggered = false;
    }
}

void TBD_WiFi_Portail_NTP::onNTPSyncEventFunction(NTPSyncEvent_t event)
{
    this->_serialDebug->println(F("onNTPSyncEvent"));
    this->_ntpEvent = event;
    this->_syncEventTriggered = true;
}

/// Convert date to JSON
DynamicJsonDocument TBD_WiFi_Portail_NTP::getRealTimeJson()
{

    this->_serialDebug->println(this->_ntp->getTimeDateString());
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

DynamicJsonDocument TBD_WiFi_Portail_NTP::getRealTimeJsonObj()
{

    this->_serialDebug->println(this->_ntp->getTimeDateString());
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
String TBD_WiFi_Portail_NTP::getRealTimeString()
{
    String jsonStringRealTime;
    jsonStringRealTime.reserve(200);
    serializeJsonPretty(this->getRealTimeJson(), jsonStringRealTime); // on remplie la string avec les infos de JSONInfo sous forme sérialisé
    //Serial.println(jsonStringRealTime);
    return jsonStringRealTime;
}
/*
void TBD_WiFi_Portail_NTP::sendRealTimeByWebSocket(){
    if(this->_webSocket != nullptr) {
        DynamicJsonDocument doc(JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(8));
        doc["resultof"] = "getRealTime";
        JsonObject realTime = doc.createNestedObject("value");
        realTime["day"]         = this->_realTime.day;
        realTime["month"]       = this->_realTime.month;
        realTime["year"]        = this->_realTime.year;
        realTime["hour"]        = this->_realTime.hour;
        realTime["min"]         = this->_realTime.min;
        realTime["sec"]         = this->_realTime.sec;
        realTime["timezone"]    = this->_realTime.timezone;
        realTime["summerTime"]  = this->_realTime.summerTime ? 1 : 0;


        this->_webSocket->sendJsonByWebsocket(&doc);
    }
}*/

/// Convert date to JSON
DynamicJsonDocument TBD_WiFi_Portail_NTP::getUptimeJson()
{
    DynamicJsonDocument doc(JSON_ARRAY_SIZE(2) + 2 * JSON_OBJECT_SIZE(4));
    doc[F("resultof")] = F("getUptime");
    doc[F("value")] = this->getUptimeString();

    return doc;
}
/*
void TBD_WiFi_Portail_NTP::sendUptimeByWebSocket(){
    if(this->_webSocket != nullptr) {
        DynamicJsonDocument doc(JSON_ARRAY_SIZE(2)+JSON_OBJECT_SIZE(4));
        doc["resultof"] = "getUptime";
        doc["value"] = this->getUptimeString();


        this->_webSocket->sendJsonByWebsocket(&doc);
    }
}*/

#endif // USE_NTP