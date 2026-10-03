# Irrigador Automatico - PlatformIO

Projeto migrado para PlatformIO usando Arduino Uno e framework Arduino.

## Pinos

- Sensor de umidade: A0
- Rele da bomba: pino A2 (usado como saida digital)
- Monitor serial: 9600 bps

## Logica

- Abaixo de 40% de umidade: liga a bomba continuamente.
- Ao chegar a 60% de umidade: desliga a bomba.
- Entre 40% e 60%, mantem o estado atual da bomba para evitar ligacoes repetidas perto do limite.

## Calibracao

Em `src/config.h`, ajuste estes valores conforme as leituras reais do seu sensor:

```cpp
static constexpr uint16_t SENSOR_DRY_ADC = 680;
static constexpr uint16_t SENSOR_WET_ADC = 15;
```

Neste sensor, o ADC fica alto quando esta seco e baixo quando esta molhado; a conversao aceita essa direcao invertida.
