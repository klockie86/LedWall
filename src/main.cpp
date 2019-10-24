/*
WS2812b Ledwall with webinterface
Jeroen Klock 2-10-2019
See: https://github.com/klockie86/LedWall

Todo:
 - Devide librarie files
*/

////////////////////////////////////////////////////////////////////////////////
// Custom header files
////////////////////////////////////////////////////////////////////////////////
#include "lib/globals.h"    // global settings
#include "lib/rest_api.h"   // webserver and interfacing
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
<<<<<<< HEAD
  static int iLastR, iLastG, iLastB;
  static int i, lastTime = 0, curTime;
  static String sLastText;
  switch (iRecState){
    case off:
      if (iCurrentState !=  iRecState){
        DBG_OUTPUT_PORT.println("Switching off display");
=======

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
>>>>>>> d79cc84c6dcb2638f8e71446da0886ea51a3b95e
        matrix.fillScreen(matrix.Color(0,0,0));
        matrix.show();
      }
      break;
    case red:
      if (iCurrentState !=  iRecState){
        DBG_OUTPUT_PORT.println("Switching display to red");
        matrix.fillScreen(matrix.Color(255,0,0));
        matrix.show();
      }
      break;
    case green:
      if (iCurrentState !=  iRecState){
        matrix.fillScreen(matrix.Color(0,255,0));
        DBG_OUTPUT_PORT.println("Switching display to green");
        matrix.show();
      }
      break;
    case blue:
      if (iCurrentState !=  iRecState){
        DBG_OUTPUT_PORT.println("Switching display to blue");
        matrix.fillScreen(matrix.Color(0,0,255));
        matrix.show();
      }
      break;
    case flipmode:
      i ++;
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
      break;
    case colorpick:
      if (iColorR != iLastR || iColorG != iLastG || iColorB != iLastB){
          matrix.fillScreen(matrix.Color(iColorR,iColorG,iColorB));
          matrix.show();
          DBG_OUTPUT_PORT.println("Custom backgroundcolor R:"+(String)iColorR+" G:"+(String)iColorG+" B:"+(String)iColorB);
      }
      iLastR = iColorR;
      iLastG = iColorG;
      iLastB = iColorB;
      break;
    default:
      matrix.fillScreen(matrix.Color(255,255,255));
      matrix.show();
  }
  if (bShowTemp){
    curTime = millis();
    if(lastTime == 0 || (curTime - lastTime > 2000)){
      float t, h;
      char cTemp[12];
      // read climate from dht
      if(getClimate(t,h)){
        // convert float to string with 1 decimal
        sprintf(cTemp, "%.1fC %.1f%%", t,h);

        // show temp in display
        matrix.setTextColor(matrix.Color(
                                 iTextColorR,
                                 iTextColorG,
                                 iTextColorB));
        matrix.setCursor(0, 4);
        matrix.print((String)cTemp);
        DBG_OUTPUT_PORT.println("Showing temp in screen:"+(String)cTemp);
        DBG_OUTPUT_PORT.println("Textcolor R:"+(String)iTextColorR+" G:"+(String)iTextColorG+" B:"+(String)iTextColorB);
      }
      else{
        sprintf(cTemp, "");
      }
      lastTime = curTime;
    }
  }
<<<<<<< HEAD
  if (sLastText != sText){
    matrix.setTextColor(matrix.Color(iTextColorR, iTextColorG, iTextColorB));
    matrix.setCursor(-((millis() / 30) & 127) + 20, 0);
    matrix.print(sText);
    matrix.show();
    DBG_OUTPUT_PORT.println("Showing text in screen:"+sText);
    DBG_OUTPUT_PORT.println("Textcolor R:"+(String)iTextColorR+" G:"+(String)iTextColorG+" B:"+(String)iTextColorB);
  }
  sLastText = sText;
  iCurrentState = iRecState;
=======
  bOneShot = false;
>>>>>>> d79cc84c6dcb2638f8e71446da0886ea51a3b95e
}
