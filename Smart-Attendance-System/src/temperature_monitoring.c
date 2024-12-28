#include "temperature_monitoring.h"
#include <stdio.h>



void ADC_init(void) {
    DDRA &= ~(1 << PA7); // تنظیم PA7 به‌عنوان ورودی
    ADCSRA = 0x87; // فعال‌سازی ADC و تنظیم prescaler به 128
    ADMUX = (1 << REFS1) | (1 << REFS0);  // انتخاب ولتاژ مرجع 2.56V و راست‌تراز کردن داده‌ها
}

uint16_t ADC_read(uint8_t channel) {
    // انتخاب کانال (پاک‌کردن بیت‌های قدیمی و تنظیم کانال جدید)
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);

    // شروع تبدیل
    ADCSRA |= (1 << ADSC);

    // انتظار برای پایان تبدیل
    while (!(ADCSRA & (1 << ADIF)));

    // پاک کردن پرچم ADIF
    ADCSRA |= (1 << ADIF);

    // خواندن مقدار ADC
    uint16_t result = ADCL;
    result |= (ADCH << 8);

    return result;
}

float get_temperature(uint8_t channel) {
    uint16_t adc_value = ADC_read(channel); // خواندن مقدار ADC
    float temperature = (adc_value * V_REF * 100.0) / ADC_RESOLUTION; // تبدیل به دما (در °C)
    return temperature;
}

void display_temperature_continuously(uint8_t channel) {
    float previous_temperature = -1000.0; // Initialize with an invalid temperature

    while (1) {
        // Get the current temperature
        float temperature = get_temperature(channel);

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