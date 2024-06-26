#ifndef WifiAll_h_included
#define WifiAll_h_included

// Definitions of AP, STA, WifiAll classes

#include <Arduino.h>

#include <vector>
#include <ESP8266WiFi.h> // to include ESP8266WiFiType.h library for WIFI_MODE and Wifi.macAddress()

namespace WiFi_Portail_API {
    class AP;
    class STA;
    class WifiAll;

    class AP {
    public:
        // constructor
        AP();

        AP(const String &ssid);

        AP(const String &ssid, const String &password);

        // destructor
        ~AP();

        // getters and setters
        String getSSID() const;

        void setSSID(const String &_ssid);

        String getPassword() const;

        void setPassword(const String &_password);

        bool isHide() const;

        void setHide(bool _hide);

        String getIp() const;

        void setIp(const String &_ip);

        String getMask() const;

        void setMask(const String &_mask);

        int getAutoDisable() const;

        void setAutoDisable(int _autoDisable);

        bool isConnected() const;

        void setConnected(bool _connected);

        //toString
        //friend ostream &operator<<(ostream &os, const AP &ap);
        String toString() const;

        String serialized() const;

    private:
        String ssid;
        String password;
        bool hide;
        String ip;
        String mask;
        int autoDisable;
        bool connected;
    };

    class STA {
    public:
        // constructor
        STA();

        STA(const String &ssid);

        STA(const String &ssid, const String &password);

        // destructor
        ~STA();

        // getters and setters
        String getSSID() const;

        void setSSID(const String &_ssid);

        String getPassword() const;

        void setPassword(const String &_password);

        bool isUseDHCP() const;

        void setUseDHCP(bool _useDHCP);

        String getIp() const;

        void setIp(const String &_ip);

        String getMask() const;

        void setMask(const String &_mask);

        String getDNSServer() const;

        void setDNSServer(const String &DNS_Server);

        String getGateway() const;

        void setGateway(const String &_gateway);

        int getAutoDisable() const;

        void setAutoDisable(int _autoDisable);

        bool isConnected() const;

        void setConnected(bool _connected);

        //toString
        //friend ostream &operator<<(ostream &os, const STA &sta);
        String toString() const;

        String serialized() const;

    private:
        String ssid;
        String password;
        bool useDHCP;
        String ip;
        String mask;
        String DNSServer;
        String gateway;
        int autoDisable;
        bool connected;
    };

    class WifiAll {
    public:
        // constructor
        explicit WifiAll(const String &hostname = ("ESP_" + WiFi.macAddress()),
                const String &mdnsName = ("ESP_" + WiFi.macAddress()));

        WifiAll(const String &hostname, const String &mdnsName, const AP& ap);

        WifiAll(const String &hostname, const String &mdnsName, const AP& ap, bool resetWifi);

        WifiAll(const String &hostname, const String &mdnsName, const AP& ap, WiFiMode_t mode, bool resetWifi);

        WifiAll(const String &hostname, const String &mdnsName, std::vector <STA> &allSTA);

        WifiAll(const String &hostname, const String &mdnsName, std::vector <STA> &allSTA, bool resetWifi);

        WifiAll(const String &hostname, const String &mdnsName, std::vector <STA> &allSTA, WiFiMode_t mode,
                bool resetWifi);

        WifiAll(const String &hostname, const String &mdnsName, const AP& ap, std::vector <STA> &allSTA);

        WifiAll(const String &hostname, const String &mdnsName, const AP& ap, std::vector <STA> &allSTA,
                WiFiMode_t mode);

        WifiAll(const String &hostname, const String &mdnsName, const AP& ap, std::vector <STA> &allSTA, bool resetWifi);

        WifiAll(const String &hostname, const String &mdnsName, const AP& ap, std::vector <STA> &allSTA, WiFiMode_t mode,
                bool resetWifi);

        // destructor
        ~WifiAll();

        // getters and setters
        String getHostname() const;

        void setHostname(const String &_hostname);

        String getMdnsName() const;

        void setMdnsName(const String &_mdnsName);

        WiFiMode_t getMode() const;

        void setMode(WiFiMode_t _mode);

        AP *getAP();

        void setAP(const AP& _ap);

        std::vector <STA> *getAllSTA();

        void setAllSTA(std::vector <STA> &allSTA);

        // bool removeAllSTA();
        // STA &getOneSTA(int indice);
        void addSTA(const STA &sta);

        void resetSTA();

        // STA &setOneSTA(int indice, STA &sta);
        // bool removeOneSTA(int indice);
        // T* STAtoArray();
        bool isResetWifi() const;

        void setResetWifi(bool _resetWifi);

        // toString
        //friend ostream &operator<<(ostream &os, WifiAll &all);
        String toString() const;

        String serialized() const;

        String serializedForSend() const;

    private:
        // variables
        String hostname; // name which is displayed on router
        String mdnsName; // Domain name for the mDNS responder
        WiFiMode_t mode; // from ESP8266WiFiType.h : WIFI_OFF = 0, WIFI_STA = 1, WIFI_AP = 2, WIFI_AP_STA = 3
        //                          // these two pseudo modes are experimental: WIFI_SHUTDOWN = 4, WIFI_RESUME = 8
        AP *ap;
        std::vector <STA> *allSTA;
        bool resetWifi;
    };
}
#endif // WifiAll_h_included
