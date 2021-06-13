//
// Created by antbd on 07/06/2021.
//

#ifndef TBD_WIFI_PORTAIL_NTP_H
#define TBD_WIFI_PORTAIL_NTP_H

#include <Arduino.h>
#include "TBD_WiFi_Portail.h"
#include "TBD_WiFi_Portail_SerialDebug.h"
#include "TBD_WiFi_Portail_WebSocket.h"
#include <ArduinoJson.h>

// from https://github.com/gmag11/NtpClient/blob/master/examples/NTPClientBasic/NTPClientBasic.ino
#include <ESPAsyncUDP.h>
#include <TimeLib.h> //TimeLib library is needed https://github.com/PaulStoffregen/Time
#include <NtpClientLib.h> //Include NtpClient library header

#warning Be sure to modify the NTPClientLib.h library in the first lines: \
    "using namespace std;\n \
    using namespace placeholders;" \
    by this: \
    "//using namespace std;\n \
    using namespace std::placeholders;" \
    and, DO NOT USE OTHER "using namespace std" in your code


// https://projetsdiy.fr/esp8266-web-recuperer-heure-internet-ntp-ntpclientlib-ntpclient/

struct RealTime {
    int day;
    int month;
    int year;
    int hour;
    int min;
    int sec;

    String toString(){
        return String(this->day)+F("/")+String(this->month)+F("/")+String(this->year)+F(" ")+String(this->hour)+F(":")+String(this->min)+F(":")+String(this->sec);
    }
};

class TBD_WiFi_Portail_NTP {
public:
    TBD_WiFi_Portail_NTP(TBD_WiFi_Portail_SerialDebug& serialDebug, String ntpServerName = "pool.ntp.org");
    ~TBD_WiFi_Portail_NTP();
    void addWebSocket(TBD_WiFi_Portail_WebSocket& webSocket);

    void begin();
    void loop();

    bool setNtpServerName(String ntpServerName);
    String getNtpServerName();

    bool setInterval(int interval);
    int getInterval();

    bool setTimeZone(int8_t timeZone, int8_t minutes = 0);
    int8_t getTimeZoneHours();
    int8_t getTimeZoneMinutes();

    void setDayLight(bool dayLight);
    bool getDayLight();

    String getTimeDateString();
    String getUptimeString();

    void processSyncEvent(NTPSyncEvent_t ntpEvent);
    void testIfSyncEventTriggered();
    void onNTPSyncEventFunction(NTPSyncEvent_t event);
    void sendRealTimeByWebSocket();
    void sendUptimeByWebSocket();

    NTPClient* ntp; // for NtpClientLib.h
    RealTime realTime;
private:
    TBD_WiFi_Portail_SerialDebug* _serialDebug;
    TBD_WiFi_Portail_WebSocket* _webSocket;

    boolean syncEventTriggered = false; // True if a time even has been triggered
    NTPSyncEvent_t ntpEvent; // Last triggered event
    AsyncUDP* _udp;


    String _ntpServerName;
    int _timeZone; // 0=UTC+0h
    int _minutesTimeZone;
    bool _dayLight;
    int _timeout;
    int _interval;
};


#endif //TBD_WIFI_PORTAIL_NTP_H
