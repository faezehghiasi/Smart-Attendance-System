#include "USART.h"
void USART_init(unsigned int ubrr) {
    UBRRL = (unsigned char)ubrr;  // Set low byte of UBRR
    UBRRH = (unsigned char)(ubrr >> 8);  // Set high byte of UBRR
    UCSRB = (1 << RXEN) | (1 << TXEN);  // Enable receiver and transmitter
    UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);  // Set UCSZ1, UCSZ0 for 8-bit data
}

void USART_Transmit_Data(unsigned char data)
{
    while(!(UCSRA &(1<<UDRE)));
    UDR = data; 
}

void USART_Transmit_string(const char *str) {
    while (*str) {
        USART_Transmit_Data(*str++);
    }
}