# TTS_V2 - Time Triggered Scheduler Implementation (Bare Metal)

Authors:
- Lucas Dantas Brandão
- Gusmar Gianordoli Santana

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Platform: EFM32GG](https://img.shields.io/badge/Platform-EFM32GG-blue.svg)]()
[![Simulator: Wokwi](https://img.shields.io/badge/Simulator-Wokwi-green.svg)](https://wokwi.com)

## Descrição do projeto

Este repositório contém a implementação do Time Triggered Scheduler (TTS) versão 2 para microcontroladores EFM32GG, desenvolvida em programação bare metal (sem uso de bibliotecas Arduino). O sistema executa cinco tarefas concorrentes com períodos distintos e comportamento determinístico.

### Principais características

- Implementação bare metal (sem bibliotecas Arduino)
- Execução de cinco tarefas simultâneas com períodos configuráveis
- Escalonamento determinístico com resolução de 1 ms
- Suporte a execução em hardware EFM32GG e simulação no Wokwi (adaptado)
- Uso de recursos mínimos de CPU

## Funcionalidades Implementadas

### Tarefas do Sistema:

1. **Display 7 Segmentos** - Período: 1000ms
   - Incrementa contador de 0 a 9 a cada segundo
   - Exibe o dígito atual no display 7 segmentos

2. **LED1 (Vermelho)** - Período: 500ms  
   - Pisca a cada 500 milissegundos
   - Delay inicial: 100ms

3. **LED2 (Verde)** - Período: 750ms
   - Pisca a cada 750 milissegundos  
   - Delay inicial: 200ms

4. **LED3 (Azul)** - Período: 1200ms
   - Pisca a cada 1200 milissegundos
   - Delay inicial: 300ms

5. **Leitura ADC** - Período: 100ms
   - Lê valor analógico do potenciômetro
   - Armazena valor em variável global
   - Delay inicial: 50ms

## Estrutura do Projeto

```
tts_v2-baremetal-wokwi/
├── src/                    # Código fonte
│   ├── main.c             # Arquivo principal com implementação das tarefas
│   ├── tt_tasks-v2.c      # Implementação do kernel TTS
│   ├── led.c              # Controle dos LEDs
│   ├── display.c          # Controle do display 7 segmentos
│   └── adc.c              # Controle do ADC
├── include/               # Arquivos de header
│   ├── tt_tasks.h         # Interface do kernel TTS
│   ├── led.h              # Interface dos LEDs
│   ├── display.h          # Interface do display
│   ├── adc.h              # Interface do ADC
│   └── config.h           # Configurações do projeto
├── wokwi/                 # Arquivos para simulação Wokwi
│   ├── sketch.ino         # Código adaptado para Arduino/Wokwi
│   ├── diagram.json       # Configuração do circuito
│   └── wokwi.toml         # Configuração do projeto Wokwi
├── docs/                  # Documentação
│   └── WOKWI_INSTRUCTIONS.md  # Instruções detalhadas para Wokwi
├── examples/              # Exemplos e extensões
│   └── extended_example.c # Exemplo de funcionalidades estendidas
├── build/                 # Diretório de compilação (gerado)
├── Makefile              # Script de compilação
└── README.md             # Este arquivo
```

## Características Técnicas

- **Microcontrolador**: EFM32GG995F1024 (Cortex-M3)
- **Frequência do SysTick**: 1 kHz (interrupção a cada 1ms)
- **Máximo de Tarefas**: 10 (configurável via TASK_MAXCNT)
- **Resolução do Timer**: 1ms
- **Programação**: 100% Bare Metal (sem Arduino libs)

## Pinout do Hardware

### LEDs:
- LED1 (Vermelho): PE2
- LED2 (Verde): PE3  
- LED3 (Azul): PE4

### Display 7 Segmentos:
- Segmento A: PD0
- Segmento B: PD1
- Segmento C: PD2
- Segmento D: PD3
- Segmento E: PD4
- Segmento F: PD5
- Segmento G: PD6
- Comum (Cátodo): GND

### ADC:
- Canal 0: PA0 (conectado ao potenciômetro)

## Uso no Wokwi

Link de criação de projeto no Wokwi:

[https://wokwi.com/projects/new/arduino-uno](https://wokwi.com/projects/new/arduino-uno)

Instruções de importação:

1. Abra o link acima.
2. Substitua o conteúdo padrão do editor pelo código do arquivo `wokwi/sketch.ino` deste repositório.
3. Substitua o conteúdo do arquivo `diagram.json` do projeto pelo arquivo `wokwi/diagram.json` deste repositório.
4. Inicie a simulação.

Resultado esperado:

- Display 7 segmentos: contador 0–9 com período de 1 s
- LED1: piscando a cada 500 ms
- LED2: piscando a cada 750 ms
- LED3: piscando a cada 1200 ms
- Leitura de ADC a cada 100 ms

Alternativa (criação manual do circuito no Wokwi):

1. Crie um novo projeto Arduino Uno em Wokwi.
2. Adicione os seguintes componentes e conexões conforme `wokwi/diagram.json`:
   - 3 LEDs nos pinos digitais 2, 3 e 4 com resistores de 220 Ω
   - 1 display 7 segmentos conectado aos pinos digitais 5–11
   - 1 potenciômetro conectado a A0
3. Cole o código do arquivo `wokwi/sketch.ino` no editor e inicie a simulação.

## Compilação

Para compilar o projeto (se usando toolchain local):

```bash
make clean
make all
```

## Funcionamento do TTS

O kernel TTS_V2 implementa:
- **Task_Init()**: Inicializa o sistema de tarefas
- **Task_Add()**: Adiciona nova tarefa com período e delay
- **Task_Dispatch()**: Executa tarefas prontas (chamado no loop principal)
- **Task_Update()**: Incrementa contador de ticks (chamado na interrupção)

### Algoritmo de Escalonamento:
1. SysTick gera interrupção a cada 1ms
2. Task_Update() incrementa contador global
3. Task_Dispatch() verifica tarefas prontas
4. Executa tarefas com delay = 0
5. Decrementa delay de todas as tarefas ativas

## Autores

Projeto desenvolvido para disciplina de Sistemas Embarcados - UFES
- Implementação TTS_V2 Bare Metal
- Suporte completo ao Wokwi Simulator
