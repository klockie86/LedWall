/*
Rest API for WS2812b Ledwall with webinterface
Jeroen Klock 2-10-2019
See: https://github.com/klockie86/LedWall
*/

// some general libraries
#include <Arduino.h>
//#include "lib/settings.h"

void handleNotFound();
void setState();
void setBackColor();
void setText();
void setTextColor();
void showTemp();
void setBrightness();
void readTemp();
