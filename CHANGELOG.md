# Changelog

Todas as mudanças notáveis neste projeto serão documentadas neste arquivo.

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
