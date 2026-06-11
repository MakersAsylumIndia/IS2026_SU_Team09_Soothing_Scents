#include <WiFi.h>
#include <esp_now.h>

const int pulsePin = 34;
const int THRESHOLD = 2500;

bool beatDetected = false;

unsigned long lastBeatTime = 0;

float bpm = 0;
float displayedBPM = 0;

// RECEIVER MAC ADDRESS
uint8_t receiverMAC[] = {0x8C, 0x4F, 0x00, 0x2F, 0xDD, 0xD0};

typedef struct {
  int bpm;
} PulseData;

PulseData data;

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  Serial.println("Pulse Sensor Started");
  Serial.println("--------------------");
}

void loop() {

  int signal = analogRead(pulsePin);

  // Detect heartbeat
  if (signal > THRESHOLD && !beatDetected) {

    beatDetected = true;

    unsigned long now = millis();
    unsigned long beatInterval = now - lastBeatTime;

    lastBeatTime = now;

    // Accept only realistic BPM values
    if (beatInterval > 300 && beatInterval < 1500) {

      float newBPM = 60000.0 / beatInterval;

      // Smooth reading
      if (bpm == 0) {
        bpm = newBPM;
      } else {
        bpm = bpm * 0.8 + newBPM * 0.2;
      }

      data.bpm = (int)bpm;

      esp_now_send(receiverMAC, (uint8_t *)&data, sizeof(data));

      // Only display significant changes
      if (abs(bpm - displayedBPM) >= 10) {

        displayedBPM = bpm;

        Serial.println();
        Serial.println("====================");
        Serial.print("BPM: ");
        Serial.println((int)bpm);
        Serial.println("====================");
      }
    }
  }

  // Wait until signal drops before detecting next beat
  if (signal < THRESHOLD - 200) {
    beatDetected = false;
  }
}