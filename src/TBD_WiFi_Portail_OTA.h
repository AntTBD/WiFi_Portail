//
// Created by antbd on 06/09/2021.
//

#ifndef TBD_WIFI_PORTAIL_OTA_H
#define TBD_WIFI_PORTAIL_OTA_H

#include "TBD_WiFi_Portail.h"
#ifdef USE_OTA
#include "TBD_WiFi_Portail_WebEvents.h"
#include "TBD_WiFi_Portail_MDNS.h"

#define OTA_DEBUG Serial
#include <ArduinoOTA.h>

#warning Be shure to be in the same network for ota update (computer self-sharedWifi not works !)

namespace WiFi_Portail_API {

    class OTAManagerClass {
    public:
        OTAManagerClass();

        ~OTAManagerClass();

        // Add web events to have ability to informe client during ota update
        void addWebEvents(WebEvents &webEvents);

        void setHostname(const String &hostname);

        void setPassword(const String &password);

        String getHostname() const;

        String getPassword() const;

        void setArduinoOtaUseMDNS(bool useMDNS);

        bool getArduinoOtaUseMDNS() const;

        String toString() const;

        void begin(String hostname = "user_OTA", String password = "password_OTA");

        void loop();

        void rebootESP8266();

        void reset_all_pins();

        void arduinoOTAOnStart();

        void arduinoOTAOnEnd();

        void arduinoOTAOnProgress(unsigned int progress, unsigned int total);

        void arduinoOTAOnError(ota_error_t error);

    private:

        WebEvents *_webEvents;
        String _hostname;
        String _password;
        bool _arduinoOtaUseMDNS;

        int led_pin;
        unsigned long previousMillisSendInfos = 0; // will store last time send informations was updated
        unsigned long intervalSendInfos = 1000;    // interval at which to send informations when update firmware by ota

    };
    extern OTAManagerClass OTAManager;
}
#endif // USE_OTA

#endif //TBD_WIFI_PORTAIL_OTA_H
