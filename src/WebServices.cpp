#include "WebServices.h"

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

// String + root
void handleRoot() {
  String html = F(
    "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>ZAWORY OGRODOWE</title>"
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

// WiFi conf
void wifi_setup(){
 // Static IP conf
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("Błąd: nie udało się zastosować konfiguracji statycznego IP.");
  }
  // Wifi conf
  WiFi.begin(ssid, password);
  Serial.print("Łączenie z WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nPołączono z WiFi!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// Webserver conf
void web_server_setup(){
  server.on("/", HTTP_GET, handleRoot);
  server.on("/toggle", HTTP_POST, handleToggle);
  server.on("/toggle2", HTTP_POST, handleToggle2);

  server.begin();
  Serial.println("Serwer wystartował.");
}

// OTA update through WiFi
void setupOTA() {
  ArduinoOTA.onStart([]() {
    String type = (ArduinoOTA.getCommand() == U_FLASH) ? "flash" : "filesystem";
    Serial.println("Rozpoczęto aktualizację: " + type);
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("\nAktualizacja zakończona!");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Postęp: %u%%\r", (progress * 100) / total);
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Błąd [%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Błąd autoryzacji");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Błąd inicjalizacji");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Błąd połączenia");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Błąd odbioru");
    else if (error == OTA_END_ERROR) Serial.println("Błąd zakończenia");
  });

  ArduinoOTA.begin();
  Serial.println("OTA gotowe, czekam na aktualizacje...");
}