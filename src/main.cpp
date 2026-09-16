#include <Arduino.h>

#include "brain.h"
#include "config.h"

void setup() {
  Serial.begin(config::SERIAL_BAUD_RATE);
  brain_init(config::RELAY_PIN, config::SOIL_SENSOR_PIN);
}

void loop() {
  brain_loop();
}
