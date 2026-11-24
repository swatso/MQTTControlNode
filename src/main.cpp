#include <Arduino.h>
#include <LittleFS.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include "config.h"

struct Input {
  uint8_t pin;
  const char *highTopic;
  const char *highPayload;
  const char *lowTopic;
  const char *lowPayload;
  int stableState;
  int lastReading;
  unsigned long lastChange;
  bool pending;
};

Input inputs[] = {
    {INPUT_1_PIN, INPUT_1_HIGH_TOPIC, INPUT_1_HIGH_PAYLOAD, INPUT_1_LOW_TOPIC, INPUT_1_LOW_PAYLOAD},
    {INPUT_2_PIN, INPUT_2_HIGH_TOPIC, INPUT_2_HIGH_PAYLOAD, INPUT_2_LOW_TOPIC, INPUT_2_LOW_PAYLOAD},
};

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);
String host, ssid, password;
bool configured = false;
unsigned long lastWifiAttempt = 0;
unsigned long lastMqttAttempt = 0;

bool readSetting(const char *path, String &value) {
  File file = LittleFS.open(path, "r");
  if (!file) {
    Serial.printf("Missing configuration file: %s\n", path);
    return false;
  }
  value = file.readString();
  while (value.endsWith("\n") || value.endsWith("\r")) {
    value.remove(value.length() - 1);
  }
  return true;
}

void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for serial to be ready
  delay(1000); // Give some time for the serial to initialize
  Serial.println("Starting setup...");
  pinMode(WIFI_LED_PIN, OUTPUT);
  digitalWrite(WIFI_LED_PIN, WIFI_LED_OFF);
  for (Input &input : inputs) {
    pinMode(input.pin, INPUT_PULLUP);
    input.stableState = input.lastReading = digitalRead(input.pin);
    input.lastChange = millis();
    input.pending = true;
  }
  if (!LittleFS.begin()) {
    Serial.println("Could not mount LittleFS");
    return;
  }
  bool haveHost = readSetting("/host.txt", host);
  bool haveSsid = readSetting("/ssid.txt", ssid);
  bool havePassword = readSetting("/pass.txt", password);
  if (!haveHost || !haveSsid || !havePassword || host.isEmpty() || ssid.isEmpty()) {
    Serial.println("Configure host.txt, ssid.txt and pass.txt in LittleFS");
    return;
  }

  WiFi.mode(WIFI_STA);
  if (!WiFi.setHostname(host.c_str())) {
    Serial.println("Invalid Wi-Fi hostname");
    return;
  }
  mqtt.setServer(IPAddress(192, 168, 0, 2), 1883);
  configured = true;
  WiFi.begin(ssid.c_str(), password.c_str());
  lastWifiAttempt = millis();
  Serial.println("Setup complete");
}

void loop() {
  const unsigned long now = millis();
  for (Input &input : inputs) {
    int reading = digitalRead(input.pin);
    if (reading != input.lastReading) {
      input.lastReading = reading;
      input.lastChange = now;
    }
    if (reading != input.stableState && now - input.lastChange >= DEBOUNCE_MS) {
      input.stableState = reading;
      input.pending = true;
    }
  }

  bool wifiConnected = configured && WiFi.status() == WL_CONNECTED;
  digitalWrite(WIFI_LED_PIN, wifiConnected ? WIFI_LED_ON : WIFI_LED_OFF);
  if (!configured) {
    return;
  }
  if (!wifiConnected) {
    if (now - lastWifiAttempt >= 10000) {
      WiFi.disconnect();
      WiFi.begin(ssid.c_str(), password.c_str());
      lastWifiAttempt = now;
    }
    return;
  }

  if (!mqtt.connected()) {
    if (now - lastMqttAttempt < 5000) {
      return;
    }
    lastMqttAttempt = now;
    if (!mqtt.connect(host.c_str())) {
      return;
    }
    for (Input &input : inputs) {
      input.pending = true;
    }
  }
  mqtt.loop();
  for (Input &input : inputs) {
    if (input.pending) {
      const bool high = input.stableState == HIGH;
      if (mqtt.publish(high ? input.highTopic : input.lowTopic,
                       high ? input.highPayload : input.lowPayload)) {
        input.pending = false;
      }
    }
  }
}