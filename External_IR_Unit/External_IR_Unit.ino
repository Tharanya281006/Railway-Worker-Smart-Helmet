/*
  Smart Helmet-Based Railway Worker Warning System
  ESP32 #1: External IR sensor unit

  LOGIC:
  IR detects an object/event -> send "WARNING" over Bluetooth.
  IR clear -> send "CLEAR" over Bluetooth.

  ASSUMPTIONS TO VERIFY BEFORE UPLOAD:
  - Classic ESP32 board with Bluetooth Classic SPP support.
  - IR sensor digital OUT connected to GPIO 27.
  - Many IR modules output LOW on detection. If yours outputs HIGH,
    change SENSOR_DETECTED_LEVEL from LOW to HIGH.
  - Both ESP32 boards must share a compatible Bluetooth Classic connection.

  This is a student prototype example, not a railway-certified safety device.
*/

#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

const int IR_SENSOR_PIN = 27;
const int SENSOR_DETECTED_LEVEL = LOW;
const unsigned long MESSAGE_INTERVAL_MS = 250;

unsigned long lastMessageTime = 0;
bool previousDetected = false;

void setup() {
  Serial.begin(115200);
  pinMode(IR_SENSOR_PIN, INPUT);

  if (!SerialBT.begin("Railway_External_Unit")) {
    Serial.println("Bluetooth could not start. Check ESP32 model.");
    while (true) {
      delay(1000);
    }
  }

  Serial.println("External IR unit started.");
  Serial.println("Bluetooth name: Railway_External_Unit");
  Serial.println("Waiting for helmet ESP32...");
}

void loop() {
  bool detected = (digitalRead(IR_SENSOR_PIN) == SENSOR_DETECTED_LEVEL);

  // Print only when the sensor state changes.
  if (detected != previousDetected) {
    Serial.println(detected ? "IR: DETECTED" : "IR: CLEAR");
    previousDetected = detected;
  }

  // Send the current state periodically, but only when connected.
  if (SerialBT.hasClient() &&
      millis() - lastMessageTime >= MESSAGE_INTERVAL_MS) {
    lastMessageTime = millis();

    if (detected) {
      SerialBT.println("WARNING");
      Serial.println("Bluetooth TX: WARNING");
    } else {
      SerialBT.println("CLEAR");
      Serial.println("Bluetooth TX: CLEAR");
    }
  }

  delay(20);
}
