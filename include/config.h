#pragma once

#define INPUT_1_PIN 0
#define INPUT_2_PIN 1
#define INPUT_3_PIN 3
#define INPUT_4_PIN 4

#define INPUT_1_HIGH_TOPIC "mqttcontrolnode/input1/high"
#define INPUT_1_HIGH_PAYLOAD "released"
#define INPUT_1_LOW_TOPIC "mqttcontrolnode/input1/low"
#define INPUT_1_LOW_PAYLOAD "pressed"
#define INPUT_2_HIGH_TOPIC "mqttcontrolnode/input2/high"
#define INPUT_2_HIGH_PAYLOAD "released"
#define INPUT_2_LOW_TOPIC "mqttcontrolnode/input2/low"
#define INPUT_2_LOW_PAYLOAD "pressed"
#define INPUT_3_HIGH_TOPIC "mqttcontrolnode/input3/high"
#define INPUT_3_HIGH_PAYLOAD "released"
#define INPUT_3_LOW_TOPIC "mqttcontrolnode/input3/low"
#define INPUT_3_LOW_PAYLOAD "pressed"
#define INPUT_4_HIGH_TOPIC "mqttcontrolnode/input4/high"
#define INPUT_4_HIGH_PAYLOAD "released"
#define INPUT_4_LOW_TOPIC "mqttcontrolnode/input4/low"
#define INPUT_4_LOW_PAYLOAD "pressed"

#define WIFI_LED_PIN 8
#define WIFI_LED_ON LOW
#define WIFI_LED_OFF HIGH
#define DEBOUNCE_MS 50
