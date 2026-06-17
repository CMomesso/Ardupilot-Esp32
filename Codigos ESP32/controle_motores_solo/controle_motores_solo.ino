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

const byte START_BYTE = 0xAA;

enum EstadoDados{
  ESPERANDO_START,
  LENDO_HABILITADO,
  LENDO_VELOCIDADE,
  LENDO_DIRECAO,
  LENDO_CHECKSUM
};

EstadoDados estado = ESPERANDO_START;

byte habilitado = 0;
byte velocidade = 0;
byte direcao = 1;
byte checksum_recebido = 0;

unsigned long ultimoPacoteValido = 0;
const unsigned long TIMEOUT_COMUNICACAO = 500;

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
  
  receberPacote();

  if (millis() - ultimoPacoteValido > TIMEOUT_COMUNICACAO) {
    parar();
    digitalWrite(LED_PIN, LOW);
  }

}

void receberPacote() {
  while (PixhawkSerial.available()) {
    byte b = PixhawkSerial.read();

    switch (estado) {

      case ESPERANDO_START:
        if (b == START_BYTE) {
          estado = LENDO_HABILITADO;
        }
        break;

      case LENDO_HABILITADO:
        habilitado = b;
        estado = LENDO_VELOCIDADE;
        break;

      case LENDO_VELOCIDADE:
        velocidade = b;
        estado = LENDO_DIRECAO;
        break;

      case LENDO_DIRECAO:
        direcao = b;
        estado = LENDO_CHECKSUM;
        break;

      case LENDO_CHECKSUM:
        checksum_recebido = b;

        byte checksum_calculado;
        checksum_calculado = (habilitado + velocidade + direcao ) % 256;

        if (checksum_recebido == checksum_calculado) {
          ultimoPacoteValido = millis();
          executarComando();
        } else {
          Serial.println("Pacote corrompido");
        }

        estado = ESPERANDO_START;
        break;
    }
  }
}

void executarComando() {
  Serial.print("Habilitado: ");
  Serial.print(habilitado);

  Serial.print(" | Velocidade: ");
  Serial.print(velocidade);

  Serial.print(" | Direcao: ");
  Serial.println(direcao);

  if (habilitado == 0) {
    parar();
    return;
  }

  int pwmBase = 0;

  if (velocidade == 0) {
    pwmBase = 0;
  }
  else if (velocidade == 1){
    pwmBase = 90;
  } 
  else if (velocidade == 2) {
    pwmBase = 180;
  } 
  else if (velocidade == 3) {
    pwmBase = 250;
  } 
  else {
    pwmBase = 0;
  }

  if (direcao == 0) {
    frente(pwmBase);
  } 
  else if (direcao == 2) {
    tras(pwmBase);
  }
  else{
    frente(pwmBase);
  }
}


void frente(int pwmBase){
  ledcWrite(en, pwmBase);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
}

void tras(int pwmBase){
  ledcWrite(en, pwmBase);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
}

void parar(){
  ledcWrite(en, 0);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}




