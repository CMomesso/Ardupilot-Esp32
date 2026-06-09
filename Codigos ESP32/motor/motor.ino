// Motor A connections
int in1 = 22;  // IN1 pin
int in2 = 23;  // IN2 pin
int en = 13;

// PWM properties
freq = 1000;
resolution = 8;

void setup() {
  // Set all the motor control pins to outputs
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(en, OUTPUT);

  ledcAttach(en, freq, resolution);
  Serial.begin(115200);

  // testing
  Serial.print("Testing DC Motor...");

}

void loop() {

  ledcWrite(en, 250);

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  Serial.println("Moving Forward");
  delay(2000);

}
