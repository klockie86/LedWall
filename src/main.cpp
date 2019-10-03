/*
WS2812b Ledwall with webinterface

Changes:
V0.0  Jeroen Klock 2-10-2019

*/

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
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>           //https://github.com/tzapu/WiFiManager

#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <FS.h>
#include <WebSockets.h>            //https://github.com/Links2004/arduinoWebSockets
#include <WebSocketsServer.h>

#include <WiFiClient.h>


#define PIN D2          // Output pin for data line leds
#define LENGTH 20       // Amount of leds width panel
#define WIDTH 15        // Amount of leds length panel
#define BRIGHTNESS 20   // LED Brightness 0 - 255


/*
// ***************************************************************************
// Instanciate HTTP(80) / WebSockets(81) Server
// ***************************************************************************
ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

// ***************************************************************************
// Include: OTA
// ***************************************************************************
#if defined(ENABLE_OTA)
  #if ENABLE_OTA == 1
    #include <ESP8266HTTPUpdateServer.h>
    ESP8266HTTPUpdateServer httpUpdater;
  #endif
  #if ENABLE_OTA == 0
    #include <WiFiUdp.h>
    #include <ArduinoOTA.h>
  #endif
#endif
*/
// MATRIX DECLARATION:
// Parameter 1 = width of NeoPixel matrix
// Parameter 2 = height of matrix
// Parameter 3 = pin number (most are valid)
// Parameter 4 = matrix layout flags, add together as needed:
//   NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
//     Position of the FIRST LED in the matrix; pick two, e.g.
//     NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
//   NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs are arranged in horizontal
//     rows or in vertical columns, respectively; pick one or the other.
//   NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns proceed
//     in the same order, or alternate lines reverse direction; pick one.
//   See example below for these values in action.
// Parameter 5 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(LENGTH, WIDTH, PIN,
  NEO_MATRIX_TOP + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
  NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200); // enable serial for debugging
  Serial.printf("\nInitializing LED matrix\n");
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(BRIGHTNESS);
}

int x    = matrix.width();
int pass = 0;

void loop() {
  static int i = 0;
  i++;
  //matrix.fillScreen(0);
  for(int y = 0; y < WIDTH; y++){
    for(int x = 0; x < LENGTH; x++){
      if(y & 1){
        matrix.setPixelColor(19 - x + y * 20, matrix.gamma32(matrix.ColorHSV((x + y) * 256 * 8 + i * 128 * 2)));
      }
      else{
        matrix.setPixelColor(x + y * 20,
        matrix.gamma32(matrix.ColorHSV((x + y) * 256 * 8+ i * 128 * 2)));
      }
    }
  }
  matrix.setTextColor(matrix.Color(0, 0, 0));
  matrix.setCursor(-((millis() / 30) & 127) + 20, 4);
  matrix.print(F("Omexom"));
  matrix.show();
}
