#ifndef EEROM_H
#define EEROM_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "globals.h"


uint8_t EEPROM_Read(uint16_t address);
uint16_t GET_EEPROM_LAST_ADDRESS();
void EEPROM_Write(uint16_t address, uint8_t data);
void WRITE_STU_ID_IN_EEPROM(unsigned long int stuId);
unsigned long int  READ_STU_ID_FROM_EEPROM(uint16_t address);
void GET_ARRAY_OF_STUDENTS(unsigned long int * students);

#endif