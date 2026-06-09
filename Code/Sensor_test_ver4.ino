//more stable and accurate readings, averages BPM readings to reduce fluctuations and noise

#define PULSE_PIN 15

int threshold = 550;

unsigned long lastBeat = 0;
int bpm = 0;

const int samples = 5;
int bpmBuffer[samples];
int idx = 0;

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < samples; i++) {
    bpmBuffer[i] = 0;
  }
}

void loop() {
  int signal = analogRead(PULSE_PIN);

  if (signal > threshold) {
    unsigned long now = millis();

    if (now - lastBeat > 300) {
      int instantBPM = 60000 / (now - lastBeat);
      lastBeat = now;

      if (instantBPM > 30 && instantBPM < 220) {
        bpmBuffer[idx] = instantBPM;
        idx = (idx + 1) % samples;

        int sum = 0;
        for (int i = 0; i < samples; i++) {
          sum += bpmBuffer[i];
        }

        bpm = sum / samples;

        Serial.println(bpm);

        delay(200);
      }
    }
  }
}