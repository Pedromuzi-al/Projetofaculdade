#include "brain.h"

#include "config.h"
#include "control.h"
#include "irrigation.h"

namespace {
struct IrrigationSession {
  bool active;
};

IrrigationSession session = {false};
bool relay_test_mode = false;
bool relay_test_pulse_active = false;
unsigned long relay_test_started_ms = 0;

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

void process_serial_commands() {
  while (Serial.available() > 0) {
    const char command = Serial.read();
    if (command == 'T' || command == 't') {
      session = {false};
      irrigation_stop();
      relay_test_mode = true;
      relay_test_pulse_active = true;
      relay_test_started_ms = millis();
      irrigation_start();
      Serial.println("TESTE: rele ON por 1s; desconecte a bomba antes do teste");
    } else if (command == 'A' || command == 'a') {
      if (relay_test_pulse_active) {
        irrigation_stop();
      }
      relay_test_mode = false;
      relay_test_pulse_active = false;
      session = {false};
      Serial.println("Modo automatico ativado");
    }
  }
}
}  // namespace

void brain_init(uint8_t relay_pin, uint8_t soil_sensor_pin) {
  control_init(soil_sensor_pin);
  irrigation_init(relay_pin);
  session = {false};
  relay_test_mode = false;
  relay_test_pulse_active = false;
  relay_test_started_ms = 0;

  Serial.println("Irrigador iniciado");
}

void brain_loop() {
  process_serial_commands();

  if (relay_test_pulse_active &&
      millis() - relay_test_started_ms >= config::RELAY_TEST_DURATION_MS) {
    irrigation_stop();
    relay_test_pulse_active = false;
    Serial.println("TESTE: rele OFF");
  }

  if (relay_test_mode) {
    return;
  }

  if (!control_is_sensor_initialized() || !control_is_sensor_connected()) {
    Serial.println("Falha no sensor: irrigacao segura OFF");
    irrigation_stop();
    session = {false};
    return;
  }

  uint16_t sensor_adc = 0;
  uint8_t humidity = control_read_soil_percent(&sensor_adc);

  Serial.print("ADC=");
  Serial.print(sensor_adc);
  Serial.print(" | ");
  Serial.print("Umidade=");
  Serial.print(humidity);
  Serial.println("%");

  if (humidity >= config::STOP_IRRIGATION_PERCENT) {
    if (session.active) {
      stop_session();
    } else {
      irrigation_stop();
    }
    return;
  }

  if (!session.active) {
    if (humidity < config::START_IRRIGATION_PERCENT) {
      start_session();
    }
  }
}
