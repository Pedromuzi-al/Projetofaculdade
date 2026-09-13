# Irrigador Automatico - PlatformIO

Projeto migrado para PlatformIO usando Arduino Uno e framework Arduino.

## Pinos

- Sensor de umidade: A0
- Rele da bomba: pino digital 7
- Monitor serial: 9600 bps

## Logica

- Abaixo de 40% de umidade: inicia a irrigacao.
- A bomba liga em pulsos de 1 segundo.
- Entre pulsos, o sistema aguarda 5 segundos para a agua penetrar no solo.
- Ao chegar em 60% de umidade: encerra a irrigacao.
- Limite de seguranca: 20 pulsos.

## Calibracao

Em `src/control.cpp`, ajuste estes valores conforme as leituras reais do seu sensor:

```cpp
static const uint16_t SENSOR_DRY_ADC = 0;
static const uint16_t SENSOR_WET_ADC = 1023;
```

Se o seu sensor funcionar invertido, ou seja, valor alto quando esta seco e valor baixo quando esta molhado, a conversao em `control_read_soil_percent()` deve ser invertida.
