//
// Created by antbd on 14/05/2022.
//

#ifndef TBD_WIFI_PORTAIL_UTILS_H
#define TBD_WIFI_PORTAIL_UTILS_H

#include "WiFi_Portail.h"
#include <IPAddress.h>

namespace WiFi_Portail_API {
    class UtilsClass {
    private :
        unsigned long WD_StartTime;
        unsigned long WD_REBOOT_DELAY_MS;
    public:
        void beginWatchdog(unsigned long delay_ms = 24ul * 60 * 60 * 1000); // 1day in mS
        void loopWatchdog();

        void restartESP();
        String IpToString(IPAddress adress) const;
        String formatBytes(size_t bytes) const;
    };

    extern UtilsClass Utils;
}


#endif //TBD_WIFI_PORTAIL_UTILS_H
