//
// Created by antbd on 29/05/2021.
//

#ifndef TBD_WIFI_PORTAIL_SERIALDEBUG_H
#define TBD_WIFI_PORTAIL_SERIALDEBUG_H


#include <Arduino.h>
#include "TBD_WiFi_Portail.h"

// Using serial.print to debug a library https://forum.arduino.cc/t/using-serial-print-to-debug-a-library/141845/24
//#include <HardwareSerial.h>

//Creating a printf() wrapper https://playground.arduino.cc/Main/Printf/
#include <stdarg.h>

class TBD_WiFi_Portail_SerialDebug {
public:
    //TBD_WiFi_Portail_SerialDebug();
    //TBD_WiFi_Portail_SerialDebug(HardwareSerial* port = &Serial);
    //TBD_WiFi_Portail_SerialDebug(HardwareSerial& port = Serial);

    //TBD_WiFi_Portail_SerialDebug(HardwareSerial& port = Serial);
    TBD_WiFi_Portail_SerialDebug(HardwareSerial& port = Serial, long baudRate = 115200, bool wifiDiagnostic = false);

    ~TBD_WiFi_Portail_SerialDebug();

    bool begin();

    size_t print(const __FlashStringHelper *);
    size_t print(const String &);
    //size_t print(String);
    size_t print(const char *);
    size_t print(char);
    size_t print(unsigned char, int = DEC);
    size_t print(int, int = DEC);
    size_t print(unsigned int, int = DEC);
    size_t print(long, int = DEC);
    size_t print(unsigned long, int = DEC);
    size_t print(double, int = 2);

    size_t println(const __FlashStringHelper *);
    size_t println(const String &);
    //size_t println(String);
    size_t println(const char *);
    size_t println(char);
    size_t println(unsigned char, int = DEC);
    size_t println(int, int = DEC);
    size_t println(unsigned int, int = DEC);
    size_t println(long, int = DEC);
    size_t println(unsigned long, int = DEC);
    size_t println(double, int = 2);
    size_t println(void);

    //Creating a printf() wrapper https://playground.arduino.cc/Main/Printf/
    size_t printf(const char * fmt, ...);
    size_t printf(const __FlashStringHelper *fmt, ... );

    HardwareSerial* getSerial();

protected:
    HardwareSerial* _stream;

private:
    long _baudRate;
    bool _wifiDiagnostic;
};


//extern TBD_WiFi_Portail_SerialDebug _TBD_WiFi_Portail_SerialDebug;

#endif //TBD_WIFI_PORTAIL_SERIALDEBUG_H
