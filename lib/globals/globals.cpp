/*
Global settings for WS2812b Ledwall with webinterface
Jeroen Klock 17-10-2019
See: https://github.com/klockie86/LedWall
*/

#include <globals.h>

////////////////////////////////////////////////////////////////////////////////
// Global vars
////////////////////////////////////////////////////////////////////////////////
enum state;
int iCurrentState, iRecState, iColorR, iColorG, iColorB, iTextColorR, iTextColorG, iTextColorB;
bool bShowTemp;
String sText = BRANDNAME;

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

// read temp from DHT and return
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
