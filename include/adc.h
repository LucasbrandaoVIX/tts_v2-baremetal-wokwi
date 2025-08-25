#ifndef ADC_H
#define ADC_H

#include <stdint.h>

// ADC channel definitions
#define ADC_CHANNEL_0 0

// Function prototypes
void ADC_Init(void);
uint16_t ADC_Read(uint8_t channel);
uint16_t ADC_GetLastValue(void);

#endif // ADC_H
