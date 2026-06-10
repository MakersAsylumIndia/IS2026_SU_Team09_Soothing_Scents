#include <Wire.h>

void setup() {
  Serial.begin(115200);

  Wire.begin(26, 25);

  Serial.println("Scanning...");

  for (byte address = 1; address < 127; address++) {

    Wire.beginTransmission(address);

    if (Wire.endTransmission() == 0) {
      Serial.print("Found device at 0x");
      Serial.println(address, HEX);
    }
  }
}

void loop() {}