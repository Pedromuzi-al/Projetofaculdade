#include "brain.h"

#include "config.h"
#include "control.h"
#include "irrigation.h"

namespace {
enum class IrrigationState {
  Idle,
  PulseOn,
  PulseOff,
};

struct IrrigationSession {
  bool active;
  uint8_t pulse_count;
  IrrigationState state;
  unsigned long state_since_ms;
};

IrrigationSession session = {false, 0, IrrigationState::Idle, 0};
bool relay_test_mode = false;
bool relay_test_pulse_active = false;
unsigned long relay_test_started_ms = 0;

void stop_session(bool reached_target) {
  session.active = false;
  session.pulse_count = 0;
  session.state = IrrigationState::Idle;
  irrigation_stop();

  if (reached_target) {
    Serial.println("Irrigacao: OFF (60% atingidos)");
  } else {
    Serial.println("Irrigacao: OFF (limite de pulsos)");
  }
}

void start_session() {
  session.active = true;
  session.pulse_count = 0;
  session.state = IrrigationState::PulseOn;
  session.state_since_ms = millis();

  if (!irrigation_is_on()) {
    irrigation_start();
  }

  Serial.println("Irrigacao: ON");
}

void process_serial_commands() {
  while (Serial.available() > 0) {
    const char command = Serial.read();
    if (command == 'T' || command == 't') {
      session = {false, 0, IrrigationState::Idle, 0};
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
      session = {false, 0, IrrigationState::Idle, 0};
      Serial.println("Modo automatico ativado");
    }
  }
}
}  // namespace

void brain_init(uint8_t relay_pin, uint8_t soil_sensor_pin) {
  control_init(soil_sensor_pin);
  irrigation_init(relay_pin);
  session = {false, 0, IrrigationState::Idle, 0};
  relay_test_mode = false;
  relay_test_pulse_active = false;
  relay_test_started_ms = 0;

  Serial.println("Irrigador iniciado");
}

void brain_loop() {
  process_serial_commands();

  if (relay_test_pulse_active &&
      millis() - relay_test_started_ms >= config::IRRIGATION_PULSE_MS) {
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
    session = {false, 0, IrrigationState::Idle, 0};
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
      stop_session(true);
    } else {
      irrigation_stop();
    }
    return;
  }

  if (!session.active) {
    if (humidity < config::START_IRRIGATION_PERCENT) {
      start_session();
    }
    return;
  }

  const unsigned long now = millis();

  switch (session.state) {
    case IrrigationState::PulseOn:
      if (now - session.state_since_ms >= config::IRRIGATION_PULSE_MS) {
        irrigation_stop();
        session.pulse_count++;
        session.state = IrrigationState::PulseOff;
        session.state_since_ms = now;

        Serial.println("Pulso concluido; bomba OFF");
      }
      break;

    case IrrigationState::PulseOff:
      if (now - session.state_since_ms >= config::IRRIGATION_PAUSE_MS) {
        humidity = control_read_soil_percent(&sensor_adc);

        Serial.print("ADC apos pausa=");
        Serial.print(sensor_adc);
        Serial.print(" | ");
        Serial.print("Umidade apos pausa=");
        Serial.print(humidity);
        Serial.println("%");

        if (session.pulse_count >= config::MAX_IRRIGATION_PULSES) {
          stop_session(false);
          break;
        }

        session.state = IrrigationState::PulseOn;
        session.state_since_ms = now;

        if (!irrigation_is_on()) {
          irrigation_start();
        }

        Serial.println("Pulso iniciado; bomba ON");
      }
      break;

    case IrrigationState::Idle:
      break;
  }
}
