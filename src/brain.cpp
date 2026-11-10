#include "brain.h"

#include "config.h"
#include "control.h"
#include "irrigation.h"

namespace {
struct IrrigationSession {
  bool active;
};

IrrigationSession session = {false};

void stop_session() {
  session.active = false;
  irrigation_stop();
  Serial.println("Irrigacao: OFF (60% atingidos)");
}

void start_session() {
  session.active = true;
  irrigation_start();
  Serial.println("Irrigacao: ON");
}
}  // namespace

void brain_init(uint8_t relay_pin, uint8_t soil_sensor_pin) {
  control_init(soil_sensor_pin);
  irrigation_init(relay_pin);
  session = {false};

  Serial.println("Irrigador iniciado");
  Serial.print("Pinos: rele=A0 (ativo LOW), sensor=A3; calibracao seco=");
  Serial.print(config::SENSOR_DRY_ADC);
  Serial.print(", molhado=");
  Serial.println(config::SENSOR_WET_ADC);
}

void brain_loop() {
  uint16_t sensor_adc = UINT16_MAX;
  const uint8_t humidity = control_read_soil_percent(&sensor_adc);
  if (sensor_adc == UINT16_MAX) {
    Serial.println("Falha no sensor: irrigacao segura OFF");
    irrigation_stop();
    session = {false};
    return;
  }

  Serial.print("ADC=");
  Serial.print(sensor_adc);
  Serial.print(" | ");
  Serial.print("Umidade=");
  Serial.print(humidity);
  Serial.println("%");
  Serial.print("Rele antes da decisao: ");
  Serial.println(irrigation_is_on() ? "ON" : "OFF");

  if (!session.active) {
    if (humidity < config::START_IRRIGATION_PERCENT) {
      start_session();
    }
    return;
  }

  if (humidity >= config::STOP_IRRIGATION_PERCENT) {
    stop_session();
  }
}
