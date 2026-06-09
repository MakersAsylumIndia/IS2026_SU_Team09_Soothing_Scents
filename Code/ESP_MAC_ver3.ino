#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println("Starting...");

  WiFi.mode(WIFI_STA);

  Serial.print("Mode set. MAC = ");
  Serial.println(WiFi.macAddress());
}

void loop() {}