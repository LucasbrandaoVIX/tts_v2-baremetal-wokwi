#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

// Display 7 segment pin definitions (Port D)
#define SEG_A (1 << 0)  // PD0
#define SEG_B (1 << 1)  // PD1
#define SEG_C (1 << 2)  // PD2
#define SEG_D (1 << 3)  // PD3
#define SEG_E (1 << 4)  // PD4
#define SEG_F (1 << 5)  // PD5
#define SEG_G (1 << 6)  // PD6

// Function prototypes
void Display_Init(void);
void Display_ShowDigit(uint8_t digit);
void Display_Clear(void);

#endif // DISPLAY_H
