# Sketch: conformidade bare metal

## Problema identificado

O `sketch.ino` original utilizava chamadas da API do Arduino (`digitalWrite`, `digitalRead`, `analogRead`, `pinMode`), o que contraria a exigência de implementação bare metal.

## Correções implementadas

Foram introduzidas funções para abstração mínima do hardware que operam diretamente sobre registradores do microcontrolador:

```c
void set_pin_output(unsigned char port, unsigned char pin);
void set_pin_high(unsigned char port, unsigned char pin);
void set_pin_low(unsigned char port, unsigned char pin);
void toggle_pin(unsigned char port, unsigned char pin);
unsigned char read_pin(unsigned char port, unsigned char pin);

void adc_init(void);
unsigned int adc_read(unsigned char channel);
```
# Sketch corrigido: conformidade bare metal

## Problema identificado

O `sketch.ino` original utilizava chamadas da API do Arduino (`digitalWrite`, `digitalRead`, `analogRead`, `pinMode`), o que contraria a exigência de implementação bare metal.

## Correções implementadas

Foram introduzidas funções para abstração mínima do hardware que operam diretamente sobre registradores do microcontrolador:

```c
void set_pin_output(unsigned char port, unsigned char pin);
void set_pin_high(unsigned char port, unsigned char pin);
void set_pin_low(unsigned char port, unsigned char pin);
void toggle_pin(unsigned char port, unsigned char pin);
unsigned char read_pin(unsigned char port, unsigned char pin);

void adc_init(void);
unsigned int adc_read(unsigned char channel);
```

Essas funções substituem as chamadas às APIs do Arduino e usam registradores AVR ou EFM32 conforme a plataforma alvo.

## Implementação técnica resumida

1. GPIO

   - Manipulação direta de `DDR`, `PORT` e `PIN` para configurar e controlar pinos.

   - Exemplo AVR:

   ```c
   DDRB |= (1 << pin);    /* configura saída */
   PORTB |= (1 << pin);   /* set high */
   PORTB &= ~(1 << pin);  /* set low */
   PORTB ^= (1 << pin);   /* toggle */
   ```

2. ADC

   - Configuração de referência e prescaler em `ADMUX` e `ADCSRA`.

   - Leitura por polling com `ADSC` e leitura do registrador `ADC`.

3. Timer

   - Timer configurado em modo CTC para gerar interrupt cada 1 ms.

## Conformidade com requisitos

O código revisado atende aos requisitos originais do projeto:

- Kernel `TTS_V2` com cinco tarefas funcionais.
- Implementação bare metal (sem uso das bibliotecas do Arduino).
- Tarefas: display 7-seg (1 s), três LEDs com períodos arbitrários, ADC a cada 100 ms.

## Registos utilizados (exemplo AVR)

- `DDRB`, `DDRD` — direção dos pinos
- `PORTB`, `PORTD` — níveis de saída
- `PINB`, `PIND` — leitura de pinos
- `ADMUX`, `ADCSRA`, `ADC` — ADC
- `TCCR1A`, `TCCR1B`, `OCR1A`, `TIMSK1` — Timer1

## Observações

- O exemplo mantém compatibilidade com a simulação no Wokwi através do alvo Arduino; para uso em EFM32 é necessária a adaptação do hardware abstraction layer para os registradores do EFM32.
