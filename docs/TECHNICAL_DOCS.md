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

#### API Principal

| Função | Descrição | Parâmetros |
|--------|-----------|------------|
| `Task_Init()` | Inicializa o kernel | Nenhum |
| `Task_Add()` | Adiciona nova tarefa | `task`, `period`, `delay` |
| `Task_Dispatch()` | Executa tarefas prontas | Nenhum |
| `Task_Update()` | Atualiza contadores | Nenhum (ISR) |

### Temporização

- **Resolução**: 1ms (SysTick a 1kHz)
- **Precisão**: ±1ms
- **Jitter**: Mínimo (escalonamento cooperativo)

### Mapeamento de Hardware

#### EFM32GG995F1024

| Periférico | Pinos | Função |
|------------|-------|--------|
| LEDs | PE2, PE3, PE4 | LED1, LED2, LED3 |
| Display 7-seg | PD0-PD6 | Segmentos A-G |
| ADC | PA0 | Canal 0 |

#### Arduino Uno (Wokwi)

| Periférico | Pinos | Função |
|------------|-------|--------|
| LEDs | D2, D3, D4 | LED1, LED2, LED3 |
| Display 7-seg | D5-D11 | Segmentos A-G |
| ADC | A0 | Canal analógico |

## Análise de Desempenho

### Utilização da CPU

| Tarefa | Período | Tempo Exec. | CPU % |
|--------|---------|-------------|-------|
| Display | 1000ms | ~10µs | 0.001% |
| LED1 | 500ms | ~5µs | 0.001% |
| LED2 | 750ms | ~5µs | <0.001% |
| LED3 | 1200ms | ~5µs | <0.001% |
| ADC | 100ms | ~20µs | 0.02% |
| **Total** | - | - | **~0.023%** |

### Análise Temporal

- **Overhead do Kernel**: <1µs por tick
- **Latência Máxima**: 1ms (período do SysTick)
- **Precisão**: Determinística para períodos múltiplos de 1ms

## Implementação dos Drivers

### Driver LED

```c
// Controle direto de registradores GPIO
void LED_Toggle(uint32_t leds) {
    GPIO->P[4].DOUTTGL = leds;  // EFM32
    // ou
    PORTD ^= (1 << pin);        // Arduino
}
```

### Driver Display 7-Segmentos

- **Padrões**: Array de 10 elementos (dígitos 0-9)
- **Tipo**: Cátodo comum
- **Controle**: 7 pinos GPIO individuais

### Driver ADC

- **Resolução**: 12 bits (0-4095)
- **Referência**: VDD (3.3V ou 5V)
- **Tempo de Conversão**: ~10µs

## Debugging e Monitoramento

### Métodos de Debug

1. **LEDs de Status**: Indicação visual do funcionamento
2. **Display**: Contador visual para verificar temporização
3. **ADC**: Leitura contínua para teste de responsividade

### Análise de Problemas Comuns

| Problema | Sintoma | Solução |
|----------|---------|---------|
| Jitter excessivo | LEDs irregulares | Verificar ISR |
| Display incorreto | Números errados | Verificar padrões |
| ADC não funciona | Valor fixo | Verificar pinos |

## Extensões Possíveis

### Funcionalidades Avançadas

1. **Prioridades de Tarefas**: Implementar escalonamento por prioridade
2. **Comunicação Serial**: Debug via UART
3. **Watchdog**: Monitoramento de sistema
4. **Sleep Modes**: Economia de energia

### Exemplo de Extensão

```c
// Tarefa com prioridade alta
Task_Add_Priority(Task_Critical, 10, 0, HIGH_PRIORITY);

// Sistema de mensagens entre tarefas
Message_Send(TASK_ADC, TASK_DISPLAY, adc_data);
```

## Compatibilidade

### Plataformas Suportadas

- ✅ EFM32GG (Cortex-M3)
- ✅ Arduino Uno (ATmega328P)
- ✅ Wokwi Simulator
- 🔄 STM32F4 (adaptação necessária)
- 🔄 ESP32 (adaptação necessária)

### Requisitos Mínimos

- **RAM**: 512 bytes
- **Flash**: 4KB
- **Timer**: 1 timer de 16/32 bits
- **GPIO**: 10 pinos mínimo
