#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

namespace config {
static constexpr uint8_t RELAY_PIN = 7;
static constexpr uint8_t SOIL_SENSOR_PIN = A0;

static constexpr uint8_t START_IRRIGATION_PERCENT = 40;
static constexpr uint8_t STOP_IRRIGATION_PERCENT = 60;
static constexpr unsigned long IRRIGATION_PULSE_MS = 1000;
static constexpr unsigned long IRRIGATION_PAUSE_MS = 5000;
static constexpr uint8_t MAX_IRRIGATION_PULSES = 20;

static constexpr uint16_t SENSOR_DRY_ADC = 0;
static constexpr uint16_t SENSOR_WET_ADC = 1023;

static constexpr uint32_t SERIAL_BAUD_RATE = 9600;
}  // namespace config

#endif
