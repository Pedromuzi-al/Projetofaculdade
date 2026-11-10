# Irrigador Automatico - PlatformIO

Projeto migrado para PlatformIO usando Arduino Uno e framework Arduino.

## Pinos

- Sensor de umidade: saida AO ligada ao A3; DO desconectado
- Rele da bomba: IN ligado ao A0; ativo em LOW
- VCC dos dois modulos: 5V; GND comum com o Arduino
- Monitor serial: 9600 bps

## Logica

- Abaixo de 40% de umidade: inicia a irrigacao.
- A bomba permanece ligada enquanto a irrigacao estiver ativa.
- Entre 40% e 60%, o estado atual da bomba e mantido para evitar acionamentos repetidos.
- Ao chegar em 60% de umidade: encerra a irrigacao.
- Se houver falha no sensor, a bomba e desligada por seguranca.

## Calibracao

Em `src/config.h`, ajuste estes valores conforme as leituras ADC reais do seu sensor. Na montagem atual foram medidos ADC 681 seco no ar e ADC 327 a 329 com a sonda na agua:

```cpp
static constexpr uint16_t SENSOR_DRY_ADC = 681;
static constexpr uint16_t SENSOR_WET_ADC = 328;
```

A conversao aceita ADC crescente ou decrescente. Use em SENSOR_DRY_ADC o valor medido seco e em SENSOR_WET_ADC o valor medido molhado. O monitor serial mostra ADC e porcentagem a 9600 baud. Os testes de plausibilidade nao garantem a deteccao de um sensor desconectado.

A porcentagem e uma escala relativa entre essas duas referencias, nao uma medicao absoluta da quantidade de agua no solo. Para irrigacao no solo, calibre tambem nas condicoes reais de uso.
