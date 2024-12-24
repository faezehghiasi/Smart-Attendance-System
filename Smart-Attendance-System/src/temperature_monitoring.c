#include "temperature_monitoring.h"
#include "lcd.h"  // Include the LCD library for displaying temperature
#include <stdio.h>
#include "globals.h"

#define V_REF 5.0  // Reference voltage for ADC
#define ADC_RESOLUTION 1024.0
#define TEMPERATURE_FACTOR 10.0  // LM35 outputs 10mV/°C

// Initialize the ADC
void ADC_init(void) {
    // Configure ADC: AVCC as reference, right adjust result
    ADMUX = (1 << REFS0); // Reference voltage AVCC
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1); // Enable ADC and set prescaler to 64
}

// Read ADC value from a specific channel
uint16_t ADC_read(uint8_t channel) {
    // Select ADC channel (0 to 7)
    ADMUX = (ADMUX & 0xF8) | (channel & 0x07);
    
    // Start conversion
    ADCSRA |= (1 << ADSC);
    
    // Wait for conversion to complete
    while (ADCSRA & (1 << ADSC));
    
    // Return ADC result
    return ADC;
}

// Get temperature in °C from the LM35 sensor
float get_temperature(void) {
    uint16_t adc_value = ADC_read(7); // Read from channel 7 (PORTA.7)
    float voltage = (adc_value * V_REF) / ADC_RESOLUTION; // Convert ADC value to voltage
    return voltage * TEMPERATURE_FACTOR; // Convert voltage to temperature
}

// Display temperature on the LCD
void display_temperature_on_LCD(void) {
    float previous_temperature = -1000.0; // Initialize with an invalid temperature

    while (1) {
        // Get the current temperature
        float temperature = get_temperature();

        // Check if the temperature has changed
        if ((int)(temperature * 100) != (int)(previous_temperature * 100)) {
            // Update the previous temperature
            previous_temperature = temperature;

            // Format temperature as integer and fractional parts
            int temp_int = (int)temperature;         // Integer part of temperature
            int temp_frac = (int)((temperature - temp_int) * 100); // Fractional part (two digits)

            // Buffer to hold the formatted string
            char buffer[16];
            snprintf(buffer, sizeof(buffer), "Temp: %d.%02d C", temp_int, temp_frac);

            // Display temperature on LCD
            LCD_clear();
            LCD_string(buffer);

            // Move to the next line and display exit prompt
            LCD_command(0xC0); // Move cursor to the second line
            LCD_string("Press * to exit");
            _delay_ms(300);
        }

        // Check for key press to exit
        if (key_pressed == '*') {
            break;
        }

        // Add a delay for smooth updates
       
    }
}

