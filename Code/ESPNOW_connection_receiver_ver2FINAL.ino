#include <WiFi.h>
#include <esp_now.h>

typedef struct {
  int bpm;
} PulseData;

PulseData incomingData;

void onReceive(const esp_now_recv_info_t *info, const uint8_t *incomingDataBytes, int len) {
  memcpy(&incomingData, incomingDataBytes, sizeof(incomingData));

  Serial.print("Received BPM: ");
  Serial.println(incomingData.bpm);
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_register_recv_cb(onReceive);

  Serial.println("Receiver ready");
}

void loop() {
}
}