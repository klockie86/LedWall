
// some general libraries
#include <Arduino.h>

#include "restAPI.h"
#include "lib/settings.h"
#include "lib/spiffs_webserver.h"

void handleNotFound(){
  if (!handleFileRead(server.uri())) {
    server.send(404, "text/plain", "FileNotFound");
  }
}

void setState() {
  String sRecState = server.arg("LEDstate"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
  iRecState = sRecState.toInt();
  DBG_OUTPUT_PORT.println("Received LEDstate: "+ sRecState);
}

void setBackColor() {
  String r = server.arg("r");
  String g = server.arg("g");
  String b = server.arg("b");
  DBG_OUTPUT_PORT.println("Received background color: "+r+" "+b+" "+g);
  iColorR = r.toInt();
  iColorG = g.toInt();
  iColorB = b.toInt();
  iRecState = colorpick; // set state to colorpick for statemachine
}

void setText() {
  String text = server.arg("Text");
  DBG_OUTPUT_PORT.println("Received text: "+ text);
}

void setTextColor() {
  String r = server.arg("r");
  String g = server.arg("g");
  String b = server.arg("b");
  DBG_OUTPUT_PORT.println("Received text color: "+r+" "+b+" "+g);
  iColorR = r.toInt();
  iColorG = g.toInt();
  iColorB = b.toInt();
  iRecState = colorpick; // set state to colorpick for statemachine
}

void showTemp(){
  String show = server.arg("Show");
  DBG_OUTPUT_PORT.println("Received show temp: "+ show);
  if (show == "true"){
      DBG_OUTPUT_PORT.println("showing temp");
  }
  else{
      DBG_OUTPUT_PORT.println("not showing temp");
  }

}

void setBrightness() {
  String sRec = server.arg("Brightness"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
  DBG_OUTPUT_PORT.println("Received Brightness: "+ sRec);
  matrix.setBrightness(sRec.toInt());
  matrix.show();
  server.send(200, "text/html", sRec);
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
