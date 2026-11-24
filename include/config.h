#pragma once

#define INPUT_1_PIN 0
#define INPUT_2_PIN 1

#define INPUT_1_HIGH_TOPIC "track/turnout/node2B/Attach"
#define INPUT_1_HIGH_PAYLOAD "F"
#define INPUT_1_LOW_TOPIC "track/turnout/node2B/Attach"
#define INPUT_1_LOW_PAYLOAD "T"
#define INPUT_2_HIGH_TOPIC "track/turnout/node2B/KeepAlive"
#define INPUT_2_HIGH_PAYLOAD "F"
#define INPUT_2_LOW_TOPIC "track/turnout/node2B/KeepAlive"
#define INPUT_2_LOW_PAYLOAD "T"

#define WIFI_LED_PIN 8
#define WIFI_LED_ON LOW
#define WIFI_LED_OFF HIGH
#define DEBOUNCE_MS 50
