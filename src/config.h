#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

namespace config {
static constexpr uint8_t RELAY_PIN = A2;
static constexpr uint8_t SOIL_SENSOR_PIN = A0;

static constexpr uint8_t START_IRRIGATION_PERCENT = 40;
static constexpr uint8_t STOP_IRRIGATION_PERCENT = 60;

static constexpr uint16_t SENSOR_DRY_ADC = 680;
static constexpr uint16_t SENSOR_WET_ADC = 15;

static constexpr uint32_t SERIAL_BAUD_RATE = 9600;
}  // namespace config

#endif
