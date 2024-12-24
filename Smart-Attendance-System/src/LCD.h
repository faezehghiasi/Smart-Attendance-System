#ifndef LCD_H
#define LCD_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define RS PD0
#define EN PD3
#define LCD_PORT PORTC
#define LCD_DDR DDRC

void LCD_init(void);
void LCD_command(unsigned char cmd);
void LCD_data(unsigned char data);
void LCD_string(const char *str);
void LCD_clear(void);

#endif // LCD_H
