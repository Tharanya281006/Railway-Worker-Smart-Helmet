/*
  Smart Helmet-Based Railway Worker Warning System
  ESP32 #2: Helmet Bluetooth receiver + buzzer

  LOGIC:
  Receive "WARNING" -> buzzer ON.
  Receive "CLEAR"   -> buzzer OFF.

  ASSUMPTIONS TO VERIFY BEFORE UPLOAD:
  - Classic ESP32 board with Bluetooth Classic SPP support.
  - Active buzzer control input connected to GPIO 26.
  - Buzzer driver/transistor used if the buzzer current exceeds GPIO limits.
  - Both ESP32 boards must support Bluetooth Classic SPP.
  - This code initiates the Bluetooth connection to the external unit.

  This is a student prototype example, not a railway-certified safety device.
*/

#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

const int BUZZER_PIN = 26;
const unsigned long RECONNECT_INTERVAL_MS = 3000;

unsigned long lastReconnectAttempt = 0;
bool alertIsOn = false;

void setBuzzer(bool on) {
  digitalWrite(BUZZER_PIN, on ? HIGH : LOW);
  alertIsOn = on;
}

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);
  setBuzzer(false);

  // true = Bluetooth client/master mode
  if (!SerialBT.begin("Railway_Helmet_Unit", true)) {
    Serial.println("Bluetooth client could not start. Check ESP32 model.");
    while (true) {
      delay(1000);
    }
  }

  Serial.println("Helmet unit started.");
  Serial.println("Trying to connect to Railway_External_Unit...");
}

void loop() {
  if (!SerialBT.connected() &&
      millis() - lastReconnectAttempt >= RECONNECT_INTERVAL_MS) {
    lastReconnectAttempt = millis();

    Serial.println("Bluetooth connection attempt...");
    if (SerialBT.connect("Railway_External_Unit")) {
      Serial.println("Bluetooth connected.");
    } else {
      Serial.println("Connection failed. Will retry.");
    }
  }

  if (SerialBT.connected() && SerialBT.available()) {
    String message = SerialBT.readStringUntil('\n');
    message.trim();

    if (message == "WARNING") {
      if (!alertIsOn) {
        setBuzzer(true);
        Serial.println("WARNING received -> BUZZER ON");
      }
    } else if (message == "CLEAR") {
      if (alertIsOn) {
        setBuzzer(false);
        Serial.println("CLEAR received -> BUZZER OFF");
      }
    } else if (message.length() > 0) {
      Serial.print("Unknown message: ");
      Serial.println(message);
    }
  }

  // If Bluetooth disconnects while warning is active, keep the buzzer on.
  // This is a conservative demo behavior; review the behavior for your project.
  delay(20);
}
