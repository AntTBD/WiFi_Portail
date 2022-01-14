//
// Created by antbd on 05/06/2021.
//


#include "TBD_WiFi_Portail_FTP.h"

#ifdef USE_FTP
namespace WiFi_Portail_API {

    FTP::FTP(SerialDebug &serialDebug, FileSystem &fileSystem, String user, String password) :
            _serialDebug(&serialDebug), _fileSystem(&fileSystem), _user(user), _password(password) {
        this->_ftpSrv = new FTPServer(*this->_fileSystem->fileSystem);
    }

    FTP::~FTP() {
        delete this->_serialDebug;
        delete this->_fileSystem;
        delete this->_ftpSrv;
    }

    void FTP::begin() {
        this->_serialDebug->println(F("=== Setup FTP ==="));
        this->_serialDebug->println(F("Starting FTP server"));
        this->_serialDebug->println(this->toString());
        this->_ftpSrv->begin(this->getUser(),
                             this->getPassword()); // username, password for ftp.  set ports in ESP8266FtpServer.h  (default 21, 50009 for PASV)
        this->_serialDebug->println(F(" => FTP ready"));
        this->_serialDebug->println(F("================="));
    }

    void FTP::loop() {

        this->_ftpSrv->handleFTP();
    }


    void FTP::setUser(const String &user) {
        this->_user = user;
    }

    void FTP::setPassword(const String &password) {
        this->_password = password;
    }

    String FTP::getUser() const {
        return this->_user;
    }

    String FTP::getPassword() const {
        return this->_password;
    }


    String FTP::toString() const {
        return (String) F("======== FTP Server =======") + F("\n")
               + F("User :      ") + this->getUser() + F("\n")
               + F("Password :  ") + this->getPassword() + F("\n")
               + F("===========================");
    }
}
#endif // USE_FTP