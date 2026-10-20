#ifndef CONTROL_H
#define CONTROL_H

#include <Arduino.h>

void control_init(uint8_t soil_sensor_pin);
uint8_t control_read_soil_percent(uint16_t* adc_out = nullptr);
bool control_is_sensor_initialized();
bool control_is_sensor_connected();

#endif
