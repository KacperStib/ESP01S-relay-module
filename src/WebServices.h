#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>

#include "TimeFromNTP.h"
#include "Relays.h"

extern ESP8266WebServer server;

extern const char* ssid;
extern const char* password;

extern IPAddress local_IP;     
extern IPAddress gateway;        
extern IPAddress subnet;       
extern IPAddress primaryDNS;     
extern IPAddress secondaryDNS; 

void handleRoot();
void handleToggle();
void handleToggle2();

void wifi_setup();
void web_server_setup();

void setupOTA();