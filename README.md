# Irrigador Automatico - PlatformIO

Projeto migrado para PlatformIO usando Arduino Uno e framework Arduino.

## Pinos

- Sensor de umidade: A0
- Rele da bomba: pino digital 7
- Monitor serial: 9600 bps

## Logica

- Abaixo de 40% de umidade: inicia a irrigacao.
- A bomba permanece ligada continuamente durante a irrigacao.
- Ao chegar em 60% de umidade: encerra a irrigacao.

## Diagnostico do rele

Com a bomba desconectada do modulo rele, envie `T` pelo monitor serial para
acionar a saida por 1 segundo. O programa fica em modo de teste, com a saida
desligada, ate receber `A`, que retoma o modo automatico. Nao envie `T` com a
bomba conectada: o teste energiza o rele.

O nivel ativo do rele e configurado em `src/config.h` (`RELAY_ACTIVE_LEVEL`).
O padrao `HIGH` preserva o comportamento anterior. Alguns modulos sao ativos em
`LOW`; confirme a especificacao do seu modulo antes de alterar essa opcao.

## Calibracao

O monitor serial mostra `ADC=` e `Umidade=`. Em `src/config.h`, ajuste
`SENSOR_DRY_ADC` e `SENSOR_WET_ADC` usando as leituras ADC reais com o sensor
seco e molhado, respectivamente:

```cpp
static constexpr uint16_t SENSOR_DRY_ADC = 0;
static constexpr uint16_t SENSOR_WET_ADC = 1023;
```

O programa aceita ADC crescente ou decrescente: se o valor diminuir ao molhar,
`SENSOR_WET_ADC` deve ser menor que `SENSOR_DRY_ADC`. A umidade exibida deve
aumentar ao molhar.
