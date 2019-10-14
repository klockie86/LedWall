/*
WS2812b Ledwall with webinterface
Jeroen Klock 2-10-2019
See: https://github.com/klockie86/LedWall

Todo:
  - loop voor IP in scherm
  - colorpicker in Webinterface.
  - HTML pagina via SPIFFS
  - logo instellen.
  - OTA
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
// Custom header files
////////////////////////////////////////////////////////////////////////////////
#include "spiffs_webserver.h"
//#include "index.h"

////////////////////////////////////////////////////////////////////////////////
// Some functions
////////////////////////////////////////////////////////////////////////////////

void handleNotFound(){
  if (!handleFileRead(server.uri())) {
    server.send(404, "text/plain", "FileNotFound");
  }
}

void handleRoot() {
    DBG_OUTPUT_PORT.println("Webserver handleroot called.");
    if (!handleFileRead(server.uri())){
      DBG_OUTPUT_PORT.println("Webserver root file not found.");
      handleNotFound();
    }
}

void resetWifi() {
  DBG_OUTPUT_PORT.println("Resetting wifi");
//  wifiManager.resetSettings();
  server.send(200, "text/plain", "wifi has been reset, please connect with new accespoint."); //Send web page
  DBG_OUTPUT_PORT.println("Restarting ESP");
  ESP.restart();
}

// when command is received from webpages
void setState() {
  String sRecState = server.arg("LEDstate"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
  iRecState = sRecState.toInt();
  DBG_OUTPUT_PORT.println("Received LEDstate: "+ sRecState);
}

void setColor() {
  String r = server.arg("r");
  String g = server.arg("g");
  String b = server.arg("b");
  DBG_OUTPUT_PORT.println("Received Color: "+r+" "+b+" "+g);
  iColorR = r.toInt();
  iColorG = g.toInt();
  iColorB = b.toInt();
  iRecState = colorpick; // set state to colorpick for statemachine
}


// when command is received from webpages
void setBrightness() {
  String sRec = server.arg("Brightness"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
  DBG_OUTPUT_PORT.println("Received Brightness: "+ sRec);
  matrix.setBrightness(sRec.toInt());
  matrix.show();
  server.send(200, "text/html", sRec);
}



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

// when command is received from webpages
void readTemp() {
  float t;
  float h;
  String sTemp;

// read climate from dht
  getClimate(t,h);
  sTemp = "<span id=\"temp\">Temperatuur: "+String(t)+"&#8451 | </span><span id = \"hum\"> Luchtvochtigheid: "+String(h)+"%</span>";
  server.send(200, "text/html", sTemp);
}


////////////////////////////////////////////////////////////////////////////////
// Setup loop
////////////////////////////////////////////////////////////////////////////////
void setup() {
  // enable serial for debugging
  DBG_OUTPUT_PORT.begin(115200);

  // start LED matrix
  DBG_OUTPUT_PORT.println("Initializing LED matrix.");
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(BRIGHTNESS);
  matrix.fillScreen(matrix.Color(255,255,255));
  matrix.show();

  // start DHT sensor
  dht.begin();


  // get local IP and print on screen
  IPAddress ip = WiFi.localIP();
  DBG_OUTPUT_PORT.println("Print IP: "+ ip.toString() + " on LEDwall.");
  matrix.setTextColor(matrix.Color(0, 255, 0));
  matrix.setCursor(0, 0);   // start at top left, with one pixel of spacing
  matrix.setTextSize(1);    // size 1 == 8 pixels high
  matrix.print(ip.toString());
  matrix.show();

  // setup spiffs
  DBG_OUTPUT_PORT.println("Setting up spiffs");
  if(!SPIFFS.begin())
  {
      DBG_OUTPUT_PORT.println("ERROR - Setting up spiffs failed");
  }
  else
  {
    Dir dir = SPIFFS.openDir("");

    while (dir.next()) {
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      DBG_OUTPUT_PORT.printf("FS File: %s, size: %s\r\n", fileName.c_str(), formatBytes(fileSize).c_str());
    }
    FSInfo fs_info;
    SPIFFS.info(fs_info);
    DBG_OUTPUT_PORT.printf("FS Usage: %d/%d bytes\r\n", fs_info.usedBytes, fs_info.totalBytes);
  }

  // Init Wifimanager
  WiFiManager wifiManager;

  // start wifimanager
  wifiManager.autoConnect(NAME, PASSWORD);
  DBG_OUTPUT_PORT.println("Connected to Wifi.");
  DBG_OUTPUT_PORT.println("Starting server.");


  server.on("/", handleRoot);
  server.on("/resetWifi", resetWifi);
  server.on("/setState", setState);
  server.on("/setColor", setColor);
  server.on("/setBrightness", setBrightness);
  server.on("/readTemp", readTemp);

  server.onNotFound([]() {
    handleNotFound();
  });
  // start webserver
  server.begin();
  DBG_OUTPUT_PORT.println("Webserver started");
}

////////////////////////////////////////////////////////////////////////////////
// Mainloop
////////////////////////////////////////////////////////////////////////////////
void loop() {
  server.handleClient();

  static int i, lastTime = 0, curTime;
  String sTemp = "";
  if(iRecState != iCurrentState || iRecState == flipmode|| iRecState == colorpick){
    switch (iRecState){
      case off:
        matrix.fillScreen(matrix.Color(0,0,0));
        matrix.show();
        break;
      case red:
        matrix.fillScreen(matrix.Color(255,0,0));
        matrix.show();
        break;
      case green:
        matrix.fillScreen(matrix.Color(0,255,0));
        matrix.show();
        break;
      case blue:
        matrix.fillScreen(matrix.Color(0,0,255));
        matrix.show();
        break;
      case flipmode:
        i ++;
        // update temperature every 5 seconds;
        curTime = millis();
        if(lastTime == 0 || (curTime - lastTime > 2000)){
          float t;
          float h;
          String sTemp;
          // read climate from dht
          if(getClimate(t,h)){
            sTemp = "Klimaat: "+String(t)+" "+String(h);
          }
          else{
            sTemp = "";
          }
          lastTime = curTime;
        }
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
        matrix.print(BRANDNAME+sTemp);
        matrix.show();
        break;
      case colorpick:
        static int lastR, lastG, lastB;
        if (iColorR != lastR || iColorG != lastG || iColorB != lastB){
          matrix.fillScreen(matrix.Color(iColorR,iColorG,iColorB));
          matrix.show();
        }
        break;
      default:
        matrix.fillScreen(matrix.Color(255,255,255));
        matrix.show();
    }
    iCurrentState = iRecState;
    // send state back to server
    server.send(200, "text/plane", String(iRecState));
  }
}
