//shows only BPM, cleaner display

#define PULSE_PIN 2  // D4

unsigned long lastBeat = 0;
int bpm = 0;

int threshold = 550;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Heartbeat Monitor Ready");
}

void loop() {
  int signal = analogRead(PULSE_PIN);

  // detect beat
  if (signal > threshold) {
    unsigned long now = millis();

    if (now - lastBeat > 300) {
      bpm = 60000 / (now - lastBeat);
      lastBeat = now;

      if (bpm > 30 && bpm < 220) {
        // ONLY output one clean line
        Serial.print("Heartbeat: ");
        Serial.println(bpm);

        delay(500); // slows spam so you see clean values
      }
    }
  }
}
