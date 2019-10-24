/*
Website API functions for WS2812b Ledwall with webinterface
Jeroen Klock 17-10-2019
See: https://github.com/klockie86/LedWall
*/

#include <rest_api.h>

void handleNotFound(){
  if (!handleFileRead(server.uri())) {
    server.send(404, "text/plain", "FileNotFound");
  }
}


void setState() {
  String sRecState = server.arg("LEDstate"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
  iRecState = sRecState.toInt();
  DBG_OUTPUT_PORT.println("Received LEDstate: "+ sRecState);
  server.send(200, "text/plain", sRecState);
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
  server.send(200, "text/plain","");
}

void setText() {
  sText = server.arg("Text");
  DBG_OUTPUT_PORT.println("Reveived: "+sText);
  server.send(200, "text/plain",sText);
}

void setTextColor() {
  String r = server.arg("r");
  String g = server.arg("g");
  String b = server.arg("b");
  DBG_OUTPUT_PORT.println("Received text color: "+r+" "+b+" "+g);
  iTextColorR = r.toInt();
  iTextColorG = g.toInt();
  iTextColorB = b.toInt();
  server.send(200, "text/plain","");
}

void showTemp(){
  String show = server.arg("Show");
  DBG_OUTPUT_PORT.println("Received show temp: "+ show);
  if(show == "true"){
      bShowTemp = true;
  }
  else{
      bShowTemp = false;
  }
  server.send(200, "text/plain",show);
}

void setBrightness() {
  String sRec = server.arg("Brightness"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
  DBG_OUTPUT_PORT.println("Received Brightness: "+ sRec);
  matrix.setBrightness(sRec.toInt());
  matrix.show();
  server.send(200, "text/plain",sRec);
}

void readTemp() {
  float t;
  float h;
  String sTemp;

// read climate from dht
  getClimate(t,h);
  sTemp = "<span id=\"temp\">Temperatuur: "+String(t)+"&#8451 | </span><span id = \"hum\"> Luchtvochtigheid: "+String(h)+"%</span>";
  server.send(200, "text/html", sTemp);
}
