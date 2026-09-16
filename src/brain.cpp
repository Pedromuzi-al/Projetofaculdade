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
}  // namespace

void brain_init(uint8_t relay_pin, uint8_t soil_sensor_pin) {
  control_init(soil_sensor_pin);
  irrigation_init(relay_pin);
  session = {false, 0, IrrigationState::Idle, 0};

  Serial.println("Irrigador iniciado");
}

void brain_loop() {
  uint8_t humidity = control_read_soil_percent();

  Serial.print("Umidade=");
  Serial.print(humidity);
  Serial.println("%");

  if (!control_is_sensor_initialized()) {
    Serial.println("Falha no sensor: irrigacao segura OFF");
    irrigation_stop();
    session = {false, 0, IrrigationState::Idle, 0};
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
        humidity = control_read_soil_percent();

        Serial.print("Umidade apos pausa=");
        Serial.print(humidity);
        Serial.println("%");

        if (humidity >= config::STOP_IRRIGATION_PERCENT) {
          stop_session(true);
          break;
        }

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
