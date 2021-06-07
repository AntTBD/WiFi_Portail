//
// Created by antbd on 29/05/2021.
//

#include "TBD_WiFi_Portail_SerialDebug.h"


//TBD_WiFi_Portail_SerialDebug::TBD_WiFi_Portail_SerialDebug(): _stream(&Serial) {}
//TBD_WiFi_Portail_SerialDebug::TBD_WiFi_Portail_SerialDebug(HardwareSerial* port): _stream(port) {}
//TBD_WiFi_Portail_SerialDebug::TBD_WiFi_Portail_SerialDebug(HardwareSerial& port): _stream(&port) {}
TBD_WiFi_Portail_SerialDebug::TBD_WiFi_Portail_SerialDebug(HardwareSerial& port, long baudRate, bool wifiDiagnostic):
    _stream(&port),
    _baudRate(baudRate),
    _wifiDiagnostic(wifiDiagnostic) {}


TBD_WiFi_Portail_SerialDebug::~TBD_WiFi_Portail_SerialDebug() {}

bool TBD_WiFi_Portail_SerialDebug::begin()
{
    if(this->_stream) {
        this->_stream->begin(_baudRate);                  // We use a communication at 115200 bds by default
        this->_stream->setDebugOutput(_wifiDiagnostic);   // Disable Wi-Fi Diagnostic by default https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html#enable-wi-fi-diagnostic
        this->println();
        this->print(F("Com OK at "));
        this->print(_baudRate);
        this->println(F(" bds"));

        // Waiting for the serial port to open
        for (uint8_t t = 5; t > 0; t--)
        {
            this->print(F("BOOT WAIT "));
            this->print(t);
            this->println(F("..."));
            this->_stream->flush();
            delay(1000);
        }
        return true;
    }
    return false;
}

size_t TBD_WiFi_Portail_SerialDebug::print(const __FlashStringHelper * ifsh) {
    if(this->_stream) {
        return this->_stream->print(ifsh);
    }
    return 0;
}
size_t TBD_WiFi_Portail_SerialDebug::print(const String &s) {
    if(this->_stream) {
        return this->_stream->print(s);
    }
    return 0;
}
/*size_t TBD_WiFi_Portail_SerialDebug::print(String s) {
    if(this->_stream) {
        return this->_stream->print(s);
    }
    return 0;
}*/
size_t TBD_WiFi_Portail_SerialDebug::print(const char *str) {
    if(this->_stream) {
        return this->_stream->print(str);
    }
    return 0;
}
size_t TBD_WiFi_Portail_SerialDebug::print(char c) {
    if(this->_stream) {
        return this->_stream->print(c);
    }
    return 0;
}
size_t TBD_WiFi_Portail_SerialDebug::print(unsigned char n, int base) {
    if(this->_stream) {
        return this->_stream->print(n, base);
    }
    return 0;
}
size_t TBD_WiFi_Portail_SerialDebug::print(int n, int base) {
    if(this->_stream) {
        return this->_stream->print(n, base);
    }
    return 0;
}
size_t TBD_WiFi_Portail_SerialDebug::print(unsigned int n, int base) {
    if(this->_stream) {
        return this->_stream->print(n, base);
    }
    return 0;
}
size_t TBD_WiFi_Portail_SerialDebug::print(long n, int base) {
    if(this->_stream) {
        return this->_stream->print(n, base);
    }
    return 0;
}
size_t TBD_WiFi_Portail_SerialDebug::print(unsigned long n, int base) {
    if(this->_stream) {
        return this->_stream->print(n, base);
    }
    return 0;
}
size_t TBD_WiFi_Portail_SerialDebug::print(double n, int digits) {
    if(this->_stream) {
        return this->_stream->print(n, digits);
    }
    return 0;
}

//------------------------------------------------------------------------------------------

size_t TBD_WiFi_Portail_SerialDebug::println(const __FlashStringHelper * ifsh) {
    if(this->_stream) {
        return this->_stream->println(ifsh);
    }
    return 0;
}
size_t TBD_WiFi_Portail_SerialDebug::println(const String &s) {
    if(this->_stream) {
        return this->_stream->println(s);
    }
    return 0;
}
/*size_t TBD_WiFi_Portail_SerialDebug::println(String s) {
    if(this->_stream) {
        return this->_stream->println(s);
    }
    return 0;
}*/
size_t TBD_WiFi_Portail_SerialDebug::println(const char *str) {
    if(this->_stream) {
        return this->_stream->println(str);
    }
    return 0;
}
size_t TBD_WiFi_Portail_SerialDebug::println(char c) {
    if(this->_stream) {
        return this->_stream->println(c);
    }
    return 0;
}
size_t TBD_WiFi_Portail_SerialDebug::println(unsigned char n, int base) {
    if(this->_stream) {
        return this->_stream->println(n, base);
    }
    return 0;
}
size_t TBD_WiFi_Portail_SerialDebug::println(int n, int base) {
    if(this->_stream) {
        return this->_stream->println(n, base);
    }
    return 0;
}
size_t TBD_WiFi_Portail_SerialDebug::println(unsigned int n, int base) {
    if(this->_stream) {
        return this->_stream->println(n, base);
    }
    return 0;
}
size_t TBD_WiFi_Portail_SerialDebug::println(long n, int base) {
    if(this->_stream) {
        return this->_stream->println(n, base);
    }
    return 0;
}
size_t TBD_WiFi_Portail_SerialDebug::println(unsigned long n, int base) {
    if(this->_stream) {
        return this->_stream->println(n, base);
    }
    return 0;
}
size_t TBD_WiFi_Portail_SerialDebug::println(double n, int digits) {
    if(this->_stream) {
        return this->_stream->println(n, digits);
    }
    return 0;
}
size_t TBD_WiFi_Portail_SerialDebug::println(void) {
    if(this->_stream) {
        return this->_stream->println();
    }
    return 0;
}

// ------------------------------------------------------------------------------------

//Creating a printf() wrapper https://playground.arduino.cc/Main/Printf/

size_t TBD_WiFi_Portail_SerialDebug::printf(const char * fmt, ...)
{
    if(this->_stream) {

        char buf[256]; // resulting string limited to 128 chars
        va_list args;
        va_start (args, fmt );
        vsnprintf(buf, 256, fmt, args);
        va_end (args);
        return this->_stream->print(buf);
    }
    return 0;
}

size_t TBD_WiFi_Portail_SerialDebug::printf(const __FlashStringHelper *fmt, ... )
{
    if(this->_stream) {
        char buf[256]; // resulting string limited to 128 chars
        va_list args;
        va_start (args, fmt);
#ifdef __AVR__
        vsnprintf_P(buf, sizeof(buf), (const char *)fmt, args); // progmem for AVR
#else
        vsnprintf(buf, sizeof(buf), (const char *)fmt, args); // for the rest of the world
#endif
        va_end(args);
        return this->_stream->print(buf);
    }
    return 0;
}


HardwareSerial* TBD_WiFi_Portail_SerialDebug::getSerial() {
    return this->_stream;
}