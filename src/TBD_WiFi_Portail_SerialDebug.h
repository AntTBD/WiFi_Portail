//
// Created by antbd on 29/05/2021.
//

#ifndef TBD_WIFI_PORTAIL_SERIALDEBUG_H
#define TBD_WIFI_PORTAIL_SERIALDEBUG_H

#include "TBD_WiFi_Portail.h"

// Using serial.print to debug a library https://forum.arduino.cc/t/using-serial-print-to-debug-a-library/141845/24
//#include <HardwareSerial.h>

//Creating a printf() wrapper https://playground.arduino.cc/Main/Printf/
#include <stdarg.h>

namespace WiFi_Portail_API {
    class SerialDebugClass {
    public:
        //SerialDebugClass();
        //SerialDebugClass(HardwareSerial* port = &Serial);
        //SerialDebugClass(HardwareSerial& port = Serial);

        //SerialDebugClass(HardwareSerial& port = Serial);
        SerialDebugClass();

        ~SerialDebugClass();

        bool begin(HardwareSerial &port = Serial, long baudRate = 115200, bool wifiDiagnostic = false);

        void setDebug(bool debug);

        bool getDebug() const;

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
        size_t printf(const char *fmt, ...);

        size_t printf(const __FlashStringHelper *fmt, ...);

        HardwareSerial *getSerial();

    protected:
        HardwareSerial *_stream;

    private:

        long _baudRate;
        bool _wifiDiagnostic;

        bool _debug;
    };

    extern SerialDebugClass SerialDebug;
}

#endif //TBD_WIFI_PORTAIL_SERIALDEBUG_H
