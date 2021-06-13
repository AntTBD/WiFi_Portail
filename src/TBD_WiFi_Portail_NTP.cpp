//
// Created by antbd on 07/06/2021.
//

#include "TBD_WiFi_Portail_NTP.h"

TBD_WiFi_Portail_NTP::TBD_WiFi_Portail_NTP(TBD_WiFi_Portail_SerialDebug& serialDebug, String ntpServerName): _serialDebug(&serialDebug) {
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
    this->ntp = &NTP;
    this->_udp = new AsyncUDP();
    this->_webSocket = nullptr;

}

TBD_WiFi_Portail_NTP::~TBD_WiFi_Portail_NTP(){
    this->ntp->stop();
}
void TBD_WiFi_Portail_NTP::addWebSocket(TBD_WiFi_Portail_WebSocket& webSocket){
    this->_webSocket = &webSocket;
}

void TBD_WiFi_Portail_NTP::begin(){
    this->_serialDebug->println(F("========= Setup NTP ========="));
    this->ntp->setInterval(this->_interval);
    this->ntp->setNTPTimeout(this->_timeout);
    this->ntp->onNTPSyncEvent(
            //std::bind(&TBD_WiFi_Portail_NTP::onNTPSyncEventFunction, this,std::placeholders::_1)
            [&](NTPSyncEvent_t event) {this->onNTPSyncEventFunction(event);}
            //this->onNTPSyncEventFunction
            );
    this->ntp->begin(this->_ntpServerName, this->_timeZone, this->_dayLight, this->_minutesTimeZone/*, this->_udp*/);
    //this->ntp->setDayLight(this->_dayLight);
    //this->ntp->setNtpServerName(this->_ntpServerName);
    //this->ntp->setTimeZone(this->_timeZone, this->_minutesTimeZone);

    this->_serialDebug->printf(F("Interval: %d\n"), this->ntp->getInterval());
    this->_serialDebug->printf(F("NTPTimeout: %d\n"), this->ntp->getNTPTimeout());
    this->_serialDebug->printf(F("DayLight: %d\n"), this->ntp->getDayLight());
    this->_serialDebug->printf(F("NtpServerName: %s\n"), this->ntp->getNtpServerName().c_str());
    this->_serialDebug->printf(F("TimeZone: %d\n"), this->ntp->getTimeZone());
    this->_serialDebug->printf(F("TimeZoneMinutes: %d\n"), this->ntp->getTimeZoneMinutes());
    this->_serialDebug->println(F("============================="));
}

void TBD_WiFi_Portail_NTP::loop() {
    static long last = 0;
    if ((millis() - last) > 5000) {
        last = millis();
        _serialDebug->println(this->getTimeDateString());
    }
}

bool TBD_WiFi_Portail_NTP::setNtpServerName(String ntpServerName){
    this->_ntpServerName = ntpServerName;
    return this->ntp->setNtpServerName(this->_ntpServerName);
}
String TBD_WiFi_Portail_NTP::getNtpServerName(){
    return this->ntp->getNtpServerName();
}

bool TBD_WiFi_Portail_NTP::setInterval(int interval){
    this->_interval = interval;
    return this->ntp->setInterval(interval);
}
int TBD_WiFi_Portail_NTP::getInterval(){
    return this->ntp->getInterval();
}

bool TBD_WiFi_Portail_NTP::setTimeZone(int8_t timeZone, int8_t minutes){
    this->_timeZone = timeZone;
    this->_minutesTimeZone = minutes;
    return this->ntp->setTimeZone(timeZone, minutes);
}
int8_t TBD_WiFi_Portail_NTP::getTimeZoneHours(){
    return this->ntp->getTimeZone();
}
int8_t TBD_WiFi_Portail_NTP::getTimeZoneMinutes(){
    return this->ntp->getTimeZoneMinutes();
}

void TBD_WiFi_Portail_NTP::setDayLight(bool dayLight){
    this->_dayLight = dayLight;
    this->ntp->setDayLight(dayLight);
}
bool TBD_WiFi_Portail_NTP::getDayLight() {
    return this->ntp->getDayLight();
}

String TBD_WiFi_Portail_NTP::getTimeDateString(){
    return (String)F(" ") + this->ntp->getTimeDateString(this->ntp->getLastNTPSync()) + F(" ") + (this->ntp->isSummerTime() ? F("Summer Time") : F("Winter Time"));
}

String TBD_WiFi_Portail_NTP::getUptimeString(){
    return (String)F("") + this->ntp->getUptimeString() +
    (String)F(" since ") + this->ntp->getTimeDateString(this->ntp->getFirstSync());
}

void TBD_WiFi_Portail_NTP::processSyncEvent(NTPSyncEvent_t ntpEvent) {
    if (ntpEvent < 0) {
        this->_serialDebug->printf(F("Time Sync error: %d\n"), ntpEvent);
        if (ntpEvent == noResponse)
            this->_serialDebug->println(F("NTP server not reachable"));
        else if (ntpEvent == invalidAddress)
            this->_serialDebug->println(F("Invalid NTP server address"));
        else if (ntpEvent == errorSending)
            this->_serialDebug->println(F("Error sending request"));
        else if (ntpEvent == responseError)
            this->_serialDebug->println(F("NTP response error"));
    } else {
        if (ntpEvent == timeSyncd) {
            this->_serialDebug->print(F("Got NTP time: "));
            this->_serialDebug->println(this->ntp->getTimeDateString(this->ntp->getLastNTPSync()));
            String time = this->ntp->getTimeStr();
            String date = this->ntp->getDateStr();
            this->realTime.hour = time.substring(0, 2).toInt();
            this->realTime.min = time.substring(3, 5).toInt();
            this->realTime.sec = time.substring(6, 8).toInt();
            this->realTime.day = date.substring(0, 2).toInt();
            this->realTime.month = date.substring(3, 5).toInt();
            this->realTime.year = date.substring(6, 10).toInt();
            this->_serialDebug->println(F("Real Time : ")+this->realTime.toString());

            this->sendRealTimeByWebSocket();

        }
    }
}

void TBD_WiFi_Portail_NTP::testIfSyncEventTriggered(){
    if (this->syncEventTriggered) {
        this->processSyncEvent(this->ntpEvent);
        this->syncEventTriggered = false;

    }
}

void TBD_WiFi_Portail_NTP::onNTPSyncEventFunction(NTPSyncEvent_t event) {
    //this->_serialDebug->println(F("onNTPSyncEvent"));
    this->ntpEvent = event;
    this->syncEventTriggered = true;
}

void TBD_WiFi_Portail_NTP::sendRealTimeByWebSocket(){
    if(this->_webSocket != nullptr) {
        DynamicJsonDocument doc(JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(6));
        doc["resultof"] = "getRealTime";
        JsonObject realTime = doc.createNestedObject("realTime");
        realTime["day"]     = this->realTime.day;
        realTime["month"]   = this->realTime.month;
        realTime["year"]    = this->realTime.year;
        realTime["hour"]    = this->realTime.hour;
        realTime["min"]     = this->realTime.min;
        realTime["sec"]     = this->realTime.sec;


        this->_webSocket->sendJsonByWebsocket(&doc);
    }
}

void TBD_WiFi_Portail_NTP::sendUptimeByWebSocket(){
    if(this->_webSocket != nullptr) {
        DynamicJsonDocument doc(JSON_ARRAY_SIZE(2)+JSON_OBJECT_SIZE(4));
        doc["resultof"] = "getUptime";
        doc["uptime"] = this->getUptimeString();


        this->_webSocket->sendJsonByWebsocket(&doc);
    }
}