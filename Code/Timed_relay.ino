const int relay1 = 4; // D4
const int relay2 = 2; // D2

void setup() {
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);

  digitalWrite(relay1, HIGH); // OFF
  digitalWrite(relay2, HIGH); // OFF
}

void loop() {

  Serial.begin(115200);

  // Relay 1 ON
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, HIGH);
  delay(5000);

  // Relay 2 ON
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, LOW);
  delay(5000);
}