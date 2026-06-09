//Detects heartbeat, filters out unrealistic readings, stores BPM values, calculates average heart rate, more stable BPM result on display

#define PULSE_PIN 5

const int threshold = 550;

unsigned long lastBeatTime = 0;
unsigned long lastPrintTime = 0;

bool pulseDetected = false;

const int NUM_READINGS = 5;
int bpmReadings[NUM_READINGS];
int index = 0;

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < NUM_READINGS; i++) {
    bpmReadings[i] = 0;
  }
}

void loop() {
  int signal = analogRead(PULSE_PIN);

  // Detect heartbeat on rising edge only
  if (signal > threshold && !pulseDetected) {

    pulseDetected = true;

    unsigned long currentTime = millis();

    if (lastBeatTime > 0) {

      unsigned long interval = currentTime - lastBeatTime;

      // Ignore unrealistic intervals
      if (interval > 300 && interval < 2000) {

        int bpm = 60000 / interval;

        bpmReadings[index] = bpm;
        index = (index + 1) % NUM_READINGS;
      }
    }

    lastBeatTime = currentTime;
  }

  // Reset detection when signal falls below threshold
  if (signal < threshold) {
    pulseDetected = false;
  }

  // Print averaged BPM every second
  if (millis() - lastPrintTime >= 1000) {

    int sum = 0;
    int count = 0;

    for (int i = 0; i < NUM_READINGS; i++) {
      if (bpmReadings[i] > 0) {
        sum += bpmReadings[i];
        count++;
      }
    }

    if (count > 0) {
      int avgBPM = sum / count;

      Serial.print("Heart Rate: ");
      Serial.print(avgBPM);
      Serial.println(" BPM");
    }

    lastPrintTime = millis();
  }

  delay(10);  // Sma#define PULSE_PIN 2

const int THRESHOLD = 1800;  // Adjust if needed

bool beatDetected = false;

unsigned long lastBeatTime = 0;
unsigned long lastPrintTime = 0;

int bpmHistory[5] = {0};
int bpmIndex = 0;

void setup() {
  Serial.begin(115200);
}

void loop() {

  int signal = analogRead(PULSE_PIN);

  // Detect rising edge crossing threshold
  if (signal > THRESHOLD && !beatDetected) {

    beatDetected = true;

    unsigned long currentTime = millis();

    if (lastBeatTime > 0) {

      unsigned long interval = currentTime - lastBeatTime;

      // Accept only realistic heart rates
      if (interval > 300 && interval < 1500) {

        int bpm = 60000 / interval;

        bpmHistory[bpmIndex] = bpm;
        bpmIndex = (bpmIndex + 1) % 5;
      }
    }

    lastBeatTime = currentTime;
  }

  // Reset detector when signal drops
  if (signal < THRESHOLD - 100) {
    beatDetected = false;
  }

  // Print averaged BPM every second
  if (millis() - lastPrintTime > 1000) {

    int sum = 0;
    int count = 0;

    for (int i = 0; i < 5; i++) {
      if (bpmHistory[i] > 0) {
        sum += bpmHistory[i];
        count++;
      }
    }

    if (count > 0) {
      int avgBPM = sum / count;

      Serial.print("Heart Rate: ");
      Serial.print(avgBPM);
      Serial.println(" BPM");
    } else {
      Serial.println("Place finger on sensor...");
    }

    lastPrintTime = millis();
  }

  delay(5);
}ll delay for stability
}
