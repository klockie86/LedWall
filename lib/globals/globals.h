/*
Global settings for WS2812b Ledwall with webinterface
Jeroen Klock 17-10-2019
See: https://github.com/klockie86/LedWall
*/

#ifndef GLOBALS_H // include guard
#define GLOBALS_H
////////////////////////////////////////////////////////////////////////////////
// Global settings
////////////////////////////////////////////////////////////////////////////////
#define BRANDNAME "Omexom"
#define NAME "Omexom LedWAll"
#define PASSWORD  "Omexom123"
#define PIN D2              // Output pin for data line leds
#define DHTPIN D4           // Input pin for DHT sensor
#define DHTTYPE DHT22       // DHT 22  (AM2302)
#define X_MAX 20            // Amount of leds X-Axis
#define Y_MAX 30            // Amount of leds Y-Axis
#define BRIGHTNESS 20       // LED Brightness 0 - 255
#define WEB_PORT 80         // Port for WIFI server
#define DBG_OUTPUT_PORT Serial         // Port for WIFI server

////////////////////////////////////////////////////////////////////////////////
// libraries
////////////////////////////////////////////////////////////////////////////////
// some general libraries
#include <Arduino.h>
// libraries for the led wall
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
// libraries for state saving
#include <EEPROM.h>
// libraries for wifi en webserver
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
// library for SPIFFS
#include <FS.h>

// DHT related stuff
#include <DHT.h>
#include <Adafruit_Sensor.h>

////////////////////////////////////////////////////////////////////////////////
// Global vars
////////////////////////////////////////////////////////////////////////////////
typedef enum state {
  off,
  red,
  green,
  blue,
  flipmode,
  colorpick
}state;

extern int iCurrentState, iRecState, iColorR, iColorG, iColorB, iTextColorR, iTextColorG, iTextColorB;
extern bool bShowTemp;
extern String sText;

////////////////////////////////////////////////////////////////////////////////
// Initiation
////////////////////////////////////////////////////////////////////////////////
extern ESP8266WebServer server;

// Instanciate NeoMatrix
extern Adafruit_NeoMatrix matrix;

// Initialize DHT
extern DHT dht;

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
int getClimate(float &,float &); // return climate

////////////////////////////////////////////////////////////////////////////////
#endif // settings.h
