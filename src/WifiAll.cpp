#include "WifiAll.h"

// Definitions of AP, STA, WifiAll classes functions

AP::AP()
{
  this->ssid = F("");
  this->password = F("");
  this->hide = false;
  this->ip = F("");
  this->mask = F("");
  this->autoDisable = -1;
}

AP::AP(const String &ssid)
{
  this->ssid = ssid;
  this->password = F("");
  this->hide = false;
  this->ip = F("");
  this->mask = F("");
  this->autoDisable = -1;
}

AP::AP(const String &ssid, const String &password)
{
  this->ssid = ssid;
  this->password = password;
  this->hide = false;
  this->ip = F("");
  this->mask = F("");
  this->autoDisable = -1;
}

AP::~AP() {}

String AP::getSSID() const
{
  return ssid;
}

void AP::setSSID(const String &_ssid)
{
  this->ssid = _ssid;
}

String AP::getPassword() const
{
  return password;
}

void AP::setPassword(const String &_password)
{
  this->password = _password;
}

bool AP::isHide() const
{
  return hide;
}
void AP::setHide(bool _hide)
{
  this->hide = _hide;
}
String AP::getIp() const
{
  return ip;
}
void AP::setIp(const String &_ip)
{
  this->ip = _ip;
}
String AP::getMask() const
{
  return mask;
}
void AP::setMask(const String &_mask)
{
  this->mask = _mask;
}
int AP::getAutoDisable() const
{
  return autoDisable;
}
void AP::setAutoDisable(int _autoDisable)
{
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

String AP::toString() const
{ //.c_str()
  String result = (String)F("  -------- AP ---------") + F("\n") + F("  - ssid:        ") + String(this->ssid) + F("\n") + F("  - password:    ") + String(this->password) + F("\n") + F("  - hide:        ") + (this->hide ? F("true") : F("false")) + F("\n") + F("  - ip:          ") + String(this->ip) + F("\n") + F("  - mask:        ") + String(this->mask) + F("\n") + F("  - autoDisable: ") + String(this->autoDisable) + F(" min") + F("\n") + F("  ---------------------");
  return result;
}

String AP::serialized() const
{
  String result = (String)F("{\n") + F("\"ssid\":\"") + String(this->ssid) + F("\",\n") + F("\"password\":\"") + String(this->password) + F("\",\n") + F("\"hide\":") + (this->hide ? F("true") : F("false")) + F(",\n") + F("\"ip\":\"") + String(this->ip) + F("\",\n") + F("\"mask\":\"") + String(this->mask) + F("\",\n") + F("\"autoDisable\":") + String(this->autoDisable) + F("\n") + F("}");
  return result;
}

STA::STA()
{
  this->ssid = F("");
  this->password = F("");
  this->useDHCP = true;
  this->ip = F("");
  this->mask = F("");
  this->DNSServer = F("");
  this->gateway = F("");
  this->autoDisable = -1;
}

STA::STA(const String &ssid)
{
  this->ssid = ssid;
  this->password = F("");
  this->useDHCP = true;
  this->ip = F("");
  this->mask = F("");
  this->DNSServer = F("");
  this->gateway = F("");
  this->autoDisable = -1;
}

STA::STA(const String &ssid, const String &password)
{
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

String STA::getSSID() const
{
  return ssid;
}

void STA::setSSID(const String &_ssid)
{
  this->ssid = _ssid;
}

String STA::getPassword() const
{
  return password;
}

void STA::setPassword(const String &_password)
{
  this->password = _password;
}

bool STA::isUseDHCP() const
{
  return useDHCP;
}
void STA::setUseDHCP(bool _useDHCP)
{
  this->useDHCP = _useDHCP;
}
String STA::getIp() const
{
  return ip;
}
void STA::setIp(const String &_ip)
{
  this->ip = _ip;
}
String STA::getMask() const
{
  return mask;
}
void STA::setMask(const String &_mask)
{
  this->mask = _mask;
}
String STA::getDNSServer() const
{
  return DNSServer;
}
void STA::setDNSServer(const String &_DNSServer)
{
  this->DNSServer = _DNSServer;
}
String STA::getGateway() const
{
  return gateway;
}
void STA::setGateway(const String &_gateway)
{
  this->gateway = _gateway;
}
int STA::getAutoDisable() const
{
  return autoDisable;
}
void STA::setAutoDisable(int _autoDisable)
{
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
String STA::toString() const
{ //.c_str()
  String result = (String)F("  -------- STA ---------") + F("\n") + F("  - ssid:        ") + String(this->ssid) + F("\n") + F("  - password:    ") + String(this->password) + F("\n") + F("  - useDHCP:     ") + (this->useDHCP ? F("true") : F("false")) + F("\n") + F("  - ip:          ") + String(this->ip) + F("\n") + F("  - mask:        ") + String(this->mask) + F("\n") + F("  - DNSServer:   ") + String(this->DNSServer) + F("\n") + F("  - gateway:     ") + String(this->gateway) + F("\n") + F("  - autoDisable: ") + String(this->autoDisable) + F(" min") + F("\n") + F("  ---------------------");
  return result;
}

String STA::serialized() const
{
  String result = (String)F("{\n") + F("\"ssid\":\"") + String(this->ssid) + F("\",\n") + F("\"password\":\"") + String(this->password) + F("\",\n") + F("\"useDHCP\":") + (this->useDHCP ? F("true") : F("false")) + F(",\n") + F("\"ip\":\"") + String(this->ip) + F("\",\n") + F("\"mask\":\"") + String(this->mask) + F("\",\n") + F("\"DNSServer\":\"") + String(this->DNSServer) + F("\",\n") + F("\"gateway\":\"") + String(this->gateway) + F("\",\n") + F("\"autoDisable\":") + String(this->autoDisable) + F("\n") + F("}");
  return result;
}

WifiAll::WifiAll(const String &hostname, const String &mdnsName)
{
  this->hostname = hostname;
  this->mdnsName = mdnsName;
  this->mode = WIFI_OFF;
  this->ap = new AP();
  this->allSTA = new std::vector<STA>();
  this->resetWifi = false;
}

WifiAll::WifiAll(const String &hostname, const String &mdnsName, const AP ap)
{
  this->hostname = hostname;
  this->mdnsName = mdnsName;
  this->mode = WIFI_OFF;
  *this->ap = ap;
  this->allSTA = new std::vector<STA>();
  this->resetWifi = false;
}

WifiAll::WifiAll(const String &hostname, const String &mdnsName, const AP ap, bool resetWifi)
{
  this->hostname = hostname;
  this->mdnsName = mdnsName;
  this->mode = WIFI_OFF;
  *this->ap = ap;
  this->allSTA = new std::vector<STA>();
  this->resetWifi = resetWifi;
}

WifiAll::WifiAll(const String &hostname, const String &mdnsName, const AP ap, WiFiMode_t mode, bool resetWifi)
{
  this->hostname = hostname;
  this->mdnsName = mdnsName;
  this->mode = mode;
  *this->ap = ap;
  this->allSTA = new std::vector<STA>();
  this->resetWifi = resetWifi;
}

WifiAll::WifiAll(const String &hostname, const String &mdnsName, std::vector<STA> &allSTA)
{
  this->hostname = hostname;
  this->mdnsName = mdnsName;
  this->mode = WIFI_OFF;
  this->ap = new AP();
  this->allSTA = &allSTA;
  this->resetWifi = false;
}

WifiAll::WifiAll(const String &hostname, const String &mdnsName, std::vector<STA> &allSTA, bool resetWifi)
{
  this->hostname = hostname;
  this->mdnsName = mdnsName;
  this->mode = WIFI_OFF;
  this->ap = new AP();
  this->allSTA = &allSTA;
  this->resetWifi = resetWifi;
}

WifiAll::WifiAll(const String &hostname, const String &mdnsName, std::vector<STA> &allSTA, WiFiMode_t mode, bool resetWifi)
{
  this->hostname = hostname;
  this->mdnsName = mdnsName;
  this->mode = mode;
  this->ap = new AP();
  this->allSTA = &allSTA;
  this->resetWifi = resetWifi;
}

WifiAll::WifiAll(const String &hostname, const String &mdnsName, const AP ap, std::vector<STA> &allSTA)
{
  this->hostname = hostname;
  this->mdnsName = mdnsName;
  this->mode = WIFI_OFF;
  *this->ap = ap;
  this->allSTA = &allSTA;
  this->resetWifi = false;
}

WifiAll::WifiAll(const String &hostname, const String &mdnsName, const AP ap, std::vector<STA> &allSTA, WiFiMode_t mode)
{
  this->hostname = hostname;
  this->mdnsName = mdnsName;
  this->mode = mode;
  *this->ap = ap;
  this->allSTA = &allSTA;
  this->resetWifi = false;
}

WifiAll::WifiAll(const String &hostname, const String &mdnsName, const AP ap, std::vector<STA> &allSTA, bool resetWifi)
{
  this->hostname = hostname;
  this->mdnsName = mdnsName;
  this->mode = WIFI_OFF;
  *this->ap = ap;
  this->allSTA = &allSTA;
  this->resetWifi = resetWifi;
}

WifiAll::WifiAll(const String &hostname, const String &mdnsName, const AP ap, std::vector<STA> &allSTA, WiFiMode_t mode, bool resetWifi)
{
  this->hostname = hostname;
  this->mdnsName = mdnsName;
  this->mode = mode;
  *this->ap = ap;
  this->allSTA = &allSTA;
  this->resetWifi = resetWifi;
}

WifiAll::~WifiAll()
{
  delete this->allSTA;
  delete this->ap;
}

String WifiAll::getHostname() const
{
  return this->hostname;
}

void WifiAll::setHostname(const String &_hostname)
{
  this->hostname = _hostname;
}

String WifiAll::getMdnsName() const
{
  return this->mdnsName;
}

void WifiAll::setMdnsName(const String &_mdnsName)
{
  this->mdnsName = _mdnsName;
}

WiFiMode_t WifiAll::getMode() const
{
  return this->mode;
}
void WifiAll::setMode(WiFiMode_t _mode)
{
  this->mode = _mode;
}

AP *WifiAll::getAP()
{
  return this->ap;
}

void WifiAll::setAP(const AP _ap)
{
  *this->ap = _ap;
}

std::vector<STA> *WifiAll::getAllSTA()
{
  return this->allSTA;
}

void WifiAll::setAllSTA(std::vector<STA> &_allSTA)
{
  this->allSTA = &_allSTA;
}

void WifiAll::addSTA(const STA &sta)
{
    this->allSTA->push_back(sta);
}

void WifiAll::resetSTA()
{
    this->allSTA->clear();
}

bool WifiAll::isResetWifi() const
{
  return this->resetWifi;
}

void WifiAll::setResetWifi(bool _resetWifi)
{
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
String WifiAll::toString() const //.c_str()
{
    String result = (String)F("---------- WifiAll ---------") + F("\n") + F("hostname:         ") + String(this->hostname) + F("\n") + F("mdnsName:         ") + String(this->mdnsName) + F("\n") + F("mode:             ") + String(this->mode) + F("\n") + F("AP: ") + F("\n") + this->ap->toString() + F("\n") + F("All STA: ") + F("\n");
    for (STA& sta : *this->allSTA)
    {
        result += sta.toString() + F("\n");
    }
    result += (String)F("resetWifi: ") + (this->resetWifi ? F("true") : F("false")) + F("\n") + F("----------------------------");
    return result;
}

String WifiAll::serialized() const
{
  String result = (String)F("{\n") + F("\"hostname\":\"") + String(this->hostname) + F("\",\n") + F("\"mdnsName\":\"") + String(this->mdnsName) + F("\",\n") + F("\"mode\":\"") + String(this->mode) + F("\",\n") + F("\"AP\":\n") + this->ap->serialized() + F(",\n") + F("\"allSTA\":[\n");
  unsigned int i = 0;
  for (STA& sta : *this->allSTA)
  {
    i++;
    result += sta.serialized();
    if (i != this->allSTA->size())
    {
      result += F(",");
    }
    result += F("\n");
  }
  result += (String)F("],\n") + F("\"resetWifi\":") + (this->resetWifi ? F("true") : F("false")) + F("\n") + F("}\n");

  return result;
}

String WifiAll::serializedForSend() const
{
  String result = (String)F("{") + F("\"mode\":\"") + String(this->mode) + F("\",") + F("\"AP\":") + this->ap->serialized() + F(",") + F("\"allSTA\":[");
  int i = 0;
  for (STA& sta : *this->allSTA)
  {
    i++;
    result += sta.serialized();
    if (i != this->allSTA->size())
    {
      result += (String)F(",");
    }
  }
  result += (String)F("]") + F("}");
  //Serial.println("serializedForSend");
  //Serial.println(result);
  return result;
}
