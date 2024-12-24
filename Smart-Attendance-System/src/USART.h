#ifndef USART_H
#define USART_H

#include<avr/io.h>
#include <util/delay.h>

#define F_CPU 16000000UL // 16 MHz clock frequency
#define BAUD 9600
#define MYUBRR ((F_CPU / 16 / BAUD) - 1)


void USART_init(unsigned int ubrr);
void USART_Transmit_Data(unsigned char data);
void USART_Transmit_string(const char *str) ;


#endif
