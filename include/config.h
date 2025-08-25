/**
 * @file config.h
 * @brief Configuration parameters for TTS_V2 system
 */

#ifndef CONFIG_H
#define CONFIG_H

// Task configuration
#define TASK_MAXCNT 10              // Maximum number of tasks

// Timing configuration  
#define SYSTICK_FREQUENCY_HZ 1000   // SysTick frequency (1ms tick)
#define DIVIDER (SystemCoreClock/SYSTICK_FREQUENCY_HZ)

// Task periods (in milliseconds)
#define DISPLAY_PERIOD_MS 1000      // Display counter period
#define LED1_PERIOD_MS 500          // LED1 blink period
#define LED2_PERIOD_MS 750          // LED2 blink period  
#define LED3_PERIOD_MS 1200         // LED3 blink period
#define ADC_PERIOD_MS 100           // ADC read period

// Task delays (in milliseconds) 
#define DISPLAY_DELAY_MS 0          // Display initial delay
#define LED1_DELAY_MS 100           // LED1 initial delay
#define LED2_DELAY_MS 200           // LED2 initial delay
#define LED3_DELAY_MS 300           // LED3 initial delay
#define ADC_DELAY_MS 50             // ADC initial delay

// Hardware configuration
#define DISPLAY_DIGITS 10           // Number of digits (0-9)
#define ADC_RESOLUTION_BITS 12      // ADC resolution
#define ADC_MAX_VALUE ((1 << ADC_RESOLUTION_BITS) - 1)

#endif // CONFIG_H
