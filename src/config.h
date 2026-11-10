#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

namespace config {
static constexpr uint8_t RELAY_PIN = A0;
static constexpr uint8_t SOIL_SENSOR_PIN = A3;
static constexpr uint8_t RELAY_ACTIVE_LEVEL = LOW;

static constexpr uint8_t START_IRRIGATION_PERCENT = 40;
static constexpr uint8_t STOP_IRRIGATION_PERCENT = 60;

// Measured on this assembly: dry in air=681, wet in water=327..329.
// Relative scale between these references, not absolute soil water content.
static constexpr uint16_t SENSOR_DRY_ADC = 681;
static constexpr uint16_t SENSOR_WET_ADC = 328;

static constexpr uint32_t SERIAL_BAUD_RATE = 9600;
}  // namespace config

#endif
