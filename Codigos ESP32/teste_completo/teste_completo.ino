HardwareSerial PixhawkSerial(2);  // Utilizar a UART2 do ESP32

// Pinos de comunicação com a Pixhawk
#define RXD2 26
#define TXD2 27
#define LED_PIN 2

// Pinos Motores
int in1 = 32;
int in2 = 33;
int en = 25;
// configurações Motores
String comando = "Inativo";

// Propriedades do PWM
int freq = 1000;
int resolution = 8;

// Criação do modulo de telemetria
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "CamilaLab";
const char* senha = "12345678";

WebServer server(80);

void paginaInicial() {

  String html = "<!DOCTYPE html>";
  html += "<html>";
  html += "<head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta http-equiv='refresh' content='1'>";
  html += "<title>Estação ESP32</title>";
  html += "</head>";
  html += "<body>";
  html += "<h1>Oie</h1>";
  html += "<p>Velocidade do motor:</p>";
  html += "<h2>";
  html += comando;
  html += "</h2>";
  html += "</body>";
  html += "</html>";

  server.send(200, "text/html", html);
}

void setup() {

  pinMode(LED_PIN, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(en, OUTPUT);

  Serial.begin(115200);

  PixhawkSerial.begin(  // Inicia a comunicação serial PixhawkSerial
    57600,              // velocidade de comunicação (baudrate) configurada no parâmetro SERIAL2_BAUD da Pixhawk
    SERIAL_8N1,         // formato dos dados seriais
    RXD2,               // pino RX
    TXD2                // Pino TX
  );

  ledcAttach(en, freq, resolution);
  Serial.begin(115200);

  WiFi.softAP(ssid, senha);
  server.on("/", paginaInicial);
  server.begin();

}

void loop() {
  server.handleClient();
  
  if (PixhawkSerial.available()){
    char c = PixhawkSerial.read();

    Serial.print("Recebido: ");
    Serial.print(c);

  // Teste LED

    if (c == 'B'){

      frente(2000);
      tras(2000);

    }
  }

}

void parar(int tempo){
  ledcWrite(en, 0);
  digitalWrite(in2, LOW);
  digitalWrite(in1, LOW);
  comando = "parado";
  delay(tempo);
}

void frente(int tempo){
  ledcWrite(en, 250);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  comando = "frente";
  delay(tempo);
}

void tras(int tempo){
  ledcWrite(en, 250);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  comando = "tras";
  delay(tempo);
}



