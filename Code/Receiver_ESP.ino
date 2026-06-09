#include <esp_now.h>
#include <WiFi.h>

typedef struct {
  int state;
} Message;

Message incoming;

void onReceive(
  const esp_now_recv_info_t *info,
  const uint8_t *data,
  int len) {

  memcpy(&incoming, data, sizeof(incoming));

  if (incoming.state == 0) {

    // BPM < 100
    digitalWrite(4, LOW);   // Relay 1 ON
    digitalWrite(2, HIGH);  // Relay 2 OFF

    Serial.println("LOW BPM SCENT");
  }

  else {

    // BPM > 100
    digitalWrite(4, HIGH);  // Relay 1 OFF
    digitalWrite(2, LOW);   // Relay 2 ON

    Serial.println("HIGH BPM SCENT");
  }
}

void setup() {

  Serial.begin(115200);

  pinMode(4, OUTPUT);
  pinMode(2, OUTPUT);

  digitalWrite(4, HIGH);
  digitalWrite(2, HIGH);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  esp_now_register_recv_cb(onReceive);

  Serial.println("Receiver Ready");
}

void loop() {}
