#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

// Wifi password
const char* ssid = "";
const char* password = "";

// Static IP
IPAddress local_IP(192, 168, 0, 150);     
IPAddress gateway(192, 168, 0, 1);        
IPAddress subnet(255, 255, 255, 0);       
IPAddress primaryDNS(192, 168, 0, 1);     
IPAddress secondaryDNS(8, 8, 4, 4);       

// Relays
bool relay1State = false;           
bool relay2State = false;          
const int relay1Pin = 0;           
const int relay2Pin = 2;            

// String + root
void handleRoot() {
  String html = F(
    "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>ESP8266</title>"
    "<meta name='viewport' content='width=device-width, initial-scale=1'>"
    "<style>body{font-family:sans-serif;max-width:540px;margin:24px auto;padding:0 12px}"
    "h1{font-size:20px} .card{border:1px solid #ddd;border-radius:8px;padding:12px;margin-bottom:12px}"
    "b{display:inline-block;min-width:160px} button{padding:8px 12px;border-radius:6px;border:1px solid #888;background:#f2f2f2;cursor:pointer}"
    "form{display:inline} .row{display:flex;justify-content:space-between;align-items:center}</style>"
    "</head><body><h1>Panel przekaźników (ESP8266)</h1>"
  );

  html += "<div class='card'><div class='row'><div><b>Przekaźnik 1 (GPIO0):</b> ";
  html += String(relay1State ? "WŁĄCZONY" : "WYŁĄCZONY");
  html += "</div><form action=\"/toggle\" method=\"POST\"><button type=\"submit\">Zmień</button></form></div></div>";

  html += "<div class='card'><div class='row'><div><b>Przekaźnik 2 (GPIO2):</b> ";
  html += String(relay2State ? "WŁĄCZONY" : "WYŁĄCZONY");
  html += "</div><form action=\"/toggle2\" method=\"POST\"><button type=\"submit\">Zmień</button></form></div></div>";

  html += "</body></html>";

  server.send(200, "text/html", html);
}

// Toggle relay 1
void handleToggle() {
  relay1State = !relay1State;
  digitalWrite(relay1Pin, relay1State ? HIGH : LOW); 
  server.sendHeader("Location", "/");
  server.send(303);
}

// Toggle relay 2
void handleToggle2() {
  relay2State = !relay2State;
  digitalWrite(relay2Pin, relay2State ? HIGH : LOW); 
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);

  // Relay pins
  pinMode(relay1Pin, OUTPUT);
  pinMode(relay2Pin, OUTPUT);
  digitalWrite(relay1Pin, LOW);   
  digitalWrite(relay2Pin, LOW);

  // Static IP conf
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("Błąd: nie udało się zastosować konfiguracji statycznego IP.");
  }

  // WiFi conf
  WiFi.begin(ssid, password);
  Serial.print("Łączenie z WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nPołączono z WiFi!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // Webserver conf
  server.on("/", HTTP_GET, handleRoot);
  server.on("/toggle", HTTP_POST, handleToggle);
  server.on("/toggle2", HTTP_POST, handleToggle2);

  server.begin();
  Serial.println("Serwer wystartował.");
}

void loop() {
  server.handleClient();
}