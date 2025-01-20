#include "temperature_monitoring.h"
#include <stdio.h>

void ADC_init(void) {
    DDRA &= ~(1 << PA7); 
    ADCSRA = 0x87;  //1000 0111 -> set ADC Enable , 128 (111) for Prescaler --> 125KHz < 200 KHz
    ADMUX = (1 << REFS0); // Vref = AVCC
    ADMUX &= ~(1 << ADLAR); //right adjust the result
}
//**********************************************************************************************************
uint16_t ADC_read(uint8_t channel) {
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F); 
    ADCSRA |= (1 << ADSC); 
    while (!(ADCSRA & (1 << ADIF))); 
    ADCSRA |= (1 << ADIF); 
    uint16_t result = ADCL;
    result |= (ADCH << 8);
    return result;
}
//**********************************************************************************************************
float get_temperature(uint8_t channel) {
    uint16_t adc_value = ADC_read(channel);  //Vout
    // Vin = Vout * step size
    // step size = V_REF / ADC_RESOLUTION
    // Vin = Vout * (V_REF / ADC_RESOLUTION)
    // 10 mv = 1 C
    float temperature = (adc_value * V_REF * 100.0) / ADC_RESOLUTION; 
    return temperature;
}

//**********************************************************************************************************
void display_temperature_continuously(uint8_t channel) {
    float previous_temperature = -1000.0; 
    while (1) {
        float temperature = get_temperature(channel);
        if ((int)(temperature * 100) != (int)(previous_temperature * 100)) {
            previous_temperature = temperature;
            int temp_int = (int)temperature;         
            int temp_frac = (int)((temperature - temp_int) * 100); 
            char buffer[16];
            snprintf(buffer, sizeof(buffer), "Temp: %d.%02d C", temp_int, temp_frac);
            LCD_clear();
            LCD_string(buffer);
            LCD_command(0xC0); 
            LCD_string("Press * to exit");
            _delay_ms(300);
        }
        if (key_pressed == '*') {
            break;
        }
       
    }
}
