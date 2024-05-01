# TBD WiFi Portail

Library to get a portail to configure WiFi, NTP, ...

Thanks to different class to configure Serial, FileSystem, a Files Manager, multiple WiFi, MDNS, a FTP server, WebServer server, WebSocket server, WebEvents server (user & admin), OTA upload, NTP server

[comment]: <> (<div align="center">)

[comment]: <> (    <div style="width: 100px;">)

[comment]: <> (![Logo]&#40;images/logo.svg&#41;)

[comment]: <> (    </div>)

[comment]: <> (</div>)

![issues](https://img.shields.io/github/issues/AntTBD/WiFi_Portail)
![forks](https://img.shields.io/github/forks/AntTBD/WiFi_Portail)
![stars](https://img.shields.io/github/stars/AntTBD/WiFi_Portail)

![MIT License](https://img.shields.io/github/license/AntTBD/WiFi_Portail)

[comment]: <> (![travis build]&#40;https://img.shields.io/travis/AntTBD/WiFi_Portail.svg?style=flat-square&#41;)

[comment]: <> (![codecov coverage]&#40;https://img.shields.io/codecov/c/github/AntTBD/WiFi_Portail.svg?style=flat-square&#41;)

[comment]: <> (![version]&#40;https://img.shields.io/npm/v/WiFi_Portail.svg?style=flat-square&#41;)

[comment]: <> (![downloads]&#40;https://img.shields.io/npm/dm/WiFi_Portail.svg?style=flat-square&#41;)

[comment]: <> (![GitHub release &#40;latest by date&#41;]&#40;https://img.shields.io/github/v/release/AntTBD/WiFi_Portail&#41;)

[comment]: <> (![GitHub package.json version]&#40;https://img.shields.io/github/package-json/v/AntTBD/WiFi_Portail&#41;)


[comment]: <> ([![Backers on Open Collective]&#40;https://opencollective.com/WiFI_Portail/backers/badge.svg&#41;]&#40;#backers&#41; )

[comment]: <> ([![Sponsors on Open Collective]&#40;https://opencollective.com/WiFI_Portail/sponsors/badge.svg&#41;]&#40;#sponsors&#41; )

[comment]: <> ([![Chat at https://gitter.im/esp-rfid/Lobby]&#40;https://badges.gitter.im/WiFI_Portail.svg&#41;]&#40;https://gitter.im/WiFI_Portail/Lobby&#41; )

[comment]: <> ([![Build Status]&#40;https://travis-ci.org/anttbd/WiFI_Portail.svg?branch=stable&#41;]&#40;https://travis-ci.org/anttbd/WiFI_Portail&#41; )

[comment]: <> ([![Codacy Badge]&#40;https://api.codacy.com/project/badge/Grade/d475052a09bd4865bc2654f524614cba&#41;]&#40;https://www.codacy.com/app/omersiar/WiFi_Portail?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=AntTBD/WiFi_Portail&amp;utm_campaign=Badge_Grade&#41; )

[comment]: <> ([![BCH compliance]&#40;https://bettercodehub.com/edge/badge/anttbd/WiFI_Portail?branch=stable&#41;]&#40;https://bettercodehub.com/&#41; )

[comment]: <> ([![Bountysource]&#40;https://api.bountysource.com/badge/team?team_id=242217&#41;]&#40;https://salt.bountysource.com/checkout/amount?team=WiFI_Portail&#41;)


[comment]: <> (## Version)

[comment]: <> ([![Version]&#40;https://badge.fury.io/gh/tterb%2FHyde.svg&#41;]&#40;https://badge.fury.io/gh/tterb%2FHyde&#41;)

[comment]: <> ([![GitHub Release]&#40;https://img.shields.io/github/release/tterb/PlayMusic.svg?style=flat&#41;]&#40;&#41;  )


[comment]: <> (## Contributors)

[comment]: <> (This project exists thanks to all the people who contribute.)

[comment]: <> (<a href="https://github.com/AntTBD/WiFi_Portail/graphs/contributors"><img src="https://opencollective.com/WiFi_Portail/contributors.svg?width=890&button=false" /></a>)

<!--
## Instructions for NTP update lib with ESP8266 v3.0.2
In **NTPClientLib.h**, at line **501**, replace:

`bool summertime (int year, byte month, byte day, byte hour, byte weekday, byte tzHours);`

by:
```c++
    bool summertime (int year, uint8_t month, uint8_t day, uint8_t hour, uint8_t weekday, uint8_t tzHours);
```
And in **NTPClientLib.cpp**, at line **645**, replace:

`bool NTPClient::summertime (int year, byte month, byte day, byte hour, byte weekday, byte tzHours)`

by:
```c++
bool NTPClient::summertime (int year, uint8_t month, uint8_t day, uint8_t hour, uint8_t weekday, uint8_t tzHours)
```
-->

## Instructions for secure https connection

1. Generate key pairs (certificate) to **data folder** in the sketch directory. Don't forget to change **CN= with your Domain name** in the under lying codes.
>    * `openssl genrsa -out Key.pem 1024`
>    * `openssl req -x509 -out Cert.pem -key Key.pem -new -sha256 -subj /CN=your.domain -addext "keyUsage=digitalSignature,keyEncipherment" -addext extendedKeyUsage=serverAuth`
> 
> 
2. In ESPAsyncTCP.h, change the following line 1323-1324:
>   ```c++
>   if(p->pb)
>       c->_recv(pcb, p->pb, 0);
>   ```
>   to:
>   ```c++
>   if(p->pb)
>   {
>       auto errorTracker = c->getACErrorTracker();
>       c->_recv(errorTracker, pcb, p->pb, 0);
>   }
>   ```
>   
3. In ESPAsyncTCP/src/async_config.h, edit the first lines (4 to 6):
> ```c++
> /*#ifndef ASYNC_TCP_SSL_ENABLED
> #define ASYNC_TCP_SSL_ENABLED 0  // original
> #endif*/
> #define ASYNC_TCP_SSL_ENABLED 1
> ```
