# Irrigador Automatico - PlatformIO

Projeto migrado para PlatformIO usando Arduino Uno e framework Arduino.

## Pinos

- Sensor de umidade: saida AO ligada ao A3; DO desconectado
- Rele da bomba: IN ligado ao A0; ativo em LOW
- VCC dos dois modulos: 5V; GND comum com o Arduino
- Monitor serial: 9600 bps

## Ligacao da bateria e da bomba

Para uma bomba DC alimentada por bateria e um modulo rele com contatos COM,
NO/NA e NC/NF, o rele deve interromper o caminho da corrente da bomba:

```text
Bateria (+) ---- COM do rele
                 NO/NA do rele ---- Bomba (+)
Bateria (-) ----------------------- Bomba (-)
```

- COM: contato comum.
- NO/NA: normalmente aberto; conecta ao COM quando o rele e acionado.
- NC/NF: normalmente fechado; deixar sem fio nesta montagem.
- Use as marcacoes do modulo para identificar os bornes; a ordem fisica varia.
- Nao mantenha outro fio ligando diretamente bateria (+) a bomba (+), pois isso
  contorna o rele e mantem a bomba ligada mesmo com o comando OFF.
- A bateria alimenta a bomba; VCC, GND e IN do modulo servem ao controle do rele.
  Os contatos COM/NO nao fornecem energia por conta propria.
- A tensao da bateria deve ser compativel com a bomba, e os contatos do rele
  devem suportar a tensao e a corrente DC do motor, incluindo a partida.

Referencia sobre os contatos: [guia de reles da Seeed Studio](https://wiki.seeedstudio.com/Seeed_Relay_Page/).

Desconecte a bateria e a alimentacao do Arduino antes de alterar os fios.

### Verificacao na montagem

1. Confira a ligacao acima e os pinos de controle indicados em **Pinos**.
2. Ligue o sistema e abra o monitor serial em 9600 bps.
3. Abaixo de 40%, deve aparecer `Irrigacao: ON` e a bomba deve funcionar.
4. Ao chegar em 60%, deve aparecer `Irrigacao: OFF (60% atingidos)` e o motor
   da bomba deve parar mesmo com a bateria conectada.
5. Se o motor continuar girando, verifique fios contornando o rele, uso do borne
   NC/NF e se os contatos realmente abrem. O LED, o clique e as mensagens no
   serial nao comprovam que a alimentacao da bomba foi interrompida.

O firmware esta configurado para modulo **ativo em LOW**: A0 em LOW aciona;
A0 em HIGH desaciona. Confirme essa caracteristica no seu modulo antes de
alterar `RELAY_ACTIVE_LEVEL` em `src/config.h`.

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
