#include <WiFi.h>
#include <esp_now.h>

uint8_t receiverMAC[] = {0x94, 0x51, 0xDC, 0x5B, 0xD9, 0xA8};

typedef struct {
  float bpm;
} Message;

Message msg;

const int pulsePin = 34;
const int THRESHOLD = 2500;

bool beatDetected = false;
unsigned long lastBeatTime = 0;
float bpm = 0;

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

  Serial.println("Sender Ready");
}

void loop() {

  int signal = analogRead(pulsePin);

  if (signal > THRESHOLD && !beatDetected) {

    beatDetected = true;

    unsigned long now = millis();
    unsigned long interval = now - lastBeatTime;

    lastBeatTime = now;

    if (interval > 300 && interval < 1500) {

      float newBPM = 60000.0 / interval;

      if (bpm == 0) {
        bpm = newBPM;
      } else {
        bpm = bpm * 0.8 + newBPM * 0.2;
      }

      msg.bpm = bpm;

      esp_now_send(
        receiverMAC,
        (uint8_t *)&msg,
        sizeof(msg)
      );

      Serial.print("BPM Sent: ");
      Serial.println((int)bpm);
    }
  }

  if (signal < THRESHOLD - 200) {
    beatDetected = false;
  }

  delay(5);
}