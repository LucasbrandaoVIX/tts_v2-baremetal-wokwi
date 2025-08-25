/**
 * @file extended_example.c
 * @brief Extended example showing additional TTS_V2 features
 * @note This file shows how to extend the basic implementation
 */

#include <stdint.h>
#include "tt_tasks.h"
#include "led.h"
#include "display.h"
#include "adc.h"
#include "config.h"

// Extended global variables
static uint32_t system_uptime_seconds = 0;
static uint8_t led_pattern_state = 0;

/**
 * @brief Extended Task: System uptime counter
 * @note Counts system uptime in seconds
 */
void Task_Uptime_Counter(void) {
    system_uptime_seconds++;
    // Could display uptime on serial port or use for logging
}

/**
 * @brief Extended Task: LED pattern controller
 * @note Creates different LED patterns based on ADC value
 */
void Task_LED_Pattern(void) {
    uint16_t adc_val = ADC_GetLastValue();
    
    // Different patterns based on ADC range
    if (adc_val < 1365) {        // 0-33% range
        // Pattern 1: Only LED1
        LED_On(LED1);
        LED_Off(LED2 | LED3);
    } else if (adc_val < 2730) { // 33-66% range  
        // Pattern 2: LED1 + LED2
        LED_On(LED1 | LED2);
        LED_Off(LED3);
    } else {                     // 66-100% range
        // Pattern 3: All LEDs
        LED_On(LED1 | LED2 | LED3);
    }
}

/**
 * @brief Extended Task: ADC threshold monitor
 * @note Monitors ADC value and triggers actions on thresholds
 */
void Task_ADC_Monitor(void) {
    static uint16_t last_adc = 0;
    uint16_t current_adc = ADC_GetLastValue();
    
    // Check for significant changes (threshold = 100)
    if (abs(current_adc - last_adc) > 100) {
        // Could trigger an event, change display mode, etc.
        led_pattern_state = !led_pattern_state;
        last_adc = current_adc;
    }
}

/**
 * @brief Get system uptime
 * @return System uptime in seconds
 */
uint32_t Get_System_Uptime(void) {
    return system_uptime_seconds;
}

/**
 * @brief Example of how to add more tasks to the system
 * @note This function shows how to extend the main() function
 */
void Add_Extended_Tasks(void) {
    // Add extended tasks with different periods
    Task_Add(Task_Uptime_Counter, 1000, 0);    // Every 1 second
    Task_Add(Task_LED_Pattern, 200, 150);      // Every 200ms
    Task_Add(Task_ADC_Monitor, 50, 25);        // Every 50ms
}

/* 
 * Usage example in main():
 * 
 * int main(void) {
 *     // ... existing initialization code ...
 *     
 *     // Add basic tasks
 *     Task_Add(Task_DisplayCounter, 1000, 0);
 *     Task_Add(Task_LED1_Blinker, 500, 100);
 *     // ... other basic tasks ...
 *     
 *     // Add extended functionality
 *     Add_Extended_Tasks();
 *     
 *     // ... rest of main function ...
 * }
 */
