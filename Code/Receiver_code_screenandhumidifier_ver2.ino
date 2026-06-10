#include <WiFi.h>
#include <esp_now.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1
);

const int relay1 = 4;   // Lemongrass
const int relay2 = 2;   // Japanese Rose

typedef struct {
  float bpm;
} Message;

Message incoming;

unsigned long startTime = 0;
bool timerRunning = false;

void showScreen(int bpm, String scent) {

  display.clearDisplay();

  display.drawRect(0, 0, 128, 16, WHITE);

  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(5, 4);
  display.print("BPM: ");
  display.print(bpm);

  display.setCursor(0, 24);
  display.println("SMART HUMIDIFIER");

  display.drawLine(0, 38, 128, 38, WHITE);

  display.setCursor(0, 48);
  display.print("Scent: ");
  display.println(scent);

  display.display();
}

void onReceive(
  const esp_now_recv_info_t *info,
  const uint8_t *data,
  int len) {

  memcpy(&incoming, data, sizeof(incoming));

  int bpmVal = (int)incoming.bpm;

  Serial.print("Received BPM: ");
  Serial.println(bpmVal);

  if (bpmVal > 100) {

    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, LOW);

    showScreen(
      bpmVal,
      "Japanese Rose"
    );
  }
  else {

    digitalWrite(relay1, LOW);
    digitalWrite(relay2, HIGH);

    showScreen(
      bpmVal,
      "Lemongrass"
    );
  }

  startTime = millis();
  timerRunning = true;
}

void setup() {

  Serial.begin(115200);

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);

  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);

  Wire.begin(25, 26);   // SDA, SCL

  if (!display.begin(
        SSD1306_SWITCHCAPVCC,
        0x3C)) {

    Serial.println("OLED FAILED");

    while (true);
  }

  display.clearDisplay();

  display.setTextSize(2);
  display.setCursor(10, 20);
  display.println("READY");

  display.display();

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW FAILED");
    return;
  }

  esp_now_register_recv_cb(onReceive);

  Serial.println("Receiver Ready");
}

void loop() {

  if (timerRunning &&
      millis() - startTime >= 20000) {

    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);

    display.clearDisplay();

    display.setTextSize(2);
    display.setCursor(25, 18);
    display.println("OFF");

    display.setTextSize(1);
    display.setCursor(10, 48);
    display.println("Waiting for BPM");

    display.display();

    timerRunning = false;

    Serial.println("20s COMPLETE - OFF");
  }
}