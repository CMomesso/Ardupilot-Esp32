HardwareSerial PixhawkSerial(2);  // Utilizar a UART2 do ESP32

// Pinos de comunicação com a Pixhawk
#define RXD2 26
#define TXD2 27
#define LED_PIN 2

// Pinos Motores
int in1 = 32;
int in2 = 33;
int en = 25;

// Propriedades do PWM
int freq = 1000;
int resolution = 8;

void setup() {

  pinMode(LED_PIN, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(en, OUTPUT);

  PixhawkSerial.begin(  // Inicia a comunicação serial PixhawkSerial
    57600,              // velocidade de comunicação (baudrate) configurada no parâmetro SERIAL2_BAUD da Pixhawk
    SERIAL_8N1,         // formato dos dados seriais
    RXD2,               // pino RX
    TXD2                // Pino TX
  );

  ledcAttach(en, freq, resolution);
  Serial.begin(115200);

}

void loop() {
  
  if (PixhawkSerial.available()){
    char c = PixhawkSerial.read();

    Serial.print("Recebido: ");
    Serial.println(c);

    if (c == 'A') {
      frente();
      digitalWrite(LED_PIN, HIGH);
    }

    else if (c == 'C') {
      parar();
      digitalWrite(LED_PIN, LOW);
    }
  }

}

void parar(){
  ledcWrite(en, 0);
  digitalWrite(in2, LOW);
  digitalWrite(in1, LOW);
}

void frente(){
  ledcWrite(en, 250);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
}

void tras(){
  ledcWrite(en, 250);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
}




