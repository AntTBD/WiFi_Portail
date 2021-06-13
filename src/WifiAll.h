#ifndef WifiAll_h_included
#define WifiAll_h_included

// Definitions of AP, STA, WifiAll classes

#include <Arduino.h>
/*#include <string>
#include <ostream>
#include <stdlib.h>
#include <utility>*/

#include <AFArray.h>
#include <ESP8266WiFi.h>// to include ESP8266WiFiType.h library

//using namespace std;

class AP
{
public:
  String ssid;
  String password;
  bool hide;
  String ip;
  String mask;
  int autoDisable;

  // constructor
  AP();
  AP(String ssid);
  AP(String ssid, String password);

  // destructor
  ~AP();

  // getters and setters
  String getSSID();
  void setSSID(String _ssid);
  String getPassword();
  void setPassword(String _password);
  bool isHide();
  void setHide(bool _hide);
  String getIp();
  void setIp(String _ip);
  String getMask();
  void setMask(String _mask);
  int getAutoDisable();
  void setAutoDisable(int _autoDisable);

  //toString
  //friend ostream &operator<<(ostream &os, const AP &ap);
  String toString();
  String serialized();
};

class STA
{
public:
  String ssid;
  String password;
  bool useDHCP;
  String ip;
  String mask;
  String DNSServer;
  String gateway;
  int autoDisable;

  // constructor
  STA();
  STA(String ssid);
  STA(String ssid, String password);

  // destructor
  ~STA();

  // getters and setters
  String getSSID();
  void setSSID(String _ssid);
  String getPassword();
  void setPassword(String _password);
  bool isUseDHCP();
  void setUseDHCP(bool _useDHCP);
  String getIp();
  void setIp(String _ip);
  String getMask();
  void setMask(String _mask);
  String getDNSServer();
  void setDNSServer(String _DNSServer);
  String getGateway();
  void setGateway(String _gateway);
  int getAutoDisable();
  void setAutoDisable(int _autoDisable);

  //toString
  //friend ostream &operator<<(ostream &os, const STA &sta);
  String toString();
  String serialized();
};

class WifiAll 
{
public:
  /* variables */
  String hostname; // name which is displayed on router
  String mdnsName; // Domain name for the mDNS responder
  WiFiMode_t mode; // from ESP8266WiFiType.h : WIFI_OFF = 0, WIFI_STA = 1, WIFI_AP = 2, WIFI_AP_STA = 3
                   //                          /* these two pseudo modes are experimental: */ WIFI_SHUTDOWN = 4, WIFI_RESUME = 8
  AP ap;
  AFArray<STA> allSTA;
  bool resetWifi;

  // constructor
  WifiAll(String hostname=("ESP_"+WiFi.macAddress()), String mdnsName=("ESP_"+WiFi.macAddress()));
  WifiAll(String hostname, String mdnsName, AP ap);
  WifiAll(String hostname, String mdnsName, AP ap, bool resetWifi);
  WifiAll(String hostname, String mdnsName, AP ap, WiFiMode_t mode, bool resetWifi);
  WifiAll(String hostname, String mdnsName, AFArray<STA> allSTA);
  WifiAll(String hostname, String mdnsName, AFArray<STA> allSTA, bool resetWifi);
  WifiAll(String hostname, String mdnsName, AFArray<STA> allSTA, WiFiMode_t mode, bool resetWifi);
  WifiAll(String hostname, String mdnsName, AP ap, AFArray<STA> allSTA);
  WifiAll(String hostname, String mdnsName, AP ap, AFArray<STA> allSTA, WiFiMode_t mode);
  WifiAll(String hostname, String mdnsName, AP ap, AFArray<STA> allSTA, bool resetWifi);
  WifiAll(String hostname, String mdnsName, AP ap, AFArray<STA> allSTA, WiFiMode_t mode, bool resetWifi);

  // destructor
  ~WifiAll();

  // getters and setters
  String getHostname();
  void setHostname(String _hostname);
  String getMdnsName();
  void setMdnsName(String _mdnsName);
  WiFiMode_t getMode();
  void setMode(WiFiMode_t _mode);
  AP &getAP();
  void setAP(AP &_ap);
  AFArray<STA> getAllSTA();
  void setAllSTA(AFArray<STA> _allSTA);
  // bool removeAllSTA();
  // STA &getOneSTA(int indice);
  // STA &addOneSTA(STA sta);
  // STA &setOneSTA(int indice, STA sta);
  // bool removeOneSTA(int indice);
  // T* STAtoArray();
  bool isResetWifi();
  void setResetWifi(bool _resetWifi);

  // toString
  //friend ostream &operator<<(ostream &os, WifiAll &all);
  String toString();
  String serialized();
};

#endif // WifiAll_h_included
