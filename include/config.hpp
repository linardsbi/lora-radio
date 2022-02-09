#pragma once
#include <stdlib_compatibility.hpp>
#include <Arduino.h>

#ifdef __AVR_ATmega328P__
#define RADIO_PINS .RX = 3, \
                   .TX = 4, \
                   .AUX = 2, \
                   .M0 = 6, \
                   .M1 = 7
#endif

#if defined(ESP32) || defined(ESP8266)
#define RADIO_PINS .RX = D2, \
                   .TX = D3, \
                   .AUX = D5, \
                   .M0 = D7, \
                   .M1 = D6
#endif