# Documentação Técnica - TTS_V2

## Arquitetura do Sistema

### Kernel TTS (Time Triggered Scheduler)

O kernel TTS_V2 implementa um escalonador cooperativo baseado em tempo com as seguintes características:

#### Estruturas de Dados

```c
typedef struct {
    void (*task)(void);  // Ponteiro para função da tarefa
    uint32_t period;     // Período de execução (em ticks)
    uint32_t delay;      // Delay até próxima execução
} TaskInfo;
```
# Documentação técnica - TTS_V2

## Arquitetura do sistema

### Kernel TTS (Time Triggered Scheduler)

O kernel `TTS_V2` é um escalonador cooperativo baseado em tempo com as seguintes características:

#### Estruturas de dados

```c
typedef struct {
    void (*task)(void);  /* Ponteiro para função da tarefa */
    uint32_t period;     /* Período de execução (em ticks) */
    uint32_t delay;      /* Delay até a próxima execução */
} TaskInfo;
```

#### API principal

| Função | Descrição | Parâmetros |
|--------|-----------|------------|
| `Task_Init()` | Inicializa o kernel | Nenhum |
| `Task_Add()` | Adiciona nova tarefa | `task`, `period`, `delay` |
| `Task_Dispatch()` | Executa tarefas prontas | Nenhum |
| `Task_Update()` | Atualiza contadores (ISR) | Nenhum |

### Temporização

- Resolução: 1 ms (SysTick ou timer configurado para 1 kHz)
- Precisão: ±1 ms

### Mapeamento de hardware

#### EFM32GG995F1024

| Periférico | Pinos | Função |
|------------|-------|--------|
| LEDs | PE2, PE3, PE4 | LED1, LED2, LED3 |
| Display 7-seg | PD0–PD6 | Segmentos A–G |
| ADC | PA0 | Canal 0 |

#### Arduino Uno (Wokwi)

| Periférico | Pinos | Função |
|------------|-------|--------|
| LEDs | D2, D3, D4 | LED1, LED2, LED3 |
| Display 7-seg | D5–D11 | Segmentos A–G |
| ADC | A0 | Canal analógico |

## Análise de desempenho

### Utilização estimada da CPU

| Tarefa | Período | Tempo de execução (estimado) | Observação |
|--------|---------|------------------------------|-----------|
| Display | 1000 ms | ~10 µs | Atualização de 7 segmentos |
| LED1 | 500 ms | ~5 µs | Toggle de GPIO |
| LED2 | 750 ms | ~5 µs | Toggle de GPIO |
| LED3 | 1200 ms | ~5 µs | Toggle de GPIO |
| ADC | 100 ms | ~20 µs | Conversão ADC (polling) |

### Considerações temporais

- Overhead do kernel: pequeno, procedimentos críticos executados em ISR são mínimos.
- Latência máxima: 1 ms (resolução do tick).

## Implementação dos drivers

### Driver de LED

Exemplo de operação direta em registradores (EFM32/AVR):

```c
/* EFM32 */
GPIO->P[4].DOUTTGL = leds;

/* AVR */
PORTD ^= (1 << pin);
```

### Driver do display 7 segmentos

- Padrões: array com padrões para 0–9.
- Tipo: cátodo comum (no exemplo fornecido).

### Driver ADC

- Resolução típica: 12 bits (dependendo do dispositivo alvo).
- Referência: VDD (ou AVCC no AVR).

## Debugging e monitoramento

### Métodos recomendados

1. LEDs de status para validar o escalonador em tempo real.
2. Display para verificação visual do contador.
3. Valores de ADC para validar entradas analógicas.

### Problemas comuns e soluções

| Problema | Sintoma | Ação recomendada |
|----------|---------|------------------|
| Jitter excessivo | Saídas piscando irregularmente | Verificar se ISR está sobrecarregada; reduzir trabalho no ISR |
| Display incorreto | Dígitos errados ou segmentos persistentes | Verificar padrões de segmentos e inicialização de GPIO |
| ADC com valor constante | Valor fixo sem variação | Verificar referência e pino de entrada |

## Extensões possíveis

- Adicionar prioridades de tarefa e escalonamento preemptivo.
- Integrar comunicação serial (UART) para debug e telemetria.
- Adicionar watchdog hardware para robustez.

## Compatibilidade

Plataformas alvo e observações:

- EFM32GG (Cortex-M3): alvo primário.
- Arduino Uno (ATmega328P): usado para simulação no Wokwi.
- Plataformas adicionais (STM32, ESP32) requerem adaptação do hardware abstraction layer.

## Requisitos mínimos

- RAM: ~512 bytes (dependendo das estruturas alocadas).
- Flash: ~4 KB (depende do compilador e opções de link).
- Timer: ao menos um timer de 16 bits para gerar ticks com precisão.
