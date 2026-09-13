#ifndef IRRIGATION_H
#define IRRIGATION_H

#include <Arduino.h>

void irrigation_init(uint8_t relay_pin);
void irrigation_start();
void irrigation_stop();
bool irrigation_is_on();

#endif
