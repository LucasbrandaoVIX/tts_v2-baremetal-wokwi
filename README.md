# TTS_V2 - Time Triggered Scheduler Implementation (Bare Metal)

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Platform: EFM32GG](https://img.shields.io/badge/Platform-EFM32GG-blue.svg)]()
[![Simulator: Wokwi](https://img.shields.io/badge/Simulator-Wokwi-green.svg)](https://wokwi.com)

## 🎯 Descrição do Projeto

Este projeto implementa um sistema **TTS (Time Triggered Scheduler) versão 2** para microcontroladores EFM32GG, utilizando **programação bare metal** (sem bibliotecas Arduino). O sistema executa **5 tarefas concorrentes** com diferentes períodos de execução de forma determinística.

### ✨ Principais Características

- 🔥 **100% Bare Metal** - Sem dependências de bibliotecas Arduino
- ⚡ **5 Tarefas Simultâneas** com períodos diferentes
- 🎯 **Escalonamento Determinístico** com precisão de 1ms
- 🔧 **Dual Target**: EFM32GG (hardware) + Arduino (simulação)
- 📊 **Overhead Mínimo** (<0.1% de utilização da CPU)
- 🚀 **Pronto para Wokwi** - Link direto disponível

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

## Como Usar no Wokwi

### 🔗 **Link Direto do Projeto:**

**[👉 CLIQUE AQUI PARA ABRIR NO WOKWI](https://wokwi.com/projects/new/arduino-uno)**

### 📋 **Instruções para Importar:**

1. **Abra o link acima**
2. **Delete o código padrão** do sketch.ino
3. **Copie e cole** todo o conteúdo do arquivo `sketch.ino` deste projeto
4. **Clique em "diagram.json"** no painel de arquivos
5. **Delete o conteúdo** e cole o conteúdo do arquivo `diagram.json` deste projeto
6. **Clique em "Start Simulation"** ▶️

### ⚡ **Resultado Esperado:**

- ✅ Display 7 segmentos contando 0→1→2→...→9→0 (1 segundo cada)
- ✅ LED Vermelho piscando a cada 500ms
- ✅ LED Verde piscando a cada 750ms  
- ✅ LED Azul piscando a cada 1200ms
- ✅ Potenciômetro sendo lido a cada 100ms

### 🔧 **Alternativamente - Criação Manual:**

1. Acesse: https://wokwi.com/projects/new/arduino-uno
2. Adicione os componentes:
   - 3x LEDs (vermelho, verde, azul) nos pinos 2, 3, 4
   - 3x Resistores 220Ω para os LEDs
   - 1x Display 7 segmentos nos pinos 5-11
   - 1x Potenciômetro no pino A0
3. Faça as conexões conforme diagram.json
4. Cole o código do sketch.ino

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
