# Smart Helmet-Based Railway Worker Warning System (Bluetooth prototype)

## Concept
ESP32 #1 reads the digital output of an IR sensor. When the sensor reports detection, it sends `WARNING` over Bluetooth. When the sensor is clear, it sends `CLEAR`. ESP32 #2 receives these messages and turns the helmet buzzer on/off.

## Included files
- `External_IR_Unit/External_IR_Unit.ino`
- `Helmet_Buzzer_Unit/Helmet_Buzzer_Unit.ino`

## Critical: check hardware before uploading
The photo and description do not identify exact ESP32 models, pin numbers, IR sensor output polarity, or buzzer electrical requirements. The GPIO choices below are assumptions, not confirmed wiring. Check your actual connections first.

### Assumed pins
- External ESP32: IR sensor digital OUT -> GPIO 27
- Helmet ESP32: active buzzer control -> GPIO 26

Use the correct supply voltage for the IR module. Do not apply 5 V to ESP32 GPIOs. If the buzzer draws more current than an ESP32 GPIO can safely supply, use a suitable transistor/driver circuit. Make sure the circuit uses appropriate common ground connections.

## Bluetooth compatibility
This example uses Arduino's `BluetoothSerial` (Bluetooth Classic SPP). It generally requires an original ESP32 variant that supports Bluetooth Classic. ESP32-C3 does not support Bluetooth Classic SPP; other board variants must be checked. Both boards need compatible Bluetooth support.

## Arduino IDE steps
1. Install Arduino IDE and the Espressif ESP32 board package.
2. Select the correct board and COM port for ESP32 #1.
3. Upload `External_IR_Unit.ino` to ESP32 #1.
4. Select the correct board and COM port for ESP32 #2.
5. Upload `Helmet_Buzzer_Unit.ino` to ESP32 #2.
6. Open both Serial Monitors at 115200 baud.
7. Power both boards and check that the helmet unit connects to `Railway_External_Unit`.
8. Test the IR sensor with a safe object and verify whether the helmet buzzer responds.
9. If the IR output logic is reversed, change `SENSOR_DETECTED_LEVEL` from LOW to HIGH and upload again.

## Expected serial messages (only after a real test)
External unit may print:
- `IR: DETECTED`
- `Bluetooth TX: WARNING`

Helmet unit may print:
- `Bluetooth connected.`
- `WARNING received -> BUZZER ON`

These are expected message formats from the code, not proof that the hardware has been tested. Use only output actually observed during your test in your review presentation.

## Current scope and limitations
This prototype example handles only IR detection -> Bluetooth message -> buzzer. It does not implement LoRa, multi-hop relays, RSSI/SNR/PDR measurement, adaptive routing, or alternate-path recovery. It is a student prototype, not a certified railway safety device.
