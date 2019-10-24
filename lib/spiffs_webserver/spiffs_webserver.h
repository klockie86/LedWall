// ***************************************************************************
// SPIFFS Webserver
// Source: https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer/examples/FSBrowser
// modified by Jeroen Klock 16-10-2019
// ***************************************************************************
#ifndef SPIFFS_WEBSERVER_H
#define SPIFFS_WEBSERVER_H

#include "globals.h"

extern File fsUploadFile;

String formatBytes(size_t);     // calculate filesize in byte
String getContentType(String);  // return content type from file
bool handleFileRead(String);    // read file for webserver
void handleFileUpload();
void handleFileDelete();
void handleFileCreate();
void handleFileList();

////////////////////////////////////////////////////////////////////////////////
#endif
