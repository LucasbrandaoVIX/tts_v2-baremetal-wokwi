/**
 * @file led.c
 * @brief LED control for EFM32GG
 */

#include "../include/led.h"
#include "em_device.h"

/**
 * @brief Initialize LEDs
 * @param leds Bitmask of LEDs to initialize
 */
void LED_Init(uint32_t leds) {
    // Enable GPIO clock
    CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_GPIO;
    
    // Configure PE2, PE3, PE4 as output for LEDs
    if (leds & LED1) {
        GPIO->P[4].MODEL = (GPIO->P[4].MODEL & ~_GPIO_P_MODEL_MODE2_MASK) | GPIO_P_MODEL_MODE2_PUSHPULL;
        GPIO->P[4].DOUT &= ~(1 << 2); // Start with LED off
    }
    if (leds & LED2) {
        GPIO->P[4].MODEL = (GPIO->P[4].MODEL & ~_GPIO_P_MODEL_MODE3_MASK) | GPIO_P_MODEL_MODE3_PUSHPULL;
        GPIO->P[4].DOUT &= ~(1 << 3); // Start with LED off
    }
    if (leds & LED3) {
        GPIO->P[4].MODEL = (GPIO->P[4].MODEL & ~_GPIO_P_MODEL_MODE4_MASK) | GPIO_P_MODEL_MODE4_PUSHPULL;
        GPIO->P[4].DOUT &= ~(1 << 4); // Start with LED off
    }
}

/**
 * @brief Toggle LEDs
 * @param leds Bitmask of LEDs to toggle
 */
void LED_Toggle(uint32_t leds) {
    GPIO->P[4].DOUTTGL = leds;
}

/**
 * @brief Turn on LEDs
 * @param leds Bitmask of LEDs to turn on
 */
void LED_On(uint32_t leds) {
    GPIO->P[4].DOUTSET = leds;
}

/**
 * @brief Turn off LEDs
 * @param leds Bitmask of LEDs to turn off
 */
void LED_Off(uint32_t leds) {
    GPIO->P[4].DOUTCLR = leds;
}
