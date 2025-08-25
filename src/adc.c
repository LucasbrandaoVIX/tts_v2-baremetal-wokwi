/**
 * @file adc.c
 * @brief ADC control for EFM32GG
 */

#include "../include/adc.h"
#include "em_device.h"

static uint16_t last_adc_value = 0;

/**
 * @brief Initialize ADC
 */
void ADC_Init(void) {
    // Enable ADC0 clock
    CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_ADC0;
    
    // Configure ADC0
    ADC0->CTRL = ADC_CTRL_PRESC_DIV2;           // Prescaler /2
    ADC0->SINGLECTRL = ADC_SINGLECTRL_REF_VDD | // VDD reference
                       ADC_SINGLECTRL_INPUTSEL_CH0 | // Channel 0
                       ADC_SINGLECTRL_RES_12BIT;     // 12-bit resolution
    
    // Enable ADC
    ADC0->CMD = ADC_CMD_SINGLESTART;
}

/**
 * @brief Read ADC value from specified channel
 * @param channel ADC channel to read
 * @return 12-bit ADC value
 */
uint16_t ADC_Read(uint8_t channel) {
    // Select channel
    ADC0->SINGLECTRL = (ADC0->SINGLECTRL & ~_ADC_SINGLECTRL_INPUTSEL_MASK) |
                       (channel << _ADC_SINGLECTRL_INPUTSEL_SHIFT);
    
    // Start conversion
    ADC0->CMD = ADC_CMD_SINGLESTART;
    
    // Wait for conversion to complete
    while (!(ADC0->STATUS & ADC_STATUS_SINGLEDV));
    
    // Read and store result
    last_adc_value = ADC0->SINGLEDATA & 0xFFF;
    
    return last_adc_value;
}

/**
 * @brief Get the last ADC reading
 * @return Last ADC value read
 */
uint16_t ADC_GetLastValue(void) {
    return last_adc_value;
}
