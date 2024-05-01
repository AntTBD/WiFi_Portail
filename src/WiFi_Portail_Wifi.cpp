//
// Created by antbd on 04/06/2021.
//

#include <functional>

#include "WiFi_Portail_Wifi.h"
namespace WiFi_Portail_API {

    WifiManagerClass WifiManager;

    WifiManagerClass::WifiManagerClass()
    {

        this->wifiAll = new WifiAll();
        this->_connectTimeoutMs = 10000;

        this->_wifiDisconnectHandler = WiFi.onStationModeDisconnected(
                //onWifiDisconnect
                std::bind(&WifiManagerClass::onWifiDisconnect, this, std::placeholders::_1));
        this->_wifiConnectHandler = WiFi.onStationModeConnected(
                //onWifiConnect
                std::bind(&WifiManagerClass::onWifiConnect, this, std::placeholders::_1));
        this->_wifiOnStationModeGotIPHandler = WiFi.onStationModeGotIP(
                //onWifiGotIP
                std::bind(&WifiManagerClass::onWifiGotIP, this, std::placeholders::_1));
    }

    WifiManagerClass::~WifiManagerClass() {
        //delete this->_filesManager;
        delete this->wifiAll;

        /*delete this->_wifiMulti;
        delete this->_wifiDisconnectHandler;
        delete this->_wifiConnectHandler;
        delete this->_wifiOnStationModeGotIPHandler;*/
    }

    void WifiManagerClass::begin(WiFiMode_t startMode, bool resetWifi) {
        SerialDebug_println(F("====== Setup WiFi ======"));

        this->wifiAll->setMode(startMode);
        this->wifiAll->setResetWifi(resetWifi);

        this->disableWifi();
        WiFi.persistent(false);

        //  WiFi.setSleepMode(WIFI_NONE_SLEEP);
        //  SerialDebug_println(F("WIFI_NONE_SLEEP"));
        if (this->wifiAll->getMode() == WIFI_STA) {
            SerialDebug_println(F("STA: Start..."));
            if (this->connectSTA(this->wifiAll->getAllSTA(), this->wifiAll->getHostname(), this->_connectTimeoutMs) !=
                true) {
                SerialDebug_println(F("STA: Failed !"));
                delay(1000);
                SerialDebug_println(F("AP: Start..."));
                this->startAP(this->wifiAll->getAP());
            } else {
                SerialDebug_println(F("STA: Started Good !"));
            }
        } else if (this->wifiAll->getMode() == WIFI_AP) {
            SerialDebug_println(F("AP: Start..."));
            this->startAP(this->wifiAll->getAP());
        } else {
            this->disableWifi();
        }

        this->infosWifi();
        SerialDebug_println(F("========================"));
    }

    void WifiManagerClass::onWifiConnect(const WiFiEventStationModeConnected &event) {
        this->_wifiFlag = true;
        SerialDebug_printf(F("\n[ INFO ] WiFi STA Connected to %s\n"), event.ssid.c_str());
    }

    void WifiManagerClass::onWifiDisconnect(const WiFiEventStationModeDisconnected &event) {
        //mqttReconnectTimer.detach();
        if (this->_wifiFlag) {
            SerialDebug_printf(F("[ INFO ] WiFi STA Disconnected from SSID : %s\n"), event.ssid.c_str());
            SerialDebug_printf(F("Reason: %d\n"), event.reason);
            this->_wifiFlag = false;
            ///        SerialDebug_println(F("[ INFO ] WiFi try to reconnect"));
            ///        this->begin();
        }
    }

    void WifiManagerClass::onWifiGotIP(const WiFiEventStationModeGotIP &event) {
        this->_wifiFlag = true;
        SerialDebug_println(F("\n[ INFO ] WiFi IP Connected"));

        SerialDebug_printf(F("Got IP: %s\n"), event.ip.toString().c_str());
        SerialDebug_printf(F("Connected: %s\n"), (WiFi.status() == WL_CONNECTED ? F("yes") : F("no")));

        //connectToMqtt();
        //writeEvent("INFO", "wifi", "Connected with IP", "onWifiGotIP");
    }

    bool WifiManagerClass::startAP(AP *ap) {
        this->_inAPMode = true;
        SerialDebug_println(F("[ INFO ] Configuring access point... "));

        if (this->wifiAll->getAP()->getSSID() != F("")) {
            ap->setConnected(false);

            this->disableWifi();
            WiFi.enableAP(true);
            WiFi.mode(WIFI_AP);
            delay(500);

            // set custome IP and subnet
            if (ap->getIp() != F("") && ap->getMask() != F("")) {
                IPAddress myIp;
                IPAddress mySubnet;
                myIp.fromString(ap->getIp());
                mySubnet.fromString(ap->getMask());
                WiFi.softAPConfig(myIp, myIp, mySubnet);
            }

            // create Access Point
            this->_isWifiConnected = WiFi.softAP(ap->getSSID().c_str(),
                                                 (ap->getPassword() == "" ? NULL : ap->getPassword().c_str()),
                                                 (ap->isHide() ? 3 : 1), (ap->isHide() ? 1
                                                                                       : 0)); // 3rd param = channel : 3 when hide else default (1) / 4th param = hide or not
            ap->setConnected(this->_isWifiConnected);
            SerialDebug_println(this->_isWifiConnected ? F("Ready") : F("Failed! => restarting..."));
            if (!this->_isWifiConnected) {
                Utils.restartESP();
            }
        } else {
            SerialDebug_println(F("Access point not configured ! "));
        }

        return this->_isWifiConnected;
    }

    // Fallback to AP Mode, so we can connect to ESP if there is no Internet connection
    void WifiManagerClass::fallbacktoAPMode() {
        this->_inAPMode = true;
        SerialDebug_println(F("[ INFO ] ESP is running in Fallback AP Mode"));

        uint8_t macAddr[6];
        WiFi.softAPmacAddress(macAddr);
        char ssid[15];
        sprintf(ssid, "ESP-%02x%02x%02x", macAddr[3], macAddr[4], macAddr[5]);
        WiFi.enableAP(true);
        WiFi.mode(WIFI_AP);
        this->_isWifiConnected = WiFi.softAP(ssid);
        this->infosWifi();
    }

    // WiFi.config(clientip, gateway, subnet, dns);
    // Try to connect Wi-Fi
    bool WifiManagerClass::connectSTA(std::vector <STA> *allSTA, String hostname, uint32_t connectionTimeoutMUltiWifi) {
        SerialDebug_println(F("[ INFO ] Trying to connect WiFi: "));

        /*if(clientipChar != "" && subnetChar != "" && gatewayChar != "") {
          IPAddress clientip;
          IPAddress subnet;
          IPAddress gateway;
          clientip.fromString(clientipChar);
          subnet.fromString(subnetChar);
          gateway.fromString(gatewayChar);
          if(dnsChar != ""){
            IPAddress dns;
            dns.fromString(dnsChar);
            WiFi.config(clientip, gateway, subnet, dns);
          } else {
            WiFi.config(clientip, gateway, subnet);
          }
        }*/

        this->_inAPMode = false;

        this->_wifiMulti.cleanAPlist();

        WiFi.persistent(
                false); //  Disable the WiFi persistence to avoid any re-configuration that may erase static lease when starting softAP / STA
        this->disableWifi();
        WiFi.enableSTA(true);
        WiFi.mode(WIFI_STA); // Set WiFi to station mode

        // set hostnanme : name which is displayed on router
        if (hostname != F(""))
            WiFi.hostname(hostname);

        // Register multi WiFi networks
        for (STA &oneSTA: *allSTA) {
            oneSTA.setConnected(false);
            this->_wifiMulti.addAP(oneSTA.getSSID().c_str(), oneSTA.getPassword().c_str());
            bool added = this->_wifiMulti.existsAP(oneSTA.getSSID().c_str(), oneSTA.getPassword().c_str());
            SerialDebug_print(F("[ INFO ] '"));
            SerialDebug_print(oneSTA.getSSID());
            SerialDebug_print(F("' "));
            SerialDebug_print(added ? F("added") : F("failed to add"));
            SerialDebug_println(F(" to the list of wifiMulti."));
        }

        SerialDebug_println(F("Wait for the end of the connection time."));
        SerialDebug_println(F("Trying to connect to the best wifi signal (RSSI)..."));

        auto startTime = millis();
        while (this->_wifiMulti.run(connectionTimeoutMUltiWifi) != WL_CONNECTED &&
               (millis() - startTime) <= connectionTimeoutMUltiWifi) {
            SerialDebug_print(F("#"));
            //yield();
            delay(1000);
        }
        if (WiFi.status() == WL_CONNECTED) {
            this->_isWifiConnected = true;
            for (STA &oneSTA: *allSTA) {
                if (oneSTA.getSSID() == WiFi.SSID())
                    oneSTA.setConnected(true);
            }
            //String data = ssid;
            //data += " " + WiFi.localIP().toString();
            //writeEvent("INFO", "wifi", "WiFi is connected", data);
            return true;
        } else {
            // We couln't connect, time is out, inform
            SerialDebug_println();
            SerialDebug_println(F("[ WARN ] Couldn't connect in time (STA mode)"));
            return false;
        }
    }

    void WifiManagerClass::infosWifi() {
        String string;
        string.reserve(500);
        string += F("------------ WIFI INFO ---------------");
        string += F("\n");
        string += F("WiFi status :            ");
        string += this->wifiStatusToString();
        string += F("\n");

        switch (WiFi.getMode()) {
            case WIFI_OFF: // = 0
                string += F("WiFi mode :              OFF");
                string += F("\n");
                break;
            case WIFI_STA: // = 1
                string += F("WiFi mode :              STA");
                string += F("\n");
                string += F("Hostname:                ");
                string += WiFi.hostname();
                string += F("\n");
                string += F("Connected to:            ");
                string += WiFi.SSID(); // Tell us what network we're connected to
                string += F("\n");
                //string += F("WiFi password:           ");
                //string += WiFi.psk(); // Print the password used to connect ESP8266
                //string += F("\n");
                string += F("IP address:              ");
                string += WiFi.localIP().toString(); // Print the IP address
                string += F("\n");
                string += F("MAC address:             ");
                string += WiFi.macAddress(); // Print the MAC address
                string += F("\n");
                string += F("Channel:                 ");
                string += WiFi.channel(); // Print the channel of wifi
                string += F("\n");
                string += F("Signal strength (RSSI):  ");
                string += WiFi.RSSI();
                string += F(" dBm"); // Print the received signal strength
                string += F("\n");
                break;
            case WIFI_AP: // = 2
                string += F("WiFi mode :              AP");
                string += F("\n");
                string += F("AP Name:                 ");
                string += WiFi.softAPSSID(); // Tell us what network we're connected to
                string += F("\n");
                //string += F("AP password:             ");
                //string += WiFi.softAPPSK(); // Print the password used to connect ESP8266
                //string += F("\n");
                string += F("AP IP address:           ");
                string += WiFi.softAPIP().toString(); // Print the IP address
                string += F("\n");
                string += F("AP MAC address:          ");
                string += WiFi.softAPmacAddress(); // Print the MAC address
                string += F("\n");
                string += F("AP Channel:              ");
                string += WiFi.channel(); // Print the channel of wifi
                string += F("\n");
                break;
            case WIFI_AP_STA: // = 3
                string += F("Wifi Mode :              AP & STA\n");
                string += F("AP Name:                 ");
                string += WiFi.softAPSSID(); // Tell us what network we're connected to
                string += F("\n");
                //string += F("AP password:             ");
                //string += WiFi.softAPPSK(); // Print the password used to connect ESP8266
                //string += F("\n");
                string += F("AP IP address:           ");
                string += WiFi.softAPIP().toString(); // Print the IP address
                string += F("\n");
                string += F("AP MAC address:          ");
                string += WiFi.softAPmacAddress(); // Print the MAC address
                string += F("\n");
                string += F("AP Channel:              ");
                string += WiFi.channel(); // Print the channel of wifi
                string += F("\n");
                string += F("                -------------------\n");
                string += F("Hostname:                ");
                string += WiFi.hostname();
                string += F("\n");
                string += F("Connected to:            ");
                string += WiFi.SSID(); // Tell us what network we're connected to
                string += F("\n");
                //string += F("WiFi password:           ");
                //string += WiFi.psk(); // Print the password used to connect ESP8266
                //string += F("\n");
                string += F("IP address:              ");
                string += WiFi.localIP().toString(); // Print the IP address
                string += F("\n");
                string += F("MAC address:             ");
                string += WiFi.macAddress(); // Print the MAC address
                string += F("\n");
                string += F("Channel:                 ");
                string += WiFi.channel(); // Print the channel of wifi
                string += F("\n");
                string += F("Signal strength (RSSI):  ");
                string += WiFi.RSSI();
                string += F(" dBm");
                string += F("\n");
                break;
            default:
                string += F("Mode none recognised : ");
                string += WiFi.getMode();
                string += F("\n");
                break;
        }
        string += F("--------------------------------------");
        string += F("\n");
        SerialDebug_println(string);
    }

    String WifiManagerClass::wifiStatusToString() const {
        String status = F("");
        switch (WiFi.status()) {
            case WL_NO_SHIELD: // = 255   // for compatibility with WiFi Shield library
                status = F("WL_NO_SHIELD");
                break;
            case WL_IDLE_STATUS: // = 0
                status = F("WL_IDLE_STATUS");
                break;
            case WL_NO_SSID_AVAIL: // = 1
                status = F("WL_NO_SSID_AVAIL");
                break;
            case WL_SCAN_COMPLETED: // = 2
                status = F("WL_SCAN_COMPLETED");
                break;
            case WL_CONNECTED: // = 3
                status = F("WL_CONNECTED");
                break;
            case WL_CONNECT_FAILED: // = 4
                status = F("WL_CONNECT_FAILED");
                break;
            case WL_CONNECTION_LOST: // = 5
                status = F("WL_CONNECTION_LOST");
                break;
            case WL_DISCONNECTED: // = 6
                status = F("WL_DISCONNECTED");
                break;
            default:
                status = F("???");
                break;
        }
        return status;
    }

/// Convert all infos to JSON sorted by devices datas and network datas
    DynamicJsonDocument WifiManagerClass::getNetworkInfosJson() {
        //    SerialDebug_println("tata");
        //SerialDebug_println(this->wifiAll->toString().c_str());
        String serializedWifiAll = this->wifiAll->serializedForSend();
        //SerialDebug_println(serializedWifiAll.length() + " " + serializedWifiAll);

        DynamicJsonDocument doc(
                /*JSON_OBJECT_SIZE(2) +
                serializedWifiAll.length() +
                //strlen(serializedWifiAll.c_str())
                100
        */
                JSON_OBJECT_SIZE(6) +
                JSON_OBJECT_SIZE(6) +
                JSON_ARRAY_SIZE(1) +
                10 * (JSON_OBJECT_SIZE(8) + 25));
        DeserializationError error = deserializeJson(doc, serializedWifiAll);

        if (error) {
            SerialDebug_print(F("deserializeJson() failed: "));
            SerialDebug_println(error.f_str());
        }
        serializedWifiAll = String();

        //serializeJson(doc, Serial);

        return doc;
    }

/// Convert all infos to JSON and then to string
    String WifiManagerClass::getNetworkInfosString() {
        String jsonStringWifiInfos;
        jsonStringWifiInfos.reserve(2000);
        serializeJsonPretty(this->getNetworkInfosJson(),
                            jsonStringWifiInfos); // on remplie la string avec les infos de JSONInfo sous forme sérialisé
        //Serial.println(jsonStringWifiInfos);
        return jsonStringWifiInfos;
    }

//-----------------------------------------------------------
    void WifiManagerClass::disableWifi() {
    SerialDebug_println(F("Turn wifi off."));
        this->_isWifiConnected = false;
        this->_inAPMode = false;
        WiFi.disconnect(true);
        WiFi.enableSTA(false);
        WiFi.enableAP(false);
        delay(500);
        WiFi.mode(WIFI_OFF);
    }

    void WifiManagerClass::enableWifi() {
        SerialDebug_println(F("[ INFO ] Restarting the board to connect wi-fi again"));
        Utils.restartESP();
    }

    void WifiManagerClass::resetWifi() {
        this->wifiAll->setResetWifi(true);
        /// TODO : found other way to do the same save
        //FilesManager.saveConfigAllWifi();

        Utils.restartESP();
    }
}