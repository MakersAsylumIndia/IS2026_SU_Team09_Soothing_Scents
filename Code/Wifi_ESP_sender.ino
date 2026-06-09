#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  client.setServer(mqtt_server, 1883);
}

void reconnect() {
  while (!client.connected()) {
    client.connect("ESP32_SENDER");
  }
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  client.publish("esp32/heartbeat", "1");
  delay(1000);
}
