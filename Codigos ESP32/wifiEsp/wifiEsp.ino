#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "CamilaESP32";
const char* password = "12345678";

WebServer server(80);

int valor = 0;

void paginaInicial() {
  valor++;

  String html = "<!DOCTYPE html>";
  html += "<html>";
  html += "<head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta http-equiv='refresh' content='1'>";
  html += "<title>ESP32</title>";
  html += "</head>";
  html += "<body>";
  html += "<h1>Olá, mundo!</h1>";
  html += "<p>Valor do ESP32:</p>";
  html += "<h2>";
  html += valor;
  html += "</h2>";
  html += "</body>";
  html += "</html>";

  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);

  WiFi.softAP(ssid, password);

  Serial.println("Wi-Fi criado!");
  Serial.print("Nome da rede: ");
  Serial.println(ssid);

  Serial.print("IP do ESP32: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", paginaInicial);

  server.begin();
  Serial.println("Servidor iniciado.");
}

void loop() {
  server.handleClient();
}