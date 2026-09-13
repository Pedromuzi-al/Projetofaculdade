#ifndef BRAIN_H
#define BRAIN_H

#include <Arduino.h>

void brain_init(uint8_t relay_pin, uint8_t soil_sensor_pin);
void brain_loop();

#endif
