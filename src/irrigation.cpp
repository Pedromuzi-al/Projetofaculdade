#include "irrigation.h"

#include "config.h"

static uint8_t relay_pin = config::RELAY_PIN;
static bool relay_on = false;

void irrigation_init(uint8_t pin) {
  relay_pin = pin;
  digitalWrite(relay_pin, config::RELAY_ACTIVE_LEVEL == HIGH ? LOW : HIGH);
  pinMode(relay_pin, OUTPUT);
  irrigation_stop();
}

void irrigation_start() {
  if (relay_on) {
    return;
  }

  digitalWrite(relay_pin, config::RELAY_ACTIVE_LEVEL);
  relay_on = true;
}

void irrigation_stop() {
  digitalWrite(relay_pin, config::RELAY_ACTIVE_LEVEL == HIGH ? LOW : HIGH);
  relay_on = false;
}

bool irrigation_is_on() {
  return relay_on;
}
