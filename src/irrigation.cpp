#include "irrigation.h"

static uint8_t relay_pin = 7;
static bool relay_on = false;

void irrigation_init(uint8_t pin) {
  relay_pin = pin;
  pinMode(relay_pin, OUTPUT);
  irrigation_stop();
}

void irrigation_start() {
  digitalWrite(relay_pin, HIGH);
  relay_on = true;
}

void irrigation_stop() {
  digitalWrite(relay_pin, LOW);
  relay_on = false;
}

bool irrigation_is_on() {
  return relay_on;
}
