/*
Website API functions for WS2812b Ledwall with webinterface
Jeroen Klock 17-10-2019
See: https://github.com/klockie86/LedWall
*/

#ifndef REST_API_H // include guard
#define REST_API_H
////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
#include "globals.h"
#include "spiffs_webserver.h"

void handleNotFound();
void setState();
void setBackColor();
void setText();
void setTextColor();
void showTemp();
void setBrightness();
void readTemp();

////////////////////////////////////////////////////////////////////////////////
#endif
