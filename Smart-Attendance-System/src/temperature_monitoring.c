#include "temperature_monitoring.h"

// Initialize ADC
void ADC_init(void) {
    ADMUX = (1 << REFS0); // Set AVCC as reference (5V)
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1); // Enable ADC, prescaler = 64
}

uint16_t ADC_read(uint8_t channel) {
    ADMUX = (ADMUX & 0xF8) | (channel & 0x07); // Select ADC channel
    ADCSRA |= (1 << ADSC);                    // Start conversion
    while (ADCSRA & (1 << ADSC));             // Wait for conversion to complete
    return ADC;                               // Return ADC value
}

// Convert ADC value to temperature in Celsius
float get_temperature(void) {
    uint16_t adc_value = ADC_read(7); // Assuming channel 7 (PA7) is connected to the LM35 sensor
    float voltage = (adc_value * 5.0) / 1024.0; // Convert ADC value to voltage (for 5V reference)
    return voltage / 0.01; // Convert voltage to temperature (LM35: 10mV/°C)
}

// Display temperature on LCD
void display_temperature(void) {
    char temp_str[16];

    while (1) {
        float temperature = get_temperature(); // Get temperature value

        // Format temperature to string with 1 decimal point
        dtostrf(temperature, 5, 1, temp_str);

        // Display temperature on LCD
        LCD_clear();
        LCD_string("Temperature: ");
        LCD_string(temp_str);
        LCD_string(" C");

        _delay_ms(1000); // Update every second

        // Exit if '*' is pressed
        if (key_pressed == '*') {
            LCD_clear();
            break;
        }
    }
}
