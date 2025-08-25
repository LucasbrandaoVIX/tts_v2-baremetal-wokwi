/** ***************************************************************************
 * @file    main.c
 * @brief   TTS_V2 Implementation with 5 tasks - Bare Metal
 * @version 2.0
 * @note    Tasks:
 *          1. Display 7-segment: 0-9 counter every 1 second
 *          2. LED1: Toggle every 500ms
 *          3. LED2: Toggle every 750ms
 *          4. LED3: Toggle every 1200ms
 *          5. ADC: Read every 100ms
******************************************************************************/

#include <stdint.h>

#include "tt_tasks.h"

/*
 * Including this file, it is possible to define which processor using command line
 * E.g. -DEFM32GG995F1024
 * The alternative is to include the processor specific file directly
 * #include "efm32gg995f1024.h"
 */
#include "em_device.h"
#include "led.h"
#include "display.h"
#include "adc.h"

#define DIVIDER 1000

// Global variables for tasks
static uint8_t display_counter = 0;
static uint16_t adc_value = 0;

/** ***************************************************************************
 * @brief  Get current ADC value
 * @note   Utility function to access ADC reading from other tasks
 * @return Current ADC value (0-4095)
 */
uint16_t Get_ADC_Value(void) {
    return adc_value;
}

/** ***************************************************************************
 * @brief  SysTick interrupt handler
 *
 * @note   Just calls Task_Update
 * @note   Called every 1 ms
 */

void SysTick_Handler(void) {
    Task_Update();
}

/** ***************************************************************************
 * @brief  Task 1: Display 7-segment counter (0-9)
 * @note   Called every 1000 ms (1 second)
 */
void Task_DisplayCounter(void) {
    Display_ShowDigit(display_counter);
    display_counter++;
    if (display_counter > 9) {
        display_counter = 0;
    }
}

/** ***************************************************************************
 * @brief  Task 2: LED1 Blinker
 * @note   Called every 500 ms
 */
void Task_LED1_Blinker(void) {
    LED_Toggle(LED1);
}

/** ***************************************************************************
 * @brief  Task 3: LED2 Blinker
 * @note   Called every 750 ms
 */
void Task_LED2_Blinker(void) {
    LED_Toggle(LED2);
}

/** ***************************************************************************
 * @brief  Task 4: LED3 Blinker
 * @note   Called every 1200 ms
 */
void Task_LED3_Blinker(void) {
    LED_Toggle(LED3);
}

/** ***************************************************************************
 * @brief  Task 5: ADC Reading
 * @note   Called every 100 ms
 */
void Task_ADC_Read(void) {
    adc_value = ADC_Read(ADC_CHANNEL_0);
    // ADC value is stored in global variable for other tasks to use
}


/*****************************************************************************
 * @brief  Main function - TTS_V2 Implementation
 * @note   Using default clock configuration
 *         HFCLK = HFRCO
 *         HFCORECLK = HFCLK
 *         HFPERCLK  = HFCLK
 * 
 * @note   Implements 5 tasks:
 *         1. Display counter (1000ms period)
 *         2. LED1 blinker (500ms period)  
 *         3. LED2 blinker (750ms period)
 *         4. LED3 blinker (1200ms period)
 *         5. ADC reading (100ms period)
 */
int main(void) {

    /* Initialize peripherals */
    LED_Init(LED1 | LED2 | LED3);
    Display_Init();
    ADC_Init();

    /* Initialize Task Kernel */
    Task_Init();
    
    /* Add tasks to the kernel with different periods (in milliseconds) */
    Task_Add(Task_DisplayCounter, 1000, 0);    // Task 1: Display counter every 1s
    Task_Add(Task_LED1_Blinker, 500, 100);     // Task 2: LED1 every 500ms, delay 100ms
    Task_Add(Task_LED2_Blinker, 750, 200);     // Task 3: LED2 every 750ms, delay 200ms  
    Task_Add(Task_LED3_Blinker, 1200, 300);    // Task 4: LED3 every 1200ms, delay 300ms
    Task_Add(Task_ADC_Read, 100, 50);          // Task 5: ADC every 100ms, delay 50ms

    /* Configure SysTick for 1ms interrupts */
    SysTick_Config(SystemCoreClock/DIVIDER);

    /* Main task dispatch loop */
    while (1) {
        Task_Dispatch();
    }
}
