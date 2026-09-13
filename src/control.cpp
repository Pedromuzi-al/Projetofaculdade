#include "control.h"

static const uint16_t SENSOR_DRY_ADC = 0;
static const uint16_t SENSOR_WET_ADC = 1023;

static uint8_t sensor_pin = A0;

void control_init(uint8_t soil_sensor_pin) {
  sensor_pin = soil_sensor_pin;
  Serial.begin(9600);
}

uint8_t control_read_soil_percent() {
  uint16_t adc = analogRead(sensor_pin);

  if (adc <= SENSOR_DRY_ADC) {
    return 0;
  }

  if (adc >= SENSOR_WET_ADC) {
    return 100;
  }

  return (uint8_t)(((uint32_t)(adc - SENSOR_DRY_ADC) * 100UL) /
                   (SENSOR_WET_ADC - SENSOR_DRY_ADC));
}
