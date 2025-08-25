/**
 * @file display.c
 * @brief 7-segment display control for EFM32GG
 */

#include "../include/display.h"
#include "em_device.h"

// 7-segment patterns for digits 0-9 (common cathode)
// Segments: GFEDCBA
static const uint8_t digit_patterns[10] = {
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

/**
 * @brief Initialize 7-segment display
 */
void Display_Init(void) {
    // Enable GPIO clock
    CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_GPIO;
    
    // Configure PD0-PD6 as output for 7-segment display
    GPIO->P[3].MODEL = (GPIO->P[3].MODEL & ~(_GPIO_P_MODEL_MODE0_MASK | 
                                            _GPIO_P_MODEL_MODE1_MASK |
                                            _GPIO_P_MODEL_MODE2_MASK |
                                            _GPIO_P_MODEL_MODE3_MASK |
                                            _GPIO_P_MODEL_MODE4_MASK |
                                            _GPIO_P_MODEL_MODE5_MASK |
                                            _GPIO_P_MODEL_MODE6_MASK)) |
                       (GPIO_P_MODEL_MODE0_PUSHPULL |
                        GPIO_P_MODEL_MODE1_PUSHPULL |
                        GPIO_P_MODEL_MODE2_PUSHPULL |
                        GPIO_P_MODEL_MODE3_PUSHPULL |
                        GPIO_P_MODEL_MODE4_PUSHPULL |
                        GPIO_P_MODEL_MODE5_PUSHPULL |
                        GPIO_P_MODEL_MODE6_PUSHPULL);
    
    // Start with display clear
    Display_Clear();
}

/**
 * @brief Show a digit on the 7-segment display
 * @param digit Digit to show (0-9)
 */
void Display_ShowDigit(uint8_t digit) {
    if (digit > 9) {
        digit = 0; // Default to 0 for invalid digits
    }
    
    // Clear all segments first
    GPIO->P[3].DOUT &= ~0x7F;
    
    // Set the pattern for the digit
    GPIO->P[3].DOUT |= (digit_patterns[digit] & 0x7F);
}

/**
 * @brief Clear the display
 */
void Display_Clear(void) {
    GPIO->P[3].DOUT &= ~0x7F; // Clear segments A-G
}
