#include <Arduino.h>

#include "brain.h"

static const uint8_t RELAY_PIN = 7;
static const uint8_t SOIL_SENSOR_PIN = A0;

void setup() {
  brain_init(RELAY_PIN, SOIL_SENSOR_PIN);
}

void loop() {
  brain_loop();
}
