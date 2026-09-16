#include "control.h"

#include "config.h"

namespace {
static constexpr uint8_t SENSOR_SAMPLE_COUNT = 5;

uint8_t sensor_pin = config::SOIL_SENSOR_PIN;

uint16_t read_sensor_average() {
  uint32_t sum = 0;

  for (uint8_t i = 0; i < SENSOR_SAMPLE_COUNT; ++i) {
    sum += analogRead(sensor_pin);
    delay(10);
  }

  return static_cast<uint16_t>(sum / SENSOR_SAMPLE_COUNT);
}
}  // namespace

void control_init(uint8_t soil_sensor_pin) {
  sensor_pin = soil_sensor_pin;
  pinMode(sensor_pin, INPUT);
}

bool control_is_sensor_initialized() {
  return sensor_pin != 255;
}

uint8_t control_read_soil_percent() {
  if (!control_is_sensor_initialized()) {
    return 0;
  }

  uint16_t adc = read_sensor_average();
  const uint16_t dry_adc = config::SENSOR_DRY_ADC;
  const uint16_t wet_adc = config::SENSOR_WET_ADC;

  if (wet_adc <= dry_adc) {
    return 0;
  }

  if (adc <= dry_adc) {
    return 0;
  }

  if (adc >= wet_adc) {
    return 100;
  }

  uint32_t percent = ((uint32_t)(adc - dry_adc) * 100UL) / (wet_adc - dry_adc);
  return static_cast<uint8_t>(min(static_cast<uint32_t>(100), percent));
}
