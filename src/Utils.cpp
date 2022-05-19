//
// Created by antbd on 14/05/2022.
//

#include "Utils.h"

namespace WiFi_Portail_API {
    UtilsClass Utils;

    void UtilsClass::restartESP() {
        ESP.restart();
    }


    /// --------------------------- WatchDog ---------------------------
    void UtilsClass::beginWatchdog(unsigned long delay_ms) {
        this->WD_StartTime = millis();
        this->WD_REBOOT_DELAY_MS = delay_ms;
#if defined(ARDUINO_ARCH_ESP32)
        enableLoopWDT(); // default appears to be 5sec
#elif defined(ARDUINO_ARCH_ESP8266)
        ESP.wdtEnable(5000); // arg ignored :-( default appears to be 3sec
#else
#error Only ESP2866 and ESP32 reboot supported
#endif

    }

    void UtilsClass::loopWatchdog() {
        if ((millis() - this->WD_StartTime) >= this->WD_REBOOT_DELAY_MS) {
            while (1) {} // force watch dog timer reboot
        }

    }

    /// ----------------------------------------------------------------

}