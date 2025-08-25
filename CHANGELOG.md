# Changelog

Todas as mudanças relevantes neste projeto são registradas neste arquivo.

## [2.1.0] - 2025-08-25

Correção crítica de conformidade bare metal

### Correções
- Removida dependência de APIs do Arduino (`digitalWrite`, `digitalRead`, `analogRead`, `pinMode`).
- Implementadas funções de acesso direto ao hardware para GPIO e ADC.

### Adições
- Implementadas funções: `set_pin_high`, `set_pin_low`, `toggle_pin`, `set_pin_output`, `adc_init`, `adc_read`.
- Documentação de conformidade adicionada: `docs/BARE_METAL_COMPLIANCE.md`.

### Observações
- O código está apto a execução no simulador Wokwi (alvo Arduino). Para execução em EFM32GG, o hardware abstraction layer foi pensado para fácil adaptação.

## [2.0.0] - 2025-08-25

### Adicionado
- Estrutura organizada do projeto com diretórios separados
- Implementação de cinco tarefas simultâneas:
  - Display 7 segmentos (contador 0–9, 1 s)
  - LED1 vermelho (500 ms)
  - LED2 verde (750 ms)
  - LED3 azul (1200 ms)
  - Leitura ADC (100 ms)
- Suporte ao Wokwi com `sketch.ino` adaptado
- Drivers bare metal para EFM32GG (LED, display 7 segmentos, ADC)
- Kernel `TTS_V2` com delays configuráveis
- Documentação técnica e exemplos

### Estrutura do projeto

```
├── src/           # Código fonte
├── include/       # Headers
├── wokwi/         # Simulação Wokwi
├── docs/          # Documentação
├── examples/      # Exemplos
└── build/         # Compilação
```

### Melhorias
- Escalonamento cooperativo determinístico
- Overhead reduzido (estimado < 0.1% CPU)
- Precisão de 1 ms com SysTick

### Funcionalidades técnicas
- Timer de 1 ms (1 kHz) para ticks
- Suporta até 10 tarefas simultâneas
- Delays serializados para evitar clustering
- Implementação bare metal para controle de hardware

### Compatibilidade
- EFM32GG995F1024 (alvo principal)
- Arduino Uno (simulação Wokwi)
- Ferramentas: ARM GCC toolchain

### Documentação
- README principal
- Instruções para Wokwi
- Documentação técnica e compliance

## [1.0.0] - Inicial

### Adicionado
- Implementação básica TTS
- Exemplo simples com 2 LEDs
- Kernel básico sem delays
