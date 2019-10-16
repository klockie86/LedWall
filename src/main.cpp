/*
WS2812b Ledwall with webinterface
Jeroen Klock 2-10-2019
See: https://github.com/klockie86/LedWall

Todo:
  - loop voor IP in scherm
  - logo instellen.
  - OTA
*/
////////////////////////////////////////////////////////////////////////////////
// Libraries
////////////////////////////////////////////////////////////////////////////////
#include "lib/settings.h"         // global settings, vars, and functions
#include "lib/spiffs_webserver.h" // spiffs related functions
#include "lib/restAPI.h"          // interaction with webpages



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

  //server.on("/", handleRoot);
  server.on("/setState", setState);
  server.on("/setBackColor", setBackColor);
  server.on("/setText", setText);
  server.on("/setTextColor", setTextColor);
  server.on("/showTemp", showTemp);
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
  // handler
  server.handleClient();

  // on firstrun show IP in screen
  static bool bFirstrun = true;
  static bool bOneShot = true;
  static IPAddress ip = WiFi.localIP();

  // on firstrun show IP in screen
  if(bFirstrun){
    // get local IP and print on screen
    if(bOneShot){
      DBG_OUTPUT_PORT.println("Print IP: "+ ip.toString() + " on LEDwall.");
    }
    matrix.setTextColor(matrix.Color(0, 255, 0));
    matrix.setCursor(-((millis() / 30) & 127) + 20, 0);
    matrix.setTextSize(1);    // size 1 == 8 pixels high
    matrix.print("Made by Jeroen Klock. IP: "+ip.toString());
    matrix.show();
  }

  static int i, lastTime = 0, curTime;
  String sTemp = "";
  if(iRecState != iCurrentState || iRecState == flipmode|| iRecState == colorpick){
    bFirstrun = false;
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
  bOneShot = false;
}
