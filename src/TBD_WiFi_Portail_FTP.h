//
// Created by antbd on 05/06/2021.
//

#ifndef TBD_WIFI_PORTAIL_FTP_H
#define TBD_WIFI_PORTAIL_FTP_H

#include "TBD_WiFi_Portail.h"
#ifdef USE_FTP
#include "TBD_WiFi_Portail_Wifi.h"

#include <FTPServer.h> //https://github.com/dplasa/FTPClientServer

namespace WiFi_Portail_API {


    class FTPManagerClass {
    public:
        FTPManagerClass(String user = "user", String password = "password");

        ~FTPManagerClass();

        void begin();

        void loop();

        void setUser(const String &user);

        void setPassword(const String &password);

        String getUser() const;

        String getPassword() const;

        String toString() const;

    private:

        // ----------------------------------------- Init FTP Server -----------------------------------------------
        FTPServer *_ftpSrv; // construct with LittleFS

        String _user;     // User name to log in FTP Server
        String _password; // Password to log in FTP Server
    };

    extern FTPManagerClass FTPManager;
}
#endif // USE_FTP

#endif //TBD_WIFI_PORTAIL_FTP_H
