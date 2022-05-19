//
// Created by antbd on 06/09/2021.
//

#include "TBD_WiFi_Portail_OTA.h"

#ifdef USE_OTA

namespace WiFi_Portail_API {

    OTAManagerClass OTAManager;

    OTAManagerClass::OTAManagerClass() {

        this->_hostname = this->_password = F("");
#if defined(ESP8266)
        this->led_pin = D4;
#else
        this->led_pin = -1;
#endif
        this->_webEvents = nullptr;
        this->setArduinoOtaUseMDNS(true);
    }

    OTAManagerClass::~OTAManagerClass() {
        delete this->_webEvents;

    }

    // Add web events to have ability to informe client during ota update
    void OTAManagerClass::addWebEvents(WebEvents &webEvents) {
        this->_webEvents = &webEvents;
    }

    void OTAManagerClass::setHostname(const String &hostname) {
#ifdef USE_MDNS
        MDNSManager.setMdnsName(hostname); // on donne une petit nom a notre module
#endif // USE_MDNS
        this->_hostname = hostname;
    }

    void OTAManagerClass::setPassword(const String &password) {
        this->_password = password;
    }

    String OTAManagerClass::getHostname() const {
        return this->_hostname;
    }

    String OTAManagerClass::getPassword() const {
        return this->_password;
    }

    void OTAManagerClass::setArduinoOtaUseMDNS(bool arduinoOtaUseMDNS) {
        this->_arduinoOtaUseMDNS = arduinoOtaUseMDNS;

    }

    bool OTAManagerClass::getArduinoOtaUseMDNS() const {
        return this->_arduinoOtaUseMDNS;
    }

    String OTAManagerClass::toString() const {
        String str;
        str += F("=========== OTA ===========");
        str += F("\n");
        str += F("Hostname:      ");
        str += this->getHostname();
        str += F("            !!!! used mDNS hostname !!!!");
        str += F("\n");
        str += F("Password:      ");
        str += this->getPassword();
        str += F("\n");
        str += F("===========================");

        return str;
    }

// ------------------------------------ setup OTA ----------------------------------------
    void OTAManagerClass::begin(String hostname, String password) {

        SerialDebug_println(F("=== Setup OTA ==="));
        this->setHostname(hostname);
        this->setPassword(password);

        SerialDebug_println(this->toString());

        pinMode(this->led_pin, OUTPUT);    // Initialise la broche "led" comme une sortie
        digitalWrite(this->led_pin, HIGH); // on eteind la led

        // Port defaults to 8266
        //ArduinoOTA.setPort(3232);

        // Hostname defaults to esp8266-[ChipID]
        // here : use mDNS name

#ifdef USE_MDNS
        ArduinoOTA.setHostname(MDNSManager.getMdnsName().c_str()); // on donne une petit nom a notre module
#else
        ArduinoOTA.setHostname(this->_hostname.c_str()); // on donne une petit nom a notre module
#endif // USE_MDNS

        // Authentication
        ArduinoOTA.setPassword(
                this->_password.c_str()); //IDE Arduino nous demandera de saisir le mot de passe à chaque fois que vous téléverserez un programme.

        // allumage led
        ArduinoOTA.onStart(
                [&]() { this->arduinoOTAOnStart(); });
        ArduinoOTA.onEnd(
                [&]() { this->arduinoOTAOnEnd(); });
        ArduinoOTA.onProgress(
                [&](unsigned int progress, unsigned int total) { this->arduinoOTAOnProgress(progress, total); });
        ArduinoOTA.onError(
                [&](ota_error_t error) { this->arduinoOTAOnError(error); });

        ArduinoOTA.begin(this->_arduinoOtaUseMDNS); // initialize OTA

        SerialDebug_println(F(" -> OTA ready"));
        SerialDebug_println(
                F("/!\\ Be shure to be in the same network for ota update (computer self-sharedWifi not works !!!!)"));
        SerialDebug_println(F("================="));
    }

// ---------------------------------------- loop OTA -------------------------------------------
    void OTAManagerClass::loop() {
#ifdef USE_MDNS
        if (MDNSManager.isUpdatingMDNSInThisClass() == this->_arduinoOtaUseMDNS) {// ensure to not update mDNS if we use it with ota
            MDNSManager.setUpdatingMDNSInThisClass(!this->_arduinoOtaUseMDNS);
        }
#endif // USE_MDNS
        // a chaque iteration, on verifie si une mise a jour du sketch nous est envoyee
        // si tel est le cas, la lib ArduinoOTA se charge de gerer la suite :)
        ArduinoOTA.handle();
        yield();
    }

    void OTAManagerClass::rebootESP8266() {
        if (this->_webEvents != nullptr)
            this->_webEvents->debug_to_WSEvents(F("Soft reboot via ESP.restart()..."));
        FileSystem.filesytem_end();
        //close_WebSocket();
        this->reset_all_pins();
        //disconnect_Wifi();
        delay(1000);

        Utils.restartESP();
    }

    void OTAManagerClass::reset_all_pins() {

        digitalWrite(D1, LOW);
        digitalWrite(D2, LOW);
        //digitalWrite(D3, LOW);//
        //digitalWrite(D4, LOW);//
        digitalWrite(D5, LOW);
        digitalWrite(D6, LOW);
        digitalWrite(D7, LOW);
        //digitalWrite(D8, LOW);//

        // switch off all the PWMs during upgrade (src : ArduinoOTA/Exemples/OTALed.ino)
        int dimmer_pin[] = {14, 5, 15};
        for (int i = 0; i < 3; i++) {
            analogWrite(dimmer_pin[i], 0);
        }
    }

    void OTAManagerClass::arduinoOTAOnStart() {
        SerialDebug_println(F("Be shure to be in the same network for ota update (computer self-sharedWifi not works !!!!)"));
        if (this->_webEvents != nullptr)
            this->_webEvents->debug_to_WSEvents(
                    F("Be shure to be in the same network for ota update <b>(computer self-sharedWifi not works !!!!)</b>"),
                    F("WARN"));

        String type = "";
        if (ArduinoOTA.getCommand() == U_FLASH) {
            type = F("sketch");
        } else { // U_SPIFFS
            type = F("filesystem");
        }
        // NOTE: if updating fileSystem this would be the place to unmount fileSystem using fileSystem.end()
        FileSystem.filesytem_end();
        //close_WebSocket();
        this->reset_all_pins();
        delay(1000);

        digitalWrite(this->led_pin, LOW); // allume led au début du transfert
        //Serial.println("Start updating " + type);
        //TBD
        String msg = F("* OTA: Update Start (type=");
        msg += type;
        msg += F(")");
        if (this->_webEvents != nullptr)
            this->_webEvents->debug_to_WSEvents(msg, F("ota"));
        //fin TBD
    }

    void OTAManagerClass::arduinoOTAOnEnd() {
        digitalWrite(this->led_pin, HIGH); // éteint à la fin de la mise à jour
        //Serial.println(F("\n* OTA: End"));
        //events.send("* OTA: Update End", F("ota"));//TBD
        char p[50];
        sprintf(p, "* OTA: free space: %u / total: %u\n", ESP.getFreeSketchSpace(),
                (ESP.getSketchSize() + ESP.getFreeSketchSpace()));
        //events.send(p, F("ota"));
        if (this->_webEvents != nullptr) {
            this->_webEvents->debug_to_WSEvents(p, F("ota"));
            this->_webEvents->debug_to_WSEvents(F("* OTA: Update End"), F("ota"));
        }
        this->rebootESP8266();
    }

    void OTAManagerClass::arduinoOTAOnProgress(unsigned int progress, unsigned int total) {
        //Serial.printf("* OTA: Progress: %u%%\r", (progress / (total / 100)));

        //digitalWrite(led_pin_OTA, (millis() / 500) % 2);
        digitalWrite(this->led_pin, !digitalRead(this->led_pin));
        // envoi des infos toutes les x millis secondes
        if (millis() - this->previousMillisSendInfos >= this->intervalSendInfos) {
            this->previousMillisSendInfos = millis();
            char p[80];
            sprintf(p, "* OTA: Progress: %u%% | free space: %u / total: %u\n", (progress / (total / 100)),
                    ESP.getFreeSketchSpace(), (ESP.getSketchSize() + ESP.getFreeSketchSpace()));
            //events.send(p, "ota");
            if (this->_webEvents != nullptr)
                this->_webEvents->debug_to_WSEvents(p, F("ota"));
        }
    }

    void OTAManagerClass::arduinoOTAOnError(ota_error_t error) {
        if (this->_webEvents != nullptr) {
            //Serial.printf("* OTA: Error[%u]: ", error);
            //this->_webEvents->debug_to_WSEvents((String)F("* OTA: Error[")+error+(String)F("]"), F("ota"));
            if (error == OTA_AUTH_ERROR) {
                this->_webEvents->debug_to_WSEvents(F("Auth Failed"), F("ota"));
                //Serial.println("Auth Failed");
                // events.send("Auth Failed", "ota");
            } else if (error == OTA_BEGIN_ERROR) {
                this->_webEvents->debug_to_WSEvents(F("Begin Failed"), F("ota"));
                //Serial.println("Begin Failed");
                // events.send("Begin Failed", "ota");
            } else if (error == OTA_CONNECT_ERROR) {
                this->_webEvents->debug_to_WSEvents(F("Connect Failed"), F("ota"));
                //Serial.println("Connect Failed");
                // events.send("Connect Failed", "ota");
            } else if (error == OTA_RECEIVE_ERROR) {
                this->_webEvents->debug_to_WSEvents(F("Receive Failed"), F("ota"));
                //Serial.println("Receive Failed");
                // events.send("Recieve Failed", "ota");
            } else if (error == OTA_END_ERROR) {
                this->_webEvents->debug_to_WSEvents(F("End Failed"), F("ota"));
                //Serial.println("End Failed");
                // events.send("End Failed", "ota");
            }

            //Serial.println(F("* OTA: ESP Restart"));
            //events.send("Ota Error => ESP Restart", F("ota"));//TBD
            this->_webEvents->debug_to_WSEvents(F("* OTA: ESP Restart"), F("ota"));
        }
        this->rebootESP8266();
    }
}
#endif // USE_OTA