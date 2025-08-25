#ifndef LED_H
#define LED_H

#include <stdint.h>

// LED definitions for EFM32GG
#define LED1 (1 << 2)  // PE2
#define LED2 (1 << 3)  // PE3
#define LED3 (1 << 4)  // PE4

// Function prototypes
void LED_Init(uint32_t leds);
void LED_Toggle(uint32_t leds);
void LED_On(uint32_t leds);
void LED_Off(uint32_t leds);

#endif // LED_H
