#include "brain.h"

#include "control.h"
#include "irrigation.h"

static const uint8_t START_IRRIGATION_PERCENT = 40;
static const uint8_t STOP_IRRIGATION_PERCENT = 60;
static const unsigned long IRRIGATION_PULSE_MS = 1000;
static const unsigned long IRRIGATION_PAUSE_MS = 5000;
static const uint8_t MAX_IRRIGATION_PULSES = 20;

static bool irrigating = false;

void brain_init(uint8_t relay_pin, uint8_t soil_sensor_pin) {
  control_init(soil_sensor_pin);
  irrigation_init(relay_pin);
  irrigating = false;

  Serial.println("Irrigador iniciado");
}

void brain_loop() {
  uint8_t humidity = control_read_soil_percent();

  Serial.print("Umidade=");
  Serial.print(humidity);
  Serial.println("%");

  if (!irrigating) {
    if (humidity < START_IRRIGATION_PERCENT) {
      irrigation_start();
      irrigating = true;
      Serial.println("Irrigacao: ON");
    }
  } else {
    uint8_t pulse_count = 0;

    while (humidity < STOP_IRRIGATION_PERCENT &&
           pulse_count < MAX_IRRIGATION_PULSES) {
      irrigation_start();
      delay(IRRIGATION_PULSE_MS);
      irrigation_stop();
      pulse_count++;

      Serial.println("Pulso concluido; bomba OFF");

      delay(IRRIGATION_PAUSE_MS);
      humidity = control_read_soil_percent();

      Serial.print("Umidade apos pausa=");
      Serial.print(humidity);
      Serial.println("%");
    }

    irrigating = false;

    if (humidity >= STOP_IRRIGATION_PERCENT) {
      Serial.println("Irrigacao: OFF (60% atingidos)");
    } else {
      Serial.println("Irrigacao: OFF (limite de pulsos)");
    }
  }

  delay(1000);
}
