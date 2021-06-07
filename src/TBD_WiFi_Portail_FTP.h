//
// Created by antbd on 05/06/2021.
//

#ifndef TBD_WIFI_PORTAIL_FTP_H
#define TBD_WIFI_PORTAIL_FTP_H

#include <Arduino.h>
#include "TBD_WiFi_Portail.h"
#include "TBD_WiFi_Portail_SerialDebug.h"
#include "TBD_WiFi_Portail_FileSystem.h"


#include <FTPServer.h> //https://github.com/dplasa/FTPClientServer

class TBD_WiFi_Portail_FTP{
public:
    TBD_WiFi_Portail_FTP(TBD_WiFi_Portail_SerialDebug& serialDebug, TBD_WiFi_Portail_FileSystem& fileSystem, String user="user", String password="password");
    ~TBD_WiFi_Portail_FTP();

    void begin();
    void loop();

    void setUser(String user);
    void setPassword(String password);
    String getUser();
    String getPassword();

    String toString();
private:
    TBD_WiFi_Portail_SerialDebug* _serialDebug;
    TBD_WiFi_Portail_FileSystem* _fileSystem;

    // ----------------------------------------- Init FTP Server -----------------------------------------------
    FTPServer* _ftpSrv; // construct with LittleFS

    String _user;         // User name to log in FTP Server
    String _password; // Password to log in FTP Server


};
#endif //TBD_WIFI_PORTAIL_FTP_H
