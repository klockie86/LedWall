/*
WS2812b Ledwall with webinterface

Changes:
V0.0  Jeroen Klock 2-10-2019

Todo:
  - ip adres weergeven na initialisatie
  - colorpicker in Webinterface.
  - text in panel.
  - logo instellen.
*/
////////////////////////////////////////////////////////////////////////////////
// Global settings
////////////////////////////////////////////////////////////////////////////////
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

// DHT related stuff
//#include <DHT.h>

// custom .h files
#include "index.h"

////////////////////////////////////////////////////////////////////////////////
// Global vars
////////////////////////////////////////////////////////////////////////////////
int x    = X_MAX;
int pass = 0;

////////////////////////////////////////////////////////////////////////////////
// Initiation
////////////////////////////////////////////////////////////////////////////////
// Instanciate HTTP(80) / WebSockets(81) Server
ESP8266WebServer server(80);

// Instanciate NeoMatrix
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(X_MAX, Y_MAX, PIN,
  NEO_MATRIX_TOP + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
  NEO_RGB + NEO_KHZ800);

// Initialize DHT
// DHT dht(DHTPIN, DHTTYPE);

////////////////////////////////////////////////////////////////////////////////
// Some functions
////////////////////////////////////////////////////////////////////////////////
void handleRoot() {
  // when root page is called
   String s = MAIN_page; //Read HTML contents
   server.send(200, "text/html", s); //Send web page
}
// when command is received from webpages
void doSomething() {
  String ledState = "OFF";
  String t_state = server.arg("LEDstate"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
  DBG_OUTPUT_PORT.println("Received LEDstate: "+ t_state);
  if(t_state == "1"){
    matrix.fillScreen(matrix.Color(0,255,0));
    matrix.show();
  }
  else
  {
    matrix.fillScreen(matrix.Color(0, 0, 255));
    matrix.show();
  }
  server.send(200, "text/plane", ledState); //Send web page
}

////////////////////////////////////////////////////////////////////////////////
// Setup loop
////////////////////////////////////////////////////////////////////////////////
void setup() {
  DBG_OUTPUT_PORT.begin(115200); // enable serial for debugging
  DBG_OUTPUT_PORT.println("Initializing LED matrix.");
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(BRIGHTNESS);
  matrix.fillScreen(matrix.Color(255,255,255));
  matrix.show();
  DBG_OUTPUT_PORT.println("Setting up WifiManager.");
  WiFiManager wifiManager;

//  DBG_OUTPUT_PORT.println("Setting up DHT sensor.");
//  dht.begin();

// if stuck at booting while not connecting to wifi, uncommend line
// wifiManager.resetSettings();
  wifiManager.autoConnect(NAME, PASSWORD);
  DBG_OUTPUT_PORT.println("Connected to Wifi.");
  DBG_OUTPUT_PORT.println("Starting server.");

  // init webserver functions
  server.on("/", handleRoot);
  server.on("/setLED", doSomething);

  // start webserver
  server.begin();
  DBG_OUTPUT_PORT.println("Webserver started");
}

////////////////////////////////////////////////////////////////////////////////
// Mainloop
////////////////////////////////////////////////////////////////////////////////
void loop() {
  server.handleClient();
  // Wait a few seconds between measurements.

  /*
delay(2000);

float h = dht.readHumidity();
float t = dht.readTemperature();
float f = dht.readTemperature(true);

// Check if any reads failed and exit early (to try again).
if (isnan(h) || isnan(t) || isnan(f))
{
  Serial.println("Failed to read from DHT sensor!");
  return;
}

Serial.print("Humidity: ");
Serial.print(h);
Serial.print(" %\t");
Serial.print("Temperature: ");
Serial.print(t);
Serial.print(" *C ");
Serial.print(f);
Serial.println(" *F");
*/


//  matrix.fillScreen(matrix.Color(255, 0, 0));
/*
  for(int y = 0; y < Y_MAX; y++){
    for(int x = 0; x < X_MAX; x++){
      if(y & 1){
        matrix.setPixelColor(19 - x + y * 20, matrix.gamma32(matrix.ColorHSV((x + y) * 256 * 8 + i * 128 * 2)));
      }
      else{
        matrix.setPixelColor(x + y * 20,
        matrix.gamma32(matrix.ColorHSV((x + y) * 256 * 8+ i * 128 * 2)));
      }
    }
  }
  matrix.setTextColor(matrix.Color(0, 0, 255));
  matrix.setCursor(-((millis() / 30) & 127) + 20, 4);
  matrix.print(F("Omexom"));
  matrix.show();
*/
}
