#include <WiFi.h>
#include <esp_now.h>
#include <U8g2lib.h>

// OLED
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(
  U8G2_R0,
  U8X8_PIN_NONE,
  25,
  26
);

// Relays (ACTIVE LOW)
#define RELAY1 4   // Lemongrass
#define RELAY2 5   // Japanese Rose

typedef struct {
  int bpm;   // MUST match sender
} Message;

Message incoming;

int bpmValue = 0;
int mode = 0;

bool running = false;
unsigned long startTime = 0;

// ---------------- OLED ----------------

void standbyScreen() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.drawStr(20, 35, "STANDBY");
  u8g2.sendBuffer();
}

void drawScreen(int timer) {

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tr);

  u8g2.setCursor(0, 12);
  u8g2.print("BPM: ");
  u8g2.print(bpmValue);

  if (mode == 0) {

    u8g2.setCursor(0, 28);
    u8g2.print("Scent: Lemongrass");

    u8g2.setCursor(0, 42);
    u8g2.print("Feeling: Energizing");

  } else {

    u8g2.setCursor(0, 28);
    u8g2.print("Scent: Japanese Rose");

    u8g2.setCursor(0, 42);
    u8g2.print("Feeling: Calming");
  }

  u8g2.setCursor(0, 60);
  u8g2.print("Timer: ");
  u8g2.print(timer);
  u8g2.print("s");

  u8g2.sendBuffer();
}

// ---------------- ESP-NOW ----------------

void onReceive(const esp_now_recv_info_t *info,
               const uint8_t *data,
               int len) {

  // Ignore packets while timer is active
  if (running) return;

  memcpy(&incoming, data, sizeof(incoming));

  bpmValue = incoming.bpm;

  Serial.print("Received BPM: ");
  Serial.println(bpmValue);

  // BPM > 100 = Rose
  // BPM <= 100 = Lemongrass
  mode = (bpmValue > 100) ? 1 : 0;

  // Turn both OFF first
  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);

  delay(50);

  if (mode == 0) {

    Serial.println("Lemongrass ON");
    digitalWrite(RELAY1, LOW);

  } else {

    Serial.println("Japanese Rose ON");
    digitalWrite(RELAY2, LOW);
  }

  startTime = millis();
  running = true;
}

// ---------------- SETUP ----------------

void setup() {

  Serial.begin(115200);

  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);

  // Both OFF
  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);

  u8g2.begin();
  standbyScreen();

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  esp_now_register_recv_cb(onReceive);

  Serial.println("Receiver Ready");
}

// ---------------- LOOP ----------------

void loop() {

  if (running) {

    int elapsed = (millis() - startTime) / 1000;
    int remaining = 10 - elapsed;

    if (remaining <= 0) {

      Serial.println("Turning everything OFF");

      digitalWrite(RELAY1, HIGH);
      digitalWrite(RELAY2, HIGH);

      running = false;

      standbyScreen();

    } else {

      drawScreen(remaining);
    }
  }

  delay(100);
}