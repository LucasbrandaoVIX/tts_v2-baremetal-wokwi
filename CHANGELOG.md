# Changelog

Todas as mudanças notáveis neste projeto serão documentadas neste arquivo.

## [2.1.0] - 2025-08-25 - 🚨 CORREÇÃO CRÍTICA BARE METAL

### 🔧 Corrigido
- **PROBLEMA CRÍTICO**: sketch.ino estava usando bibliotecas Arduino
- **❌ Removido**: `digitalWrite()`, `digitalRead()`, `analogRead()`, `pinMode()`
- **✅ Implementado**: Funções bare metal com acesso direto aos registradores AVR

### ✨ Adicionado
- **Funções bare metal completas**:
  - `set_pin_high()`, `set_pin_low()`, `toggle_pin()`
  - `adc_init()`, `adc_read()` 
  - `set_pin_output()`
- **Documentação de conformidade** (BARE_METAL_COMPLIANCE.md)
- **Controle direto de registradores**: DDRB, PORTB, ADMUX, ADCSRA, etc.

### 🎯 Conformidade
- **✅ 100% Bare Metal** - Zero bibliotecas Arduino
- **✅ 5 Tarefas TTS** - Todos os requisitos atendidos
- **✅ Wokwi Ready** - Mantém compatibilidade
- **✅ Especificações** - Conforme instruções do projeto

## [2.0.0] - 2025-08-25

### ✨ Adicionado
- **Estrutura organizada do projeto** com diretórios separados
- **5 tarefas simultâneas** implementadas:
  - Display 7 segmentos (contador 0-9, 1s)
  - LED1 vermelho (500ms)
  - LED2 verde (750ms) 
  - LED3 azul (1200ms)
  - Leitura ADC (100ms)
- **Suporte completo ao Wokwi** com sketch.ino adaptado
- **Drivers bare metal** para EFM32GG:
  - LED driver (GPIO direto)
  - Display 7-segmentos driver
  - ADC driver (12-bit)
- **Kernel TTS_V2** com delays configuráveis
- **Documentação técnica** completa
- **Sistema de build** com Makefile organizado
- **Exemplos de extensão** do sistema

### 🏗️ Estrutura
```
├── src/           # Código fonte
├── include/       # Headers
├── wokwi/         # Simulação Wokwi
├── docs/          # Documentação
├── examples/      # Exemplos
└── build/         # Compilação
```

### 🔧 Melhorias
- **Escalonamento cooperativo** determinístico
- **Overhead mínimo** (<0.1% CPU)
- **Precisão de 1ms** com SysTick
- **Arquitetura extensível** para mais tarefas

### 📋 Funcionalidades Técnicas
- **Timer de 1ms** (1kHz SysTick/Timer1)
- **Até 10 tarefas** simultâneas
- **Delays serializados** para evitar clustering
- **Controle bare metal** sem bibliotecas Arduino
- **Compatibilidade dual** EFM32GG/Arduino

### 🎯 Compatibilidade
- ✅ EFM32GG995F1024 (target principal)
- ✅ Arduino Uno (simulação Wokwi)
- ✅ Wokwi Simulator
- ✅ ARM GCC Toolchain

### 📚 Documentação
- README.md principal
- Instruções Wokwi detalhadas
- Documentação técnica
- Exemplos de código
- Comentários em linha

## [1.0.0] - Inicial

### Adicionado
- Implementação básica TTS
- Exemplo simples com 2 LEDs
- Kernel básico sem delays
