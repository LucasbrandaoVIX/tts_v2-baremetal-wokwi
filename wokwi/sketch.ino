/** ***************************************************************************
 * @file    sketch.ino
 * @brief   TTS_V2 Implementation for Arduino - Bare Metal Style
 * @version 2.0
 * @note    Tasks:
 *          1. Display 7-segment: 0-9 counter every 1 second
 *          2. LED1: Toggle every 500ms
 *          3. LED2: Toggle every 750ms
 *          4. LED3: Toggle every 1200ms
 *          5. ADC: Read every 100ms
******************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>

// Pin definitions for Arduino Uno
#define LED1_PIN 2
#define LED2_PIN 3
#define LED3_PIN 4

// Display 7-segment pins
#define SEG_A 5
#define SEG_B 6
#define SEG_C 7
#define SEG_D 8
#define SEG_E 9
#define SEG_F 10
#define SEG_G 11

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
 * @brief  Timer1 interrupt handler (1ms)
 */
ISR(TIMER1_COMPA_vect) {
    task_tickcounter++;
}

/** ***************************************************************************
 * @brief  Task functions
 */
void Task_DisplayCounter(void) {
    unsigned char pattern = digit_patterns[display_counter];
    
    // Update 7-segment display
    digitalWrite(SEG_A, (pattern & 0x01) ? HIGH : LOW);
    digitalWrite(SEG_B, (pattern & 0x02) ? HIGH : LOW);
    digitalWrite(SEG_C, (pattern & 0x04) ? HIGH : LOW);
    digitalWrite(SEG_D, (pattern & 0x08) ? HIGH : LOW);
    digitalWrite(SEG_E, (pattern & 0x10) ? HIGH : LOW);
    digitalWrite(SEG_F, (pattern & 0x20) ? HIGH : LOW);
    digitalWrite(SEG_G, (pattern & 0x40) ? HIGH : LOW);
    
    display_counter++;
    if (display_counter > 9) {
        display_counter = 0;
    }
}

void Task_LED1_Blinker(void) {
    digitalWrite(LED1_PIN, !digitalRead(LED1_PIN));
}

void Task_LED2_Blinker(void) {
    digitalWrite(LED2_PIN, !digitalRead(LED2_PIN));
}

void Task_LED3_Blinker(void) {
    digitalWrite(LED3_PIN, !digitalRead(LED3_PIN));
}

void Task_ADC_Read(void) {
    adc_value = analogRead(ADC_CHANNEL);
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
 * @brief  Setup function
 */
void setup() {
    // Initialize pins
    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);
    pinMode(LED3_PIN, OUTPUT);
    
    pinMode(SEG_A, OUTPUT);
    pinMode(SEG_B, OUTPUT);
    pinMode(SEG_C, OUTPUT);
    pinMode(SEG_D, OUTPUT);
    pinMode(SEG_E, OUTPUT);
    pinMode(SEG_F, OUTPUT);
    pinMode(SEG_G, OUTPUT);
    
    // Initialize display to show 0
    digitalWrite(SEG_A, HIGH);
    digitalWrite(SEG_B, HIGH);
    digitalWrite(SEG_C, HIGH);
    digitalWrite(SEG_D, HIGH);
    digitalWrite(SEG_E, HIGH);
    digitalWrite(SEG_F, HIGH);
    digitalWrite(SEG_G, LOW);
    
    // Setup Timer1 for 1ms interrupts
    cli();
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    OCR1A = 249; // 16MHz / (64 * 250) = 1000Hz (1ms)
    TCCR1B |= (1 << WGM12); // CTC mode
    TCCR1B |= (1 << CS11) | (1 << CS10); // 64 prescaler
    TIMSK1 |= (1 << OCIE1A); // Enable timer compare interrupt
    sei();
    
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
