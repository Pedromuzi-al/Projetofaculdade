#include "brain.h"

#include "config.h"
#include "control.h"
#include "irrigation.h"

void brain_init(uint8_t relay_pin, uint8_t soil_sensor_pin) {
  control_init(soil_sensor_pin);
  irrigation_init(relay_pin);

  Serial.println("Irrigador iniciado");
}

void brain_loop() {
  if (!control_is_sensor_initialized() || !control_is_sensor_connected()) {
    Serial.println("Falha no sensor: irrigacao segura OFF");
    irrigation_stop();
    return;
  }

  uint8_t humidity = control_read_soil_percent();

  Serial.print("Umidade=");
  Serial.print(humidity);
  Serial.println("%");

  if (!irrigation_is_on() && humidity < config::START_IRRIGATION_PERCENT) {
    irrigation_start();
    Serial.println("Irrigacao: ON");
    return;
  }

  if (irrigation_is_on() && humidity >= config::STOP_IRRIGATION_PERCENT) {
    irrigation_stop();
    Serial.println("Irrigacao: OFF (60% atingidos)");
  }
}
