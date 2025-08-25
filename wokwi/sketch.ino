/** ***************************************************************************
 * @file    sketch.ino
 * @brief   Implementação TTS_V2 - 100% BARE METAL (SEM BIBLIOTECAS ARDUINO)
 * @version 2.1
 * @authors
 *   - Lucas Dantas Brandão
 *   - Gusmar Gianordoli Santana
 * @note    Tarefas:
 *          1. Display 7 segmentos: contador 0-9 a cada 1 segundo
 *          2. LED1: pisca a cada 500ms
 *          3. LED2: pisca a cada 750ms
 *          4. LED3: pisca a cada 1200ms
 *          5. ADC: lê a cada 100ms
 * @warning SEM bibliotecas Arduino - Código puro AVR!
 ******************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>

// Definição dos pinos para Arduino Uno (AVR ATmega328P)
#define LED1_PIN 2  // PD2
#define LED2_PIN 3  // PD3
#define LED3_PIN 4  // PD4

// Pinos do display 7 segmentos (Porta B e D)
#define SEG_A 5     // PD5
#define SEG_B 6     // PD6
#define SEG_C 7     // PD7
#define SEG_D 8     // PB0
#define SEG_E 9     // PB1
#define SEG_F 10    // PB2
#define SEG_G 11    // PB3

#define ADC_CHANNEL 0
#define TASK_MAXCNT 10

// Estrutura das tarefas
typedef struct {
    void (*task)(void);
    unsigned long period;
    unsigned long delay;
} TaskInfo;

// Variáveis globais
static TaskInfo taskinfo[TASK_MAXCNT];
static volatile unsigned long task_tickcounter = 0;
static unsigned char display_counter = 0;
static unsigned int adc_value = 0;

// Padrões dos dígitos 0-9 para display 7 segmentos
const unsigned char digit_patterns[10] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111  // 9
};

/** ***************************************************************************
 * @brief  Funções básicas para controlar hardware
 * @note   SEM bibliotecas Arduino - Manipulação direta dos registradores
 */

// Configura pino como saída
void set_pin_output(unsigned char port, unsigned char pin) {
    if (port == 'B') {
        DDRB |= (1 << pin);
    } else if (port == 'D') {
        DDRD |= (1 << pin);
    }
}

// Liga pino (5V)
void set_pin_high(unsigned char port, unsigned char pin) {
    if (port == 'B') {
        PORTB |= (1 << pin);
    } else if (port == 'D') {
        PORTD |= (1 << pin);
    }
}

// Desliga pino (0V)
void set_pin_low(unsigned char port, unsigned char pin) {
    if (port == 'B') {
        PORTB &= ~(1 << pin);
    } else if (port == 'D') {
        PORTD &= ~(1 << pin);
    }
}

// Inverte estado do pino (liga se tá desligado, desliga se tá ligado)
void toggle_pin(unsigned char port, unsigned char pin) {
    if (port == 'B') {
        PORTB ^= (1 << pin);
    } else if (port == 'D') {
        PORTD ^= (1 << pin);
    }
}

// Lê estado do pino
unsigned char read_pin(unsigned char port, unsigned char pin) {
    if (port == 'B') {
        return (PINB & (1 << pin)) ? 1 : 0;
    } else if (port == 'D') {
        return (PIND & (1 << pin)) ? 1 : 0;
    }
    return 0;
}

// Inicializa conversor ADC
void adc_init(void) {
    // Configura referência ADC para AVCC
    ADMUX = (1 << REFS0);
    
    // Liga ADC e configura prescaler para 128 (16MHz/128 = 125kHz)
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

// Lê valor do ADC
unsigned int adc_read(unsigned char channel) {
    // Seleciona canal do ADC
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    
    // Inicia conversão
    ADCSRA |= (1 << ADSC);
    
    // Espera conversão terminar
    while (ADCSRA & (1 << ADSC));
    
    // Retorna resultado do ADC
    return ADC;
}

/** ***************************************************************************
 * @brief  Interrupção do Timer1 (1ms)
 */
ISR(TIMER1_COMPA_vect) {
    task_tickcounter++;
}

/** ***************************************************************************
 * @brief  Funções das tarefas
 */
void Task_DisplayCounter(void) {
    unsigned char pattern = digit_patterns[display_counter];
    
    // Atualiza display 7 segmentos
    // Segmentos A,B,C (PD5,PD6,PD7)
    if (pattern & 0x01) set_pin_high('D', 5); else set_pin_low('D', 5);  // A
    if (pattern & 0x02) set_pin_high('D', 6); else set_pin_low('D', 6);  // B  
    if (pattern & 0x04) set_pin_high('D', 7); else set_pin_low('D', 7);  // C
    
    // Segmentos D,E,F,G (PB0,PB1,PB2,PB3)
    if (pattern & 0x08) set_pin_high('B', 0); else set_pin_low('B', 0);  // D
    if (pattern & 0x10) set_pin_high('B', 1); else set_pin_low('B', 1);  // E
    if (pattern & 0x20) set_pin_high('B', 2); else set_pin_low('B', 2);  // F
    if (pattern & 0x40) set_pin_high('B', 3); else set_pin_low('B', 3);  // G
    
    display_counter++;
    if (display_counter > 9) {
        display_counter = 0;
    }
}

void Task_LED1_Blinker(void) {
    toggle_pin('D', 2);  // Inverte LED1 (PD2)
}

void Task_LED2_Blinker(void) {
    toggle_pin('D', 3);  // Inverte LED2 (PD3)
}

void Task_LED3_Blinker(void) {
    toggle_pin('D', 4);  // Inverte LED3 (PD4)
}

void Task_ADC_Read(void) {
    adc_value = adc_read(ADC_CHANNEL);  // Lê ADC
}

/** ***************************************************************************
 * @brief  Funções do kernel TTS
 */
void Task_Init(void) {
    for (int i = 0; i < TASK_MAXCNT; i++) {
        taskinfo[i].task = 0;
        taskinfo[i].period = 0;
        taskinfo[i].delay = 0;
    }
}

unsigned char Task_Add(void (*task)(void), unsigned long period, unsigned long delay) {
    int taskno = 0;
    
    while ((taskno < TASK_MAXCNT) && taskinfo[taskno].task) taskno++;
    if (taskno == TASK_MAXCNT) return 1;
    
    taskinfo[taskno].task = task;
    taskinfo[taskno].period = period;
    taskinfo[taskno].delay = delay;
    
    return taskno;
}

void Task_Dispatch(void) {
    unsigned char dispatch = 0;
    TaskInfo *p;
    
    cli(); // Desabilita interrupções
    if (task_tickcounter > 0) {
        task_tickcounter--;
        dispatch = 1;
    }
    sei(); // Habilita interrupções
    
    while (dispatch) {
        for (int i = 0; i < TASK_MAXCNT; i++) {
            p = &taskinfo[i];
            if (p->task) {
                if (p->delay == 0) {
                    p->task();
                    if (p->period == 0) {
                        // Tarefa de uma vez só
                        p->task = 0;
                        p->period = 0;
                    } else {
                        p->delay = p->period;
                    }
                } else {
                    p->delay--;
                }
            }
        }
        
        cli();
        if (task_tickcounter > 0) {
            task_tickcounter--;
            dispatch = 1;
        } else {
            dispatch = 0;
        }
        sei();
    }
}

/** ***************************************************************************
 * @brief  Configuração inicial - SEM BIBLIOTECAS ARDUINO
 * @note   Configuração direta dos registradores AVR
 */
void setup() {
    // Configura pinos GPIO como saídas
    // LEDs: PD2, PD3, PD4
    set_pin_output('D', 2);  // LED1
    set_pin_output('D', 3);  // LED2  
    set_pin_output('D', 4);  // LED3
    
    // Pinos do Display 7 segmentos: PD5,PD6,PD7 e PB0,PB1,PB2,PB3
    set_pin_output('D', 5);  // Segmento A
    set_pin_output('D', 6);  // Segmento B
    set_pin_output('D', 7);  // Segmento C
    set_pin_output('B', 0);  // Segmento D
    set_pin_output('B', 1);  // Segmento E
    set_pin_output('B', 2);  // Segmento F
    set_pin_output('B', 3);  // Segmento G
    
    // Inicializa todos os pinos desligados
    set_pin_low('D', 2); set_pin_low('D', 3); set_pin_low('D', 4);
    set_pin_low('D', 5); set_pin_low('D', 6); set_pin_low('D', 7);
    set_pin_low('B', 0); set_pin_low('B', 1); set_pin_low('B', 2); set_pin_low('B', 3);
    
    // Inicializa display mostrando "0"
    unsigned char pattern = digit_patterns[0];
    if (pattern & 0x01) set_pin_high('D', 5); // A
    if (pattern & 0x02) set_pin_high('D', 6); // B
    if (pattern & 0x04) set_pin_high('D', 7); // C
    if (pattern & 0x08) set_pin_high('B', 0); // D
    if (pattern & 0x10) set_pin_high('B', 1); // E
    if (pattern & 0x20) set_pin_high('B', 2); // F
    
    // Configura Timer1 para interrupção de 1ms
    cli();  // Desabilita interrupções globais
    
    // Configura Timer1 em modo CTC
    TCCR1A = 0;                    // Limpa registrador de controle A do Timer1
    TCCR1B = 0;                    // Limpa registrador de controle B do Timer1
    TCNT1 = 0;                     // Zera contador
    
    // Configura valor de comparação para 1ms @ 16MHz com prescaler 64
    // (16,000,000 / 64) / 1000 - 1 = 249
    OCR1A = 249;
    
    // Configura Timer1: modo CTC + prescaler 64
    TCCR1B |= (1 << WGM12);        // Modo CTC
    TCCR1B |= (1 << CS11) | (1 << CS10);  // Prescaler 64
    
    // Habilita interrupção de comparação do Timer1
    TIMSK1 |= (1 << OCIE1A);
    
    sei();  // Habilita interrupções globais
    
    // Inicializa ADC
    adc_init();
    
    // Inicializa kernel TTS
    Task_Init();
    
    // Adiciona tarefas com períodos diferentes (em milissegundos)
    Task_Add(Task_DisplayCounter, 1000, 0);    // Display a cada 1s
    Task_Add(Task_LED1_Blinker, 500, 100);     // LED1 a cada 500ms, atraso 100ms
    Task_Add(Task_LED2_Blinker, 750, 200);     // LED2 a cada 750ms, atraso 200ms
    Task_Add(Task_LED3_Blinker, 1200, 300);    // LED3 a cada 1200ms, atraso 300ms
    Task_Add(Task_ADC_Read, 100, 50);          // ADC a cada 100ms, atraso 50ms
}

/** ***************************************************************************
 * @brief  Loop principal
 */
void loop() {
    Task_Dispatch();
}
