# SIM7600 Deep-Sleep SMS Alert

Provides a local Wi-Fi configuration page and uses a touch wakeup event to send an SMS alert with a SIM7600 modem.

## Technologies

ESP32, LilyGO SIM7600, deep sleep, SMS, Wi-Fi access point

## Requirements

Arduino core for ESP32 (`WiFi`, `Preferences`).

## Hardware and setup notes

LilyGO-oriented ESP32 + SIM7600 sketch. Configure the local access-point password before use. Avoid placing private phone numbers into source code or repository history.

## Run

1. Install the Arduino IDE and the required board support and libraries.
2. Open `SIM7600_DeepSleep_SMS_Alert.ino` from this folder.
3. Review pin assignments, the target board, and any network settings.
4. Replace placeholders (such as `YOUR_THINGSPEAK_WRITE_API_KEY`) with **your own settings locally**. Never commit real passwords or API keys.
5. Compile and upload to compatible hardware; monitor serial output where relevant.

> Note: This portfolio package is a cleaned copy of an existing project sketch, not a claim that it was retested on hardware or security-hardened.
