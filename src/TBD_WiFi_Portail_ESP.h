//
// Created by antbd on 16/06/2021.
//

#ifndef TBD_WIFI_PORTAIL_ESP_H
#define TBD_WIFI_PORTAIL_ESP_H

#include <Arduino.h>
#include "TBD_WiFi_Portail.h"
#include "TBD_WiFi_Portail_NTP.h"
#include "TBD_WiFi_Portail_FileSystem.h"
#include "TBD_WiFi_Portail_WebSocket.h"

#include <AFArray.h>
#include <ArduinoJson.h>

class TBD_WiFi_Portail_ESP {
public:
    TBD_WiFi_Portail_ESP();
    ~TBD_WiFi_Portail_ESP();
    void addNTP(TBD_WiFi_Portail_NTP& ntp);
    void addFileSystem(TBD_WiFi_Portail_FileSystem& fileSystem);
    void addWebSocket(TBD_WiFi_Portail_WebSocket& webSocket);

    DynamicJsonDocument getHardwareInfosJson();
    String getHardwareInfosString();
    void sendHardwareInfosByWebSocket();

private:
    AFArray<String> _infosName;
    //String _infosName[];

    uint16_t nbInfos;

    TBD_WiFi_Portail_NTP* _ntp;
    TBD_WiFi_Portail_FileSystem* _fileSystem;
    TBD_WiFi_Portail_WebSocket* _webSocket;

    void getInfoData(String id, String (&tab)[2]);

};


#endif //TBD_WIFI_PORTAIL_ESP_H
