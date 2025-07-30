#include <Arduino.h>
#include "WebServices.h"

// Relays
bool relay1State = false;           
bool relay2State = false;                  

void setup() {
  Serial.begin(115200);

  // Relay pins
  pinMode(relay1Pin, OUTPUT);
  pinMode(relay2Pin, OUTPUT);
  digitalWrite(relay1Pin, LOW);   
  digitalWrite(relay2Pin, LOW);

  // Webservices
  wifi_setup();
  web_server_setup();
  setupOTA();
}

void loop() {
  server.handleClient();
  ArduinoOTA.handle();
}