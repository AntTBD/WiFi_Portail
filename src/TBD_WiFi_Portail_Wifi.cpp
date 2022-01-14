//
// Created by antbd on 04/06/2021.
//

#include "TBD_WiFi_Portail_Wifi.h"
namespace WiFi_Portail_API {

    WifiManager::WifiManager(SerialDebug &serialDebug /*, FilesManager& filesManager*/, WiFiMode_t startMode,
                             bool resetWifi) :
            _serialDebug(&serialDebug) /*, _filesManager(&filesManager)*/
    {

        this->wifiAll = new WifiAll();
        this->wifiAll->setMode(startMode);
        this->wifiAll->setResetWifi(resetWifi);
        this->_connectTimeoutMs = 10000;

        this->_wifiDisconnectHandler = WiFi.onStationModeDisconnected(
                //onWifiDisconnect
                std::bind(&WifiManager::onWifiDisconnect, this, std::placeholders::_1));
        this->_wifiConnectHandler = WiFi.onStationModeConnected(
                //onWifiConnect
                std::bind(&WifiManager::onWifiConnect, this, std::placeholders::_1));
        this->_wifiOnStationModeGotIPHandler = WiFi.onStationModeGotIP(
                //onWifiGotIP
                std::bind(&WifiManager::onWifiGotIP, this, std::placeholders::_1));
    }

    WifiManager::~WifiManager() {
        delete this->_serialDebug;
        //delete this->_filesManager;
        delete this->wifiAll;

        /*delete this->_wifiMulti;
        delete this->_wifiDisconnectHandler;
        delete this->_wifiConnectHandler;
        delete this->_wifiOnStationModeGotIPHandler;*/
    }

    void WifiManager::begin() {
        this->_serialDebug->println(F("====== Setup WiFi ======"));

        this->disableWifi();
        WiFi.persistent(false);

        //  WiFi.setSleepMode(WIFI_NONE_SLEEP);
        //  this->_serialDebug->println(F("WIFI_NONE_SLEEP"));
        if (this->wifiAll->getMode() == WIFI_STA) {
            this->_serialDebug->println(F("STA: Start..."));
            if (this->connectSTA(this->wifiAll->getAllSTA(), this->wifiAll->getHostname(), this->_connectTimeoutMs) !=
                true) {
                this->_serialDebug->println(F("STA: Failed !"));
                delay(1000);
                this->_serialDebug->println(F("AP: Start..."));
                this->startAP(this->wifiAll->getAP());
            } else {
                this->_serialDebug->println(F("STA: Started Good !"));
            }
        } else if (this->wifiAll->getMode() == WIFI_AP) {
            this->_serialDebug->println(F("AP: Start..."));
            this->startAP(this->wifiAll->getAP());
        } else {
            this->disableWifi();
        }

        this->infosWifi();
        this->_serialDebug->println(F("========================"));
    }

    void WifiManager::onWifiConnect(const WiFiEventStationModeConnected &event) {
        this->_wifiFlag = true;
        this->_serialDebug->printf(F("\n[ INFO ] WiFi STA Connected to %s\n"), event.ssid.c_str());
    }

    void WifiManager::onWifiDisconnect(const WiFiEventStationModeDisconnected &event) {
        //mqttReconnectTimer.detach();
        if (this->_wifiFlag) {
            this->_serialDebug->printf(F("[ INFO ] WiFi STA Disconnected from SSID : %s\n"), event.ssid.c_str());
            this->_serialDebug->printf(F("Reason: %d\n"), event.reason);
            this->_wifiFlag = false;
            ///        this->_serialDebug->println(F("[ INFO ] WiFi try to reconnect"));
            ///        this->begin();
        }
    }

    void WifiManager::onWifiGotIP(const WiFiEventStationModeGotIP &event) {
        this->_wifiFlag = true;
        this->_serialDebug->println(F("\n[ INFO ] WiFi IP Connected"));

        this->_serialDebug->printf(F("Got IP: %s\n"), event.ip.toString().c_str());
        this->_serialDebug->printf(F("Connected: %s\n"), (WiFi.status() == WL_CONNECTED ? F("yes") : F("no")));

        //connectToMqtt();
        //writeEvent("INFO", "wifi", "Connected with IP", "onWifiGotIP");
    }

    bool WifiManager::startAP(AP *ap) {
        this->_inAPMode = true;
        this->_serialDebug->println(F("[ INFO ] Configuring access point... "));

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
            this->_serialDebug->println(this->_isWifiConnected ? F("Ready") : F("Failed! => restarting..."));
            if (!this->_isWifiConnected) {
                restartESP();
            }
        } else {
            this->_serialDebug->println(F("Access point not configured ! "));
        }

        return this->_isWifiConnected;
    }

// Fallback to AP Mode, so we can connect to ESP if there is no Internet connection
    void WifiManager::fallbacktoAPMode() {
        this->_inAPMode = true;
        this->_serialDebug->println(F("[ INFO ] ESP is running in Fallback AP Mode"));

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
    bool WifiManager::connectSTA(std::vector <STA> *allSTA, String hostname, uint32_t connectionTimeoutMUltiWifi) {
        this->_serialDebug->println(F("[ INFO ] Trying to connect WiFi: "));

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
            this->_serialDebug->print(F("[ INFO ] '"));
            this->_serialDebug->print(oneSTA.getSSID());
            this->_serialDebug->print(F("' "));
            this->_serialDebug->print(added ? F("added") : F("failed to add"));
            this->_serialDebug->println(F(" to the list of wifiMulti."));
        }

        this->_serialDebug->println(F("Wait for the end of the connection time."));
        this->_serialDebug->println(F("Trying to connect to the best wifi signal (RSSI)..."));

        auto startTime = millis();
        while (this->_wifiMulti.run(connectionTimeoutMUltiWifi) != WL_CONNECTED &&
               (millis() - startTime) <= connectionTimeoutMUltiWifi) {
            this->_serialDebug->print(F("#"));
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
            this->_serialDebug->println();
            this->_serialDebug->println(F("[ WARN ] Couldn't connect in time (STA mode)"));
            return false;
        }
    }

    void WifiManager::infosWifi() {
        this->_serialDebug->println(F("------------ WIFI INFO ---------------"));
        this->_serialDebug->print(F("WiFi status :            "));
        this->_serialDebug->println(this->wifiStatusToString());

        switch (WiFi.getMode()) {
            case WIFI_OFF: // = 0
                this->_serialDebug->println(F("Wifi Mode :              OFF"));
                break;
            case WIFI_STA: // = 1
                this->_serialDebug->println(F("Wifi Mode :              STA"));
                this->_serialDebug->print(F("Hostname:                "));
                this->_serialDebug->println(WiFi.hostname());
                this->_serialDebug->print(F("Connected to:            "));
                this->_serialDebug->println(WiFi.SSID()); // Tell us what network we're connected to
                //this->_serialDebug->print(F("WiFi password:           ")); this->_serialDebug->println(WiFi.psk()); // Print the password used to connect ESP8266
                this->_serialDebug->print(F("IP address:              "));
                this->_serialDebug->println(WiFi.localIP().toString()); // Print the IP address
                this->_serialDebug->print(F("MAC address:             "));
                this->_serialDebug->println(WiFi.macAddress()); // Print the MAC address
                this->_serialDebug->print(F("Channel:                 "));
                this->_serialDebug->println(WiFi.channel()); // Print the channel of wifi
                this->_serialDebug->print(F("Signal strength (RSSI):  "));
                this->_serialDebug->print(WiFi.RSSI());
                this->_serialDebug->println(F(" dBm")); // Print the received signal strength
                break;
            case WIFI_AP: // = 2
                this->_serialDebug->println(F("Wifi Mode :              AP"));
                this->_serialDebug->print(F("AP Name:                 "));
                this->_serialDebug->println(WiFi.softAPSSID()); // Tell us what network we're connected to
                //this->_serialDebug->print(F("AP password:             ")); this->_serialDebug->println(WiFi.softAPPSK()); // Print the password used to connect ESP8266
                this->_serialDebug->print(F("AP IP address:           "));
                this->_serialDebug->println(WiFi.softAPIP().toString()); // Print the IP address
                this->_serialDebug->print(F("AP MAC address:          "));
                this->_serialDebug->println(WiFi.softAPmacAddress()); // Print the MAC address
                this->_serialDebug->print(F("AP Channel:              "));
                this->_serialDebug->println(WiFi.channel()); // Print the channel of wifi
                break;
            case WIFI_AP_STA: // = 3
                this->_serialDebug->println(F("Wifi Mode :              AP & STA"));
                this->_serialDebug->print(F("AP Name:                 "));
                this->_serialDebug->println(WiFi.softAPSSID()); // Tell us what network we're connected to
                //this->_serialDebug->print(F("AP password:             ")); this->_serialDebug->println(WiFi.softAPPSK()); // Print the password used to connect ESP8266
                this->_serialDebug->print(F("AP IP address:           "));
                this->_serialDebug->println(WiFi.softAPIP().toString()); // Print the IP address
                this->_serialDebug->print(F("AP MAC address:          "));
                this->_serialDebug->println(WiFi.softAPmacAddress()); // Print the MAC address
                this->_serialDebug->print(F("AP Channel:              "));
                this->_serialDebug->println(WiFi.channel()); // Print the channel of wifi
                this->_serialDebug->println(F("                -------------------"));
                this->_serialDebug->print(F("Hostname:                "));
                this->_serialDebug->println(WiFi.hostname());
                this->_serialDebug->print(F("Connected to:            "));
                this->_serialDebug->println(WiFi.SSID()); // Tell us what network we're connected to
                //this->_serialDebug->print(F("WiFi password:           ")); this->_serialDebug->println(WiFi.psk()); // Print the password used to connect ESP8266
                this->_serialDebug->print(F("IP address:              "));
                this->_serialDebug->println(WiFi.localIP().toString()); // Print the IP address
                this->_serialDebug->print(F("MAC address:             "));
                this->_serialDebug->println(WiFi.macAddress()); // Print the MAC address
                this->_serialDebug->print(F("Channel:                 "));
                this->_serialDebug->println(WiFi.channel()); // Print the channel of wifi
                this->_serialDebug->print(F("Signal strength (RSSI):  "));
                this->_serialDebug->print(WiFi.RSSI());
                this->_serialDebug->println(" dBm"); // Print the received signal strength
                break;
            default:
                this->_serialDebug->print(F("Mode none recognised : "));
                this->_serialDebug->println(WiFi.getMode());
                break;
        }
        this->_serialDebug->println(F("--------------------------------------"));
    }

    String WifiManager::wifiStatusToString() const {
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
    DynamicJsonDocument WifiManager::getNetworkInfosJson() {
        //    this->_serialDebug->println("tata");
        //this->_serialDebug->println(this->wifiAll->toString().c_str());
        String serializedWifiAll = this->wifiAll->serializedForSend();
        //this->_serialDebug->println(serializedWifiAll.length() + " " + serializedWifiAll);

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
            this->_serialDebug->print(F("deserializeJson() failed: "));
            this->_serialDebug->println(error.f_str());
        }
        serializedWifiAll = String();

        //serializeJson(doc, Serial);

        return doc;
    }

/// Convert all infos to JSON and then to string
    String WifiManager::getNetworkInfosString() {
        String jsonStringWifiInfos;
        jsonStringWifiInfos.reserve(2000);
        serializeJsonPretty(this->getNetworkInfosJson(),
                            jsonStringWifiInfos); // on remplie la string avec les infos de JSONInfo sous forme sérialisé
        //Serial.println(jsonStringWifiInfos);
        return jsonStringWifiInfos;
    }

//-----------------------------------------------------------
    void WifiManager::disableWifi() {
        this->_serialDebug->println(F("Turn wifi off."));
        this->_isWifiConnected = false;
        this->_inAPMode = false;
        WiFi.disconnect(true);
        WiFi.enableSTA(false);
        WiFi.enableAP(false);
        delay(500);
        WiFi.mode(WIFI_OFF);
    }

    void WifiManager::enableWifi() {
        this->_serialDebug->println(F("[ INFO ] Restarting the board to connect wi-fi again"));
        restartESP();
    }

    void WifiManager::resetWifi() {
        this->wifiAll->setResetWifi(true);
        /// TODO : found other way to do the same save
        //this->_filesManager->saveConfigAllWifi();

        restartESP();
    }
}