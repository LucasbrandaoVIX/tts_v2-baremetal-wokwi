/** ***************************************************************************
 * @file    sketch.ino
 * @brief   TTS_V2 Implementation - 100% BARE METAL (NO ARDUINO LIBRARIES)
 * @version 2.1
 * @note    Tasks:
 *          1. Display 7-segment: 0-9 counter every 1 second
 *          2. LED1: Toggle every 500ms
 *          3. LED2: Toggle every 750ms
 *          4. LED3: Toggle every 1200ms
 *          5. ADC: Read every 100ms
 * @warning NO Arduino libraries used - Pure AVR bare metal code!
******************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>

// Pin definitions for Arduino Uno (AVR ATmega328P)
#define LED1_PIN 2  // PD2
#define LED2_PIN 3  // PD3
#define LED3_PIN 4  // PD4

// Display 7-segment pins (Port B and D)
#define SEG_A 5     // PD5
#define SEG_B 6     // PD6
#define SEG_C 7     // PD7
#define SEG_D 8     // PB0
#define SEG_E 9     // PB1
#define SEG_F 10    // PB2
#define SEG_G 11    // PB3

#define ADC_CHANNEL 0
#define TASK_MAXCNT 10

// Task structure
typedef struct {
    void (*task)(void);
    unsigned long period;
    unsigned long delay;
} TaskInfo;

// Global variables
static TaskInfo taskinfo[TASK_MAXCNT];
static volatile unsigned long task_tickcounter = 0;
static unsigned char display_counter = 0;
static unsigned int adc_value = 0;

// 7-segment patterns for digits 0-9
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
 * @brief  BARE METAL Hardware Abstraction Functions
 * @note   NO Arduino libraries - Direct register manipulation
 */

// BARE METAL: Set pin as output
void set_pin_output(unsigned char port, unsigned char pin) {
    if (port == 'B') {
        DDRB |= (1 << pin);
    } else if (port == 'D') {
        DDRD |= (1 << pin);
    }
}

// BARE METAL: Set pin HIGH
void set_pin_high(unsigned char port, unsigned char pin) {
    if (port == 'B') {
        PORTB |= (1 << pin);
    } else if (port == 'D') {
        PORTD |= (1 << pin);
    }
}

// BARE METAL: Set pin LOW
void set_pin_low(unsigned char port, unsigned char pin) {
    if (port == 'B') {
        PORTB &= ~(1 << pin);
    } else if (port == 'D') {
        PORTD &= ~(1 << pin);
    }
}

// BARE METAL: Toggle pin
void toggle_pin(unsigned char port, unsigned char pin) {
    if (port == 'B') {
        PORTB ^= (1 << pin);
    } else if (port == 'D') {
        PORTD ^= (1 << pin);
    }
}

// BARE METAL: Read pin state
unsigned char read_pin(unsigned char port, unsigned char pin) {
    if (port == 'B') {
        return (PINB & (1 << pin)) ? 1 : 0;
    } else if (port == 'D') {
        return (PIND & (1 << pin)) ? 1 : 0;
    }
    return 0;
}

// BARE METAL: ADC initialization
void adc_init(void) {
    // Set ADC reference to AVCC
    ADMUX = (1 << REFS0);
    
    // Enable ADC and set prescaler to 128 (16MHz/128 = 125kHz)
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

// BARE METAL: Read ADC value
unsigned int adc_read(unsigned char channel) {
    // Select ADC channel
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    
    // Start conversion
    ADCSRA |= (1 << ADSC);
    
    // Wait for conversion to complete
    while (ADCSRA & (1 << ADSC));
    
    // Return ADC result
    return ADC;
}

/** ***************************************************************************
 * @brief  Timer1 interrupt handler (1ms) - BARE METAL
 */
ISR(TIMER1_COMPA_vect) {
    task_tickcounter++;
}

/** ***************************************************************************
 * @brief  Task functions - 100% BARE METAL
 */
void Task_DisplayCounter(void) {
    unsigned char pattern = digit_patterns[display_counter];
    
    // Update 7-segment display using BARE METAL register access
    // Segments A,B,C (PD5,PD6,PD7)
    if (pattern & 0x01) set_pin_high('D', 5); else set_pin_low('D', 5);  // A
    if (pattern & 0x02) set_pin_high('D', 6); else set_pin_low('D', 6);  // B  
    if (pattern & 0x04) set_pin_high('D', 7); else set_pin_low('D', 7);  // C
    
    // Segments D,E,F,G (PB0,PB1,PB2,PB3)
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
    toggle_pin('D', 2);  // BARE METAL: Toggle PD2
}

void Task_LED2_Blinker(void) {
    toggle_pin('D', 3);  // BARE METAL: Toggle PD3
}

void Task_LED3_Blinker(void) {
    toggle_pin('D', 4);  // BARE METAL: Toggle PD4
}

void Task_ADC_Read(void) {
    adc_value = adc_read(ADC_CHANNEL);  // BARE METAL: Direct ADC read
}

/** ***************************************************************************
 * @brief  TTS Kernel functions
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
    
    cli(); // Disable interrupts
    if (task_tickcounter > 0) {
        task_tickcounter--;
        dispatch = 1;
    }
    sei(); // Enable interrupts
    
    while (dispatch) {
        for (int i = 0; i < TASK_MAXCNT; i++) {
            p = &taskinfo[i];
            if (p->task) {
                if (p->delay == 0) {
                    p->task();
                    if (p->period == 0) {
                        // One-time task
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
 * @brief  Setup function - 100% BARE METAL INITIALIZATION
 * @note   NO Arduino functions - Direct AVR register configuration
 */
void setup() {
    // BARE METAL: Configure GPIO pins as outputs
    // LEDs: PD2, PD3, PD4
    set_pin_output('D', 2);  // LED1
    set_pin_output('D', 3);  // LED2  
    set_pin_output('D', 4);  // LED3
    
    // 7-Segment Display pins: PD5,PD6,PD7 and PB0,PB1,PB2,PB3
    set_pin_output('D', 5);  // Segment A
    set_pin_output('D', 6);  // Segment B
    set_pin_output('D', 7);  // Segment C
    set_pin_output('B', 0);  // Segment D
    set_pin_output('B', 1);  // Segment E
    set_pin_output('B', 2);  // Segment F
    set_pin_output('B', 3);  // Segment G
    
    // BARE METAL: Initialize all pins to LOW
    set_pin_low('D', 2); set_pin_low('D', 3); set_pin_low('D', 4);
    set_pin_low('D', 5); set_pin_low('D', 6); set_pin_low('D', 7);
    set_pin_low('B', 0); set_pin_low('B', 1); set_pin_low('B', 2); set_pin_low('B', 3);
    
    // BARE METAL: Initialize display to show "0"
    unsigned char pattern = digit_patterns[0];
    if (pattern & 0x01) set_pin_high('D', 5); // A
    if (pattern & 0x02) set_pin_high('D', 6); // B
    if (pattern & 0x04) set_pin_high('D', 7); // C
    if (pattern & 0x08) set_pin_high('B', 0); // D
    if (pattern & 0x10) set_pin_high('B', 1); // E
    if (pattern & 0x20) set_pin_high('B', 2); // F
    
    // BARE METAL: Setup Timer1 for 1ms interrupts (NO Arduino functions)
    cli();  // Disable global interrupts
    
    // Configure Timer1 in CTC mode
    TCCR1A = 0;                    // Clear Timer1 control register A
    TCCR1B = 0;                    // Clear Timer1 control register B
    TCNT1 = 0;                     // Initialize counter value to 0
    
    // Set compare value for 1ms @ 16MHz with 64 prescaler
    // (16,000,000 / 64) / 1000 - 1 = 249
    OCR1A = 249;
    
    // Configure Timer1: CTC mode + 64 prescaler
    TCCR1B |= (1 << WGM12);        // CTC mode
    TCCR1B |= (1 << CS11) | (1 << CS10);  // 64 prescaler
    
    // Enable Timer1 compare interrupt
    TIMSK1 |= (1 << OCIE1A);
    
    sei();  // Enable global interrupts
    
    // BARE METAL: Initialize ADC
    adc_init();
    
    // Initialize TTS kernel
    Task_Init();
    
    // Add tasks with different periods (in milliseconds)
    Task_Add(Task_DisplayCounter, 1000, 0);    // Display every 1s
    Task_Add(Task_LED1_Blinker, 500, 100);     // LED1 every 500ms, delay 100ms
    Task_Add(Task_LED2_Blinker, 750, 200);     // LED2 every 750ms, delay 200ms
    Task_Add(Task_LED3_Blinker, 1200, 300);    // LED3 every 1200ms, delay 300ms
    Task_Add(Task_ADC_Read, 100, 50);          // ADC every 100ms, delay 50ms
}

/** ***************************************************************************
 * @brief  Main loop
 */
void loop() {
    Task_Dispatch();
}
