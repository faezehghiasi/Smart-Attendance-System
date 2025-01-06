#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
#include "globals.h"

#define V_REF 5
#define ADC_RESOLUTION 1024.0
void ADC_init(void);
uint16_t ADC_read(uint8_t channel);
float get_temperature(uint8_t channel);
void display_temperature_continuously(uint8_t channel);

#endif