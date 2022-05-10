//
// Created by antbd on 06/09/2021.
//

#include "TBD_WiFi_Portail_OTA.h"

#ifdef USE_OTA

namespace WiFi_Portail_API {

    OTA::OTA(SerialDebug &serialDebug, FileSystem &fileSystem, String hostname, String password) :
            _serialDebug(&serialDebug), _fileSystem(&fileSystem), _password(password) {

        this->setHostname(hostname);
#if defined(ESP8266)
        this->led_pin = D4;
#else
        this->led_pin = -1;
#endif
        //this->_arduinoOTA = new ArduinoOTAClass();
        this->_webEvents = nullptr;
        this->_mdns = nullptr;
        this->_arduinoOtaUseMDNS = true;
    }

    OTA::~OTA() {
        delete this->_serialDebug;
        delete this->_fileSystem;
        delete this->_webEvents;
        delete this->_mdns;

        //delete this->_arduinoOTA;
    }

    void OTA::addWebEvents(WebEvents &webEvents) {
        this->_webEvents = &webEvents;
    }

    void OTA::addMDNS(MDNSManager &mdns) {
        this->_mdns = &mdns;
    }

    void OTA::setHostname(const String &hostname) {
        if (this->_mdns != nullptr) {
            this->_mdns->setMdnsName(hostname); // on donne une petit nom a notre module
        }
        this->_hostname = hostname;
    }

    void OTA::setPassword(const String &password) {
        this->_password = password;
    }

    String OTA::getHostname() const {
        return this->_hostname;
    }

    String OTA::getPassword() const {
        return this->_password;
    }

    void OTA::setArduinoOtaUseMDNS(bool arduinoOtaUseMDNS) {
        this->_arduinoOtaUseMDNS = arduinoOtaUseMDNS;
    }

    bool OTA::getArduinoOtaUseMDNS() const {
        return this->_arduinoOtaUseMDNS;
    }

    String OTA::toString() const {
        return (String) F("=========== OTA ===========") + F("\n") + F("Hostname :      ") + this->getHostname() +
               F("            !!!! used mDNS hostname !!!!") + F("\n") + F("Password :      ") + this->getPassword() +
               F("\n") + F("===========================");
    }

// ------------------------------------ setup OTA ----------------------------------------
    void OTA::begin() {

        this->_serialDebug->println(F("=== Setup OTA ==="));

        this->_serialDebug->println(this->toString());

        pinMode(this->led_pin, OUTPUT);    // Initialise la broche "led" comme une sortie
        digitalWrite(this->led_pin, HIGH); // on eteind la led

        // Port defaults to 8266
        //ArduinoOTA.setPort(3232);

        // Hostname defaults to esp8266-[ChipID]
        // here : use mDNS name
        if (this->_mdns != nullptr) {
            ArduinoOTA.setHostname(this->_mdns->getMdnsName().c_str()); // on donne une petit nom a notre module
        } else {
            ArduinoOTA.setHostname(this->_hostname.c_str()); // on donne une petit nom a notre module
        }
        // Authentication
        ArduinoOTA.setPassword(
                this->_password.c_str()); //IDE Arduino nous demandera de saisir le mot de passe à chaque fois que vous téléverserez un programme.
        //ArduinoOTA.setPassword("password_OTA");

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

        this->_serialDebug->println(F(" -> OTA ready"));
        this->_serialDebug->println(
                F("/!\\ Be shure to be in the same network for ota update (computer self-sharedWifi not works !!!!)"));
        this->_serialDebug->println(F("================="));
    }

// ---------------------------------------- loop OTA -------------------------------------------
    void OTA::loop() {
        // a chaque iteration, on verifie si une mise a jour du sketch nous est envoyee
        // si tel est le cas, la lib ArduinoOTA se charge de gerer la suite :)
        ArduinoOTA.handle();
        yield();
    }

    void OTA::rebootESP8266() {
        if (this->_webEvents != nullptr)
            this->_webEvents->debug_to_WSEvents(F("Soft reboot via ESP.restart()..."));
        this->_fileSystem->filesytem_end();
        //close_WebSocket();
        this->reset_all_pins();
        //disconnect_Wifi();
        delay(1000);

        restartESP();
    }

    void OTA::reset_all_pins() {

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

    void OTA::arduinoOTAOnStart() {
        this->_serialDebug->println(
                F("Be shure to be in the same network for ota update (computer self-sharedWifi not works !!!!)"));
        if (this->_webEvents != nullptr)
            this->_webEvents->debug_to_WSEvents(
                    F("Be shure to be in the same network for ota update (computer self-sharedWifi not works !!!!)"),
                    "ota");

        String type = "";
        if (ArduinoOTA.getCommand() == U_FLASH) {
            type = F("sketch");
        } else { // U_SPIFFS
            type = F("filesystem");
        }
        // NOTE: if updating fileSystem this would be the place to unmount fileSystem using fileSystem.end()
        this->_fileSystem->filesytem_end();
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
            this->_webEvents->debug_to_WSEvents(msg, "ota");
        //fin TBD
    }

    void OTA::arduinoOTAOnEnd() {
        digitalWrite(this->led_pin, HIGH); // éteint à la fin de la mise à jour
        //Serial.println(F("\n* OTA: End"));
        //events.send("* OTA: Update End", "ota");//TBD
        char p[50];
        sprintf(p, "* OTA: free space: %u / total: %u\n", ESP.getFreeSketchSpace(),
                (ESP.getSketchSize() + ESP.getFreeSketchSpace()));
        //events.send(p, "ota");
        if (this->_webEvents != nullptr) {
            this->_webEvents->debug_to_WSEvents(p, "ota");
            this->_webEvents->debug_to_WSEvents(F("* OTA: Update End"), "ota");
        }

        this->rebootESP8266();
    }

    void OTA::arduinoOTAOnProgress(unsigned int progress, unsigned int total) {
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
                this->_webEvents->debug_to_WSEvents(p, "ota");
        }
    }

    void OTA::arduinoOTAOnError(ota_error_t error) {
        if (this->_webEvents != nullptr) {
            //Serial.printf("* OTA: Error[%u]: ", error);
            //this->_webEvents->debug_to_WSEvents((String)F("* OTA: Error[")+error+(String)F("]"), "ota");
            if (error == OTA_AUTH_ERROR) {
                this->_webEvents->debug_to_WSEvents(F("Auth Failed"), "ota");
                //Serial.println("Auth Failed");
                // events.send("Auth Failed", "ota");
            } else if (error == OTA_BEGIN_ERROR) {
                this->_webEvents->debug_to_WSEvents(F("Begin Failed"), "ota");
                //Serial.println("Begin Failed");
                // events.send("Begin Failed", "ota");
            } else if (error == OTA_CONNECT_ERROR) {
                this->_webEvents->debug_to_WSEvents(F("Connect Failed"), "ota");
                //Serial.println("Connect Failed");
                // events.send("Connect Failed", "ota");
            } else if (error == OTA_RECEIVE_ERROR) {
                this->_webEvents->debug_to_WSEvents(F("Receive Failed"), "ota");
                //Serial.println("Receive Failed");
                // events.send("Recieve Failed", "ota");
            } else if (error == OTA_END_ERROR) {
                this->_webEvents->debug_to_WSEvents(F("End Failed"), "ota");
                //Serial.println("End Failed");
                // events.send("End Failed", "ota");
            }

            //Serial.println(F("* OTA: ESP Restart"));
            //events.send("Ota Error => ESP Restart", "ota");//TBD
            this->_webEvents->debug_to_WSEvents(F("* OTA: ESP Restart"), "ota");
        }
        this->rebootESP8266();
    }
}
#endif // USE_OTA