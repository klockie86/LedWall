/*
settings for WS2812b Ledwall with webinterface
Jeroen Klock 2-10-2019
See: https://github.com/klockie86/LedWall
*/
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
enum state {off,red,green,blue,flipmode,colorpick};
int iCurrentState, iRecState, iColorR, iColorG, iColorB;

////////////////////////////////////////////////////////////////////////////////
// Initiation
////////////////////////////////////////////////////////////////////////////////
// Instanciate HTTP(80)
ESP8266WebServer server(80);

// Instanciate NeoMatrix
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(X_MAX, Y_MAX, PIN,
  NEO_MATRIX_TOP + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
  NEO_RGB + NEO_KHZ800);

// Initialize DHT
DHT dht(DHTPIN, DHTTYPE);

////////////////////////////////////////////////////////////////////////////////
// Some functions
////////////////////////////////////////////////////////////////////////////////
int getClimate(float &temp, float &hum){
  DBG_OUTPUT_PORT.println("Reading temperature.");
  temp = dht.readTemperature();
  hum = dht.readHumidity();
  // check if correct values are returned
  if (isnan(temp) || isnan(hum))
  {
    DBG_OUTPUT_PORT.println("ERROR: Failed to read from DHT sensor!");
    temp = 0;
    hum = 0;
    return 0;
  }
  return 1;
}
