HardwareSerial PixhawkSerial(2);

#define RXD2 26
#define TXD2 27
#define LED_PIN 2

void setup() {
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200); // USB para debug no PC

  PixhawkSerial.begin(
    57600,
    SERIAL_8N1,
    RXD2,
    TXD2
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