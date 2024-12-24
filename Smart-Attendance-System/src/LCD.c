#include "LCD.h"
void LCD_init(void) {
    LCD_DDR = 0xFF;
    DDRD |= (1 << RS) | (1 << EN);
    _delay_ms(20);

    LCD_command(0x38);
    LCD_command(0x0C);
    LCD_command(0x06);
    LCD_command(0x01);
    _delay_ms(10);
}

void LCD_command(unsigned char cmd) {
    LCD_PORT = cmd;
    PORTD &= ~(1 << RS);
    PORTD |= (1 << EN);
    _delay_us(300);
    PORTD &= ~(1 << EN);
    _delay_ms(18);
}

void LCD_data(unsigned char data) {
    LCD_PORT = data;
    PORTD |= (1 << RS);
    PORTD |= (1 << EN);
    _delay_us(300);
    PORTD &= ~(1 << EN);
    _delay_ms(18);
}

void LCD_string(const char *str) {
    while (*str) {
        LCD_data(*str++);
    }
}

void LCD_clear(void) {
    LCD_command(0x01);
    _delay_ms(18);
}