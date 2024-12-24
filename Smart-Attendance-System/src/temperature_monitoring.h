#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include "lcd.h" // LCD Library for displaying values
#include "globals.h"

// Function Prototypes
void ADC_init(void); // Initialize ADC
uint16_t ADC_read(uint8_t channel); // Read ADC value from a specific channel
float get_temperature(void); // Convert ADC value to temperature
void display_temperature(void); // Display temperature on LCD

#endif
