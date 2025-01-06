#ifndef EEPROM_H
#define EEPROM_H

#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#define STUDENT_START_ADDRESS 0x0000
#define NUM_OF_STUDENTS_HIGH_BYTE 0x03FE
#define NUM_OF_STUDENTS_LOW_BYTE 0x03FF

void EEPROM_Write(uint16_t address, uint8_t data);
uint8_t EEPROM_Read(uint16_t address);
void WRITE_STU_ID_IN_EEPROM(unsigned long int stuID);
unsigned long int READ_STU_ID_FROM_EEPROM(uint16_t index);
uint16_t GET_NUM_OF_STUDENTS();
void SET_NUM_OF_STUDENTS(uint16_t num_of_students);
bool handle_student_search(unsigned long int stuID);
void WRITE_STU_ID_IN_EEPROM_AT_INDEX(unsigned long int stuID, uint16_t index);
bool delete_student_from_eeprom(unsigned long int studentCode);

#endif
