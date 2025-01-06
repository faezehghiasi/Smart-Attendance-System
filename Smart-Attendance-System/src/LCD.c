#include "LCD.h"

void LCD_init(void) {
    LCD_DDR = 0xFF; // Set all data pins (D0-D7) of the LCD to output
    DDRD |= (1 << RS) | (1 << EN); // Set RS and EN pins of the LCD to output
    _delay_ms(20);
    LCD_command(0x38); // Function set: 8-bit mode, 2-line display, 5x8 dots font
    LCD_command(0x0C); // Display on, cursor off, blinking off
    LCD_command(0x06); // Entry mode set: Increment cursor, no shift
    LCD_command(0x01); // Clear display screen
    _delay_ms(10); 
}
//**********************************************************************************************************
void LCD_command(unsigned char cmd) {
    LCD_PORT = cmd; // Send the command to the LCD data port
    PORTD &= ~(1 << RS); // RS = 0 for command mode
    PORTD |= (1 << EN); // Enable the LCD to latch the command
    _delay_us(300); 
    PORTD &= ~(1 << EN); // Disable the LCD after latching the command
    _delay_ms(18); 
}
//**********************************************************************************************************
void LCD_data(unsigned char data) {
    LCD_PORT = data; // Send the data to the LCD data port
    PORTD |= (1 << RS); // RS = 1 for data mode
    PORTD |= (1 << EN); // Enable the LCD to latch the data
    _delay_us(300); 
    PORTD &= ~(1 << EN); // Disable the LCD after latching the data
    _delay_ms(18); 
}
//**********************************************************************************************************
void LCD_string(const char *str) {
    while (*str) { 
        LCD_data(*str++); 
    }
}
//**********************************************************************************************************
void LCD_clear(void) {
    LCD_command(0x01);
    _delay_ms(18); 
}
