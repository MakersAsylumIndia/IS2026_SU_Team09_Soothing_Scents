#define PULSE_PIN 2   // D4 on many ESP32 boards

unsigned long lastBeat = 0;
int bpm = 0;

int threshold = 550;  // adjust if needed

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Pulse Sensor Test on D4 (GPIO 2)");
}

void loop() {
  int signal = analogRead(PULSE_PIN);

  Serial.print("Signal: ");
  Serial.print(signal);

  // heartbeat detection
  if (signal > threshold) {
    unsigned long now = millis();

    if (now - lastBeat > 300) { // debounce
      bpm = 60000 / (now - lastBeat);
      lastBeat = now;

      if (bpm > 30 && bpm < 220) {
        Serial.print("  | BPM: ");
        Serial.print(bpm);
      }
    }
  }

  Serial.println();
  delay(10);
}
