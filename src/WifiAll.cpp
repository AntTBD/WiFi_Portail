#include "WifiAll.h"

// Definitions of AP, STA, WifiAll classes functions

AP::AP() {
  this->ssid = F("");
  this->password = F("");
  this->hide = false;
  this->ip = F("");
  this->mask = F("");
  this->autoDisable = -1;
}

AP::AP(String ssid) {
  this->ssid = ssid;
  this->password = F("");
  this->hide = false;
  this->ip = F("");
  this->mask = F("");
  this->autoDisable = -1;
}

AP::AP(String ssid, String password) {
  this->ssid = ssid;
  this->password = password;
  this->hide = false;
  this->ip = F("");
  this->mask = F("");
  this->autoDisable = -1;
}

AP::~AP() {}

String AP::getSSID() {
  return ssid;
}

void AP::setSSID(String _ssid) {
  this->ssid = _ssid;
}

String AP::getPassword() {
  return password;
}

void AP::setPassword(String _password) {
  this->password = _password;
}

bool AP::isHide(){
  return hide;
}
void AP::setHide(bool _hide) {
  this->hide = _hide;
}
String AP::getIp() {
  return ip;
}
void AP::setIp(String _ip) {
  this->ip = _ip;
}
String AP::getMask() {
  return mask;
}
void AP::setMask(String _mask) {
  this->mask = _mask;
}
int AP::getAutoDisable(){
  return autoDisable;
}
void AP::setAutoDisable(int _autoDisable) {
  this->autoDisable = _autoDisable;
}

/*
ostream &operator<<(ostream &os, const AP &ap) {
  os << "-------- AP ---------" << "\n"
  << "ssid: " << ap.ssid.c_str() << "\n"
  << "password: " << ap.password.c_str() << "\n"
  << "hide: " << (ap.hide ? "True" : "False") << "\n"
  << "ip: " << ap.ip.c_str() << "\n"
  << "mask: " << ap.mask.c_str() << "\n"
  << "autoDisable: " << ap.autoDisable << " min\n"
  << "---------------------";
  return os;
}
*/

String AP::toString() {//.c_str()
  String result = (String)F("  -------- AP ---------") + F("\n")
    + F("  - ssid:        ") + String(this->ssid) + F("\n")
    + F("  - password:    ") + String(this->password) + F("\n")
    + F("  - hide:        ") + (this->hide ? F("true") : F("false")) + F("\n")
    + F("  - ip:          ") + String(this->ip) + F("\n")
    + F("  - mask:        ") + String(this->mask) + F("\n")
    + F("  - autoDisable: ") + String(this->autoDisable) + F(" min") + F("\n")
    + F("  ---------------------");
  return result;
}

String AP::serialized(){
  return (String)F("{\n")
   + F("\"ssid\":\"") + String(this->ssid) + F("\",\n")
   + F("\"password\":\"") + String(this->password) + F("\",\n")
   + F("\"hide\":") + (this->hide ? F("true") : F("false")) + F(",\n")
   + F("\"ip\":\"") + String(this->ip) + F("\",\n")
   + F("\"mask\":\"") + String(this->mask) + F("\",\n")
   + F("\"autoDisable\":") + String(this->autoDisable) + F("\n")
   + F("}");
}

STA::STA() {
  this->ssid = F("");
  this->password = F("");
  this->useDHCP = true;
  this->ip = F("");
  this->mask = F("");
  this->DNSServer = F("");
  this->gateway = F("");
  this->autoDisable = -1;
}

STA::STA(String ssid) {
  this->ssid = ssid;
  this->password = F("");
  this->useDHCP = true;
  this->ip = F("");
  this->mask = F("");
  this->DNSServer = F("");
  this->gateway = F("");
  this->autoDisable = -1;
}

STA::STA(String ssid, String password) {
  this->ssid = ssid;
  this->password = password;
  this->useDHCP = true;
  this->ip = F("");
  this->mask = F("");
  this->DNSServer = F("");
  this->gateway = F("");
  this->autoDisable = -1;
}
STA::~STA() {}

String STA::getSSID() {
  return ssid;
}

void STA::setSSID(String _ssid) {
  this->ssid = _ssid;
}

String STA::getPassword() {
  return password;
}

void STA::setPassword(String _password) {
  this->password = _password;
}

bool STA::isUseDHCP(){
  return useDHCP;
}
void STA::setUseDHCP(bool _useDHCP) {
  this->useDHCP = _useDHCP;
}
String STA::getIp(){
  return ip;
}
void STA::setIp(String _ip) {
  this->ip = _ip;
}
String STA::getMask() {
  return mask;
}
void STA::setMask(String _mask) {
  this->mask = _mask;
}
String STA::getDNSServer() {
  return DNSServer;
}
void STA::setDNSServer(String _DNSServer) {
  this->DNSServer = _DNSServer;
}
String STA::getGateway() {
  return gateway;
}
void STA::setGateway(String _gateway) {
  this->gateway = _gateway;
}
int STA::getAutoDisable(){
  return autoDisable;
}
void STA::setAutoDisable(int _autoDisable) {
  this->autoDisable = _autoDisable;
}
/*
ostream &operator<<(ostream &os, const STA &sta) {
  os << "-------- STA ---------" << "\n"
  << "ssid: " << sta.ssid.c_str() << "\n"
  << "password: " << sta.password.c_str() << "\n"
  << "useDHCP: " << (sta.useDHCP ? "True" : "False") << "\n"
  << "ip: " << sta.ip.c_str() << "\n"
  << "mask: " << sta.mask.c_str() << "\n"
  << "DNSServer: " << sta.DNSServer.c_str() << "\n"
  << "gateway: " << sta.gateway.c_str() << "\n"
  << "autoDisable: " << sta.autoDisable << " min\n"
  << "---------------------";
  return os;
}
*/
String STA::toString() {//.c_str()
  String result = (String)F("  -------- STA ---------") + F("\n")
    + F("  - ssid:        ") + String(this->ssid) + F("\n")
    + F("  - password:    ") + String(this->password) +F("\n")
    + F("  - useDHCP:     ") + (this->useDHCP ? F("true") : F("false")) + F("\n")
    + F("  - ip:          ") + String(this->ip) + F("\n")
    + F("  - mask:        ") + String(this->mask) + F("\n")
    + F("  - DNSServer:   ") + String(this->DNSServer) + F("\n")
    + F("  - gateway:     ") + String(this->gateway) + F("\n")
    + F("  - autoDisable: ") + String(this->autoDisable) + F(" min") + F("\n")
    + F("  ---------------------");
  return result;
}

String STA::serialized(){
  return (String)F("{\n")
   + F("\"ssid\":\"") + String(this->ssid) + F("\",\n")
   + F("\"password\":\"") + String(this->password) + F("\",\n")
   + F("\"useDHCP\":") + (this->useDHCP ? F("true") : F("false")) + F(",\n")
   + F("\"ip\":\"") + String(this->ip) + F("\",\n")
   + F("\"mask\":\"") + String(this->mask) + F("\",\n")
   + F("\"DNSServer\":\"") + String(this->DNSServer) + F("\",\n")
   + F("\"gateway\":\"") + String(this->gateway) + F("\",\n")
   + F("\"autoDisable\":") + String(this->autoDisable) + F("\n")
   + F("}");
}

WifiAll::WifiAll(String hostname, String mdnsName) {
  this->hostname = hostname;
  this->mdnsName = mdnsName;
  this->mode = WIFI_OFF;
  this->ap = AP();
  //this->allSTA = NULL;
  this->resetWifi = false;
}

WifiAll::WifiAll(String hostname, String mdnsName, AP ap) {
  this->hostname = hostname;
  this->mdnsName = mdnsName;
  this->mode = WIFI_OFF;
  this->ap = ap;
  //this->allSTA = NULL;
  this->resetWifi = false;
}

WifiAll::WifiAll(String hostname, String mdnsName, AP ap, bool resetWifi) {
  this->hostname = hostname;
  this->mdnsName = mdnsName;
  this->mode = WIFI_OFF;
  this->ap = ap;
  //this->allSTA = NULL;
  this->resetWifi = resetWifi;
}

WifiAll::WifiAll(String hostname, String mdnsName, AP ap, WiFiMode_t mode, bool resetWifi) {
  this->hostname = hostname;
  this->mdnsName = mdnsName;
  this->mode = mode;
  this->ap = ap;
  //this->allSTA = NULL;
  this->resetWifi = resetWifi;
}

WifiAll::WifiAll(String hostname, String mdnsName, AFArray<STA> allSTA) {
  this->hostname = hostname;
  this->mdnsName = mdnsName;
  this->mode = WIFI_OFF;
  this->ap = AP();
  this->allSTA = allSTA;
  this->resetWifi = false;
}

WifiAll::WifiAll(String hostname, String mdnsName, AFArray<STA> allSTA, bool resetWifi) {
  this->hostname = hostname;
  this->mdnsName = mdnsName;
  this->mode = WIFI_OFF;
  this->ap = AP();
  this->allSTA = allSTA;
  this->resetWifi = resetWifi;
}

WifiAll::WifiAll(String hostname, String mdnsName, AFArray<STA> allSTA, WiFiMode_t mode, bool resetWifi) {
  this->hostname = hostname;
  this->mdnsName = mdnsName;
  this->mode = mode;
  this->ap = AP();
  this->allSTA = allSTA;
  this->resetWifi = resetWifi;
}

WifiAll::WifiAll(String hostname, String mdnsName, AP ap, AFArray<STA> allSTA) {
  this->hostname = hostname;
  this->mdnsName = mdnsName;
  this->mode = WIFI_OFF;
  this->ap = ap;
  this->allSTA = allSTA;
  this->resetWifi = false;
}

WifiAll::WifiAll(String hostname, String mdnsName, AP ap, AFArray<STA> allSTA, WiFiMode_t mode) {
  this->hostname = hostname;
  this->mdnsName = mdnsName;
  this->mode = mode;
  this->ap = ap;
  this->allSTA = allSTA;
  this->resetWifi = false;
}

WifiAll::WifiAll(String hostname, String mdnsName, AP ap, AFArray<STA> allSTA, bool resetWifi) {
  this->hostname = hostname;
  this->mdnsName = mdnsName;
  this->mode = WIFI_OFF;
  this->ap = ap;
  this->allSTA = allSTA;
  this->resetWifi = resetWifi;
}

WifiAll::WifiAll(String hostname, String mdnsName, AP ap, AFArray<STA> allSTA, WiFiMode_t mode, bool resetWifi) {
  this->hostname = hostname;
  this->mdnsName = mdnsName;
  this->mode = mode;
  this->ap = ap;
  this->allSTA = allSTA;
  this->resetWifi = resetWifi;
}

WifiAll::~WifiAll() {}

String WifiAll::getHostname() {
  return this->hostname;
}

void WifiAll::setHostname(String _hostname) {
  this->hostname = _hostname;
}

String WifiAll::getMdnsName() {
  return this->mdnsName;
}

void WifiAll::setMdnsName(String _mdnsName) {
  this->mdnsName = _mdnsName;
}

WiFiMode_t WifiAll::getMode() {
  return this->mode;
}
void WifiAll::setMode(WiFiMode_t _mode) {
  this->mode = _mode;
}

AP &WifiAll::getAP() {
  return this->ap;
}

void WifiAll::setAP(AP &_ap) {
  this->ap = _ap;
}

AFArray<STA> WifiAll::getAllSTA() {
  return this->allSTA;
}

void WifiAll::setAllSTA(AFArray<STA> _allSTA) {
  this->allSTA = _allSTA;
}

bool WifiAll::isResetWifi() {
  return this->resetWifi;
}

void WifiAll::setResetWifi(bool _resetWifi) {
  this->resetWifi = _resetWifi;
}
/*
ostream &operator<<(ostream &os, WifiAll &all) {
  os << "---------- WifiAll ---------" << "\n"
  << "hostname: " << all.hostname.c_str() << "\n"
  << "mdnsName: " << all.mdnsName.c_str() << "\n"
  << "AP: \n" << all.ap << "\n"
  << "STA: \n" << all.sta << "\n"
  << "resetWifi: " << (all.resetWifi ? "True" : "False") << "\n"
  << "----------------------------";
  return os;
}
*/
String WifiAll::toString() {//.c_str()
  String result = (String)F("---------- WifiAll ---------") + F("\n")
    + F("hostname:         ") + String(this->hostname) + F("\n")
    + F("mdnsName:         ") + String(this->mdnsName) + F("\n")
    + F("mode:             ") + String(this->mode) + F("\n")
    + F("AP: ") + F("\n") + this->ap.toString() + F("\n")
    + F("All STA: ") + F("\n");
  while (this->allSTA.has_next()){
    result += this->allSTA.next().toString() + F("\n");
  }
  result += (String)F("resetWifi: ") + (this->resetWifi ? F("true") : F("false")) + F("\n")
    + F("----------------------------");
  return result;
}

String WifiAll::serialized(){
  String result = (String)F("{\n")
   + F("\"hostname\":\"") + String(this->hostname) + F("\",\n")
   + F("\"mdnsName\":\"") + String(this->mdnsName) + F("\",\n")
   + F("\"mode\":\"") + String(this->mode) + F("\",\n")
   + F("\"AP\":\n") 
   + this->ap.serialized() + F(",\n")
   + F("\"allSTA\":[\n");
  unsigned int i = 0;
  while (this->allSTA.has_next()) {
    i++;
    result += this->allSTA.next().serialized();
    if(i != this->allSTA.size()) {
      result += F(",");
    }
    result += F("\n");
  }
  result += (String)F("],\n")
   + F("\"resetWifi\":") + (this->resetWifi ? F("true") : F("false")) + F("\n")
   + F("}\n");

   return result;
}
