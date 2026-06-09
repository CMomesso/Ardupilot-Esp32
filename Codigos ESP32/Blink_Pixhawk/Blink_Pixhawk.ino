HardwareSerial PixhawkSerial(2);

#define RXD2 26
#define TXD2 27
#define LED_PIN 2

void setup() {
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200); // USB para debug no PC

  PixhawkSerial.begin(  // Inicia a comunicação serial PixhawkSerial
    57600,              // velocidade de comunicação (baudrate) configurada no parâmetro SERIAL2_BAUD da Pixhawk
    SERIAL_8N1,         // formato dos dados seriais
    RXD2,               // pino RX
    TXD2                // Pino TX
  );

  Serial.println("ESP32 pronto. Aguardando comando da Pixhawk...");
}

void loop() {
  if (PixhawkSerial.available()) {
    char c = PixhawkSerial.read();

    Serial.print("Recebido: ");
    Serial.println(c);

    if (c == 'B') {
      digitalWrite(LED_PIN, HIGH);
      delay(200);
      digitalWrite(LED_PIN, LOW);
      delay(200);
    }
  }
}