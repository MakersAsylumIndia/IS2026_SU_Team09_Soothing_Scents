void setup() {
  pinMode(4, OUTPUT);
}

void loop() {
  digitalWrite(4, LOW);
  delay(3000);

  digitalWrite(4, HIGH);
  delay(3000);
}
