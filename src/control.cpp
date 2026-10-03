#include "control.h"

#include "config.h"

namespace {
static constexpr uint8_t SENSOR_SAMPLE_COUNT = 5;
static constexpr uint8_t SENSOR_CONNECTION_CHECK_SAMPLES = 10;
static constexpr uint16_t SENSOR_FLOATING_MIN_ADC = 5;
static constexpr uint16_t SENSOR_FLOATING_MAX_ADC = 1018;

uint8_t sensor_pin = config::SOIL_SENSOR_PIN;

uint16_t read_sensor_average() {
  uint32_t sum = 0;

  for (uint8_t i = 0; i < SENSOR_SAMPLE_COUNT; ++i) {
    sum += analogRead(sensor_pin);
    delay(10);
  }

  return static_cast<uint16_t>(sum / SENSOR_SAMPLE_COUNT);
}

bool is_sensor_reading_plausible(uint16_t adc) {
  return adc >= SENSOR_FLOATING_MIN_ADC && adc <= SENSOR_FLOATING_MAX_ADC;
}
}  // namespace

void control_init(uint8_t soil_sensor_pin) {
  sensor_pin = soil_sensor_pin;
  pinMode(sensor_pin, INPUT);
}

bool control_is_sensor_initialized() {
  return sensor_pin != 255;
}

bool control_is_sensor_connected() {
  if (!control_is_sensor_initialized()) {
    return false;
  }

  uint16_t min_adc = UINT16_MAX;
  uint16_t max_adc = 0;
  uint32_t sum = 0;

  for (uint8_t i = 0; i < SENSOR_CONNECTION_CHECK_SAMPLES; ++i) {
    const uint16_t adc = analogRead(sensor_pin);
    min_adc = min(min_adc, adc);
    max_adc = max(max_adc, adc);
    sum += adc;
    delay(10);
  }

  const uint16_t average_adc = static_cast<uint16_t>(sum / SENSOR_CONNECTION_CHECK_SAMPLES);
  const uint16_t span = max_adc - min_adc;

  if (!is_sensor_reading_plausible(average_adc)) {
    return false;
  }

  if (span > 900) {
    return false;
  }

  return true;
}

uint8_t control_read_soil_percent(uint16_t* adc_out) {
  if (!control_is_sensor_initialized() || !control_is_sensor_connected()) {
    if (adc_out != nullptr) {
      *adc_out = 0;
    }
    return 0;
  }

  const uint16_t adc = read_sensor_average();
  if (adc_out != nullptr) {
    *adc_out = adc;
  }

  const uint16_t dry_adc = config::SENSOR_DRY_ADC;
  const uint16_t wet_adc = config::SENSOR_WET_ADC;

  if (wet_adc == dry_adc) {
    return 0;
  }

  if (dry_adc < wet_adc) {
    if (adc <= dry_adc) {
      return 0;
    }

    if (adc >= wet_adc) {
      return 100;
    }

    const uint32_t percent =
        (static_cast<uint32_t>(adc - dry_adc) * 100UL) / (wet_adc - dry_adc);
    return static_cast<uint8_t>(percent);
  }

  if (adc >= dry_adc) {
    return 0;
  }

  if (adc <= wet_adc) {
    return 100;
  }

  const uint32_t percent =
      (static_cast<uint32_t>(dry_adc - adc) * 100UL) / (dry_adc - wet_adc);
  return static_cast<uint8_t>(percent);
}
