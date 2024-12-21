#ifndef Keypad_H
#define Keypad_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "globals.h"


#define KEY_PRT PORTA
#define KEY_DDR DDRA
#define KEY_PIN PINA

void keypad_init(void);
unsigned char scan_keypad(void);
void init_interrupt(void);

#endif