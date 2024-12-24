#ifndef TEMPERATURE_MONITORING_H
#define TEMPERATURE_MONITORING_H

#include <avr/io.h>
#include <util/delay.h>

// Function prototypes
void ADC_init(void);
uint16_t ADC_read(uint8_t channel);
float get_temperature(void);
void display_temperature_on_LCD(void);

#endif // TEMPERATURE_MONITORING_H
