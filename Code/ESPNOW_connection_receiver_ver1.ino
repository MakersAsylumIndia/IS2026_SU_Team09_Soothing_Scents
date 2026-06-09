#include <WiFi.h>
#include <esp_now.h>

typedef struct struct_message {
  char text[32];
} struct_message;

struct_message incomingMessage;

void onReceive(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  memcpy(&incomingMessage, incomingData, sizeof(incomingMessage));

  Serial.print("Received: ");
  Serial.println(incomingMessage.text);
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

void loop() {}
