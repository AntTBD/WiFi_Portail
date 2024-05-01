//
// Created by antbd on 05/06/2021.
//


#include "WiFi_Portail_FTP.h"

#ifdef USE_FTP
namespace WiFi_Portail_API {

    FTPManagerClass FTPManager;

    FTPManagerClass::FTPManagerClass(String user, String password) :
            _user(user), _password(password) {
        this->_ftpSrv = new FtpServer();
    }

    FTPManagerClass::~FTPManagerClass() {
        delete this->_ftpSrv;
    }

    void FTPManagerClass::begin() {
        SerialDebug_println(F("=== Setup FTP ==="));
        SerialDebug_println(F("Starting FTP server"));
        SerialDebug_println(this->toString());
        this->_ftpSrv->begin(this->getUser().c_str(),
                             this->getPassword().c_str()); // username, password for ftp.  set ports in ESP8266FtpServer.h  (default 21, 50009 for PASV)
        SerialDebug_println(F(" => FTP ready"));
        SerialDebug_println(F("================="));
    }

    void FTPManagerClass::loop() {

        this->_ftpSrv->handleFTP();
    }


    void FTPManagerClass::setUser(const String &user) {
        this->_user = user;
    }

    void FTPManagerClass::setPassword(const String &password) {
        this->_password = password;
    }

    String FTPManagerClass::getUser() const {
        return this->_user;
    }

    String FTPManagerClass::getPassword() const {
        return this->_password;
    }


    String FTPManagerClass::toString() const {
        String str = F("======== FTP Server =======");
        str += F("\n");
        str += F("User:      ");
        str += this->getUser();
        str += F("\n");
        str += F("Password:  ");
        str += this->getPassword();
        str += F("\n");
        str += F("===========================");
        return str;
    }
}
#endif // USE_FTP