#include <esp_now.h>
#include <WiFi.h>

uint8_t receiverMAC[] = {0x8C, 0x4F, 0x00, 0x2F, 0xDD, 0xD0};

typedef struct {
  int state;
} Message;

Message msg;

const int pulsePin = 34;
const int THRESHOLD = 2500;

bool beatDetected = false;
unsigned long lastBeatTime = 0;

float bpm = 0;
int lastSentState = -1;

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

  esp_now_add_peer(&peerInfo);
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

      if (bpm == 0)
        bpm = newBPM;
      else
        bpm = bpm * 0.8 + newBPM * 0.2;

      int state = (bpm > 100) ? 1 : 0;

      if (state != lastSentState) {

        msg.state = state;

        esp_now_send(receiverMAC,
                     (uint8_t *)&msg,
                     sizeof(msg));

        Serial.print("BPM: ");
        Serial.print((int)bpm);

        if (state == 1)
          Serial.println(" -> HIGH");
        else
          Serial.println(" -> LOW");

        lastSentState = state;
      }
    }
  }

  if (signal < THRESHOLD - 200)
    beatDetected = false;
}