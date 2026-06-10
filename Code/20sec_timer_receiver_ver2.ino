#include <WiFi.h>
#include <esp_now.h>

const int relay1 = 4;
const int relay2 = 2;

typedef struct {
  float bpm;
} Message;

Message incoming;

unsigned long startTime = 0;
bool timerRunning = false;

void onReceive(const esp_now_recv_info_t *info,
               const uint8_t *data,
               int len) {

  memcpy(&incoming, data, sizeof(incoming));

  Serial.print("BPM: ");
  Serial.println((int)incoming.bpm);

  if (incoming.bpm > 100) {

    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, LOW);

    Serial.println("Humidifier 2 ON");
  }
  else {

    digitalWrite(relay1, LOW);
    digitalWrite(relay2, HIGH);

    Serial.println("Humidifier 1 ON");
  }

  startTime = millis();
  timerRunning = true;
}

void setup() {

  Serial.begin(115200);

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);

  // IMPORTANT: both OFF initially (active LOW relay)
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  esp_now_register_recv_cb(onReceive);

  Serial.println("Receiver Ready");
}

void loop() {

  if (timerRunning && millis() - startTime >= 20000) {

    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);

    timerRunning = false;

    Serial.println("20s DONE → ALL OFF");
  }
}