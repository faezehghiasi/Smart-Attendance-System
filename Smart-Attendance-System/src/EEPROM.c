#include "EEPROM.h"

void EEPROM_Write(uint16_t address, uint8_t data) {
    while (EECR & (1 << EEWE));  // Wait for the completion of the previous write operation
    EEARH = (address >> 8);      // Set the high byte of the address
    EEARL = (address & 0xFF);    // Set the low byte of the address
    EEDR = data;                 // Set the data to write
    EECR |= (1 << EEMWE);        // Enable master write
    EECR |= (1 << EEWE);         // Start the write operation
}
//**********************************************************************************************************
uint8_t EEPROM_Read(uint16_t address) {
    while (EECR & (1 << EEWE));  // Wait for the completion of any previous write operation
    EEARH = (address >> 8);      // Set the high byte of the address
    EEARL = (address & 0xFF);    // Set the low byte of the address
    EECR |= (1 << EERE);         // Start the read operation
    return EEDR;                 // Return the read data
}
//**********************************************************************************************************
void WRITE_STU_ID_IN_EEPROM(unsigned long int stuID) {
    uint16_t num_of_students = GET_NUM_OF_STUDENTS();  // Get the number of stored students
    uint16_t address = STUDENT_START_ADDRESS + (num_of_students * 4);  // Calculate the address for the new student

    // Write the student ID in 4 bytes
    EEPROM_Write(address, (stuID >> 24) & 0xFF);      // Write the first byte
    EEPROM_Write(address + 1, (stuID >> 16) & 0xFF);  // Write the second byte
    EEPROM_Write(address + 2, (stuID >> 8) & 0xFF);   // Write the third byte
    EEPROM_Write(address + 3, stuID & 0xFF);          // Write the fourth byte

    // Update the number of students
    num_of_students++;
    SET_NUM_OF_STUDENTS(num_of_students);
}
//**********************************************************************************************************
unsigned long int READ_STU_ID_FROM_EEPROM(uint16_t index) {
    uint16_t address = STUDENT_START_ADDRESS + (index * 4);  // Calculate the address based on the index
    unsigned long int stuID = 0;

    // Read the 4 bytes of the student ID
    stuID |= ((unsigned long int)EEPROM_Read(address) << 24);     // Read the first byte
    stuID |= ((unsigned long int)EEPROM_Read(address + 1) << 16); // Read the second byte
    stuID |= ((unsigned long int)EEPROM_Read(address + 2) << 8);  // Read the third byte
    stuID |= (unsigned long int)EEPROM_Read(address + 3);         // Read the fourth byte

    return stuID;
}
//**********************************************************************************************************
uint16_t GET_NUM_OF_STUDENTS() {
    uint16_t num_of_students = 0;
    uint8_t high_byte = EEPROM_Read(NUM_OF_STUDENTS_HIGH_BYTE);  // Read the high byte
    uint8_t low_byte = EEPROM_Read(NUM_OF_STUDENTS_LOW_BYTE);    // Read the low byte

    // Combine the two bytes to get the total number of students
    num_of_students = (high_byte << 8) | low_byte;
    return num_of_students;
}
//**********************************************************************************************************
void SET_NUM_OF_STUDENTS(uint16_t num_of_students) {
    EEPROM_Write(NUM_OF_STUDENTS_HIGH_BYTE, (num_of_students >> 8) & 0xFF);  // Write the high byte
    EEPROM_Write(NUM_OF_STUDENTS_LOW_BYTE, num_of_students & 0xFF);          // Write the low byte
}
//**********************************************************************************************************
void GET_ARRAY_OF_STUDENTS(unsigned long int *students) {
    uint16_t num_of_students = GET_NUM_OF_STUDENTS();  // Get the number of stored students
    for (uint16_t i = 0; i < num_of_students; i++) {
        students[i] = READ_STU_ID_FROM_EEPROM(i);  // Read each student ID from EEPROM
    }
}
//**********************************************************************************************************
bool delete_student_from_eeprom(unsigned long int studentCode) {
    uint16_t num_of_students = GET_NUM_OF_STUDENTS();  // Get the number of stored students
    bool found = false;

    for (uint16_t i = 0; i < num_of_students; i++) {
        unsigned long int current_student = READ_STU_ID_FROM_EEPROM(i);
        if (current_student == studentCode) {
            found = true;
            if (num_of_students == 1) {
                // If only one student exists, clear the first slot
                WRITE_STU_ID_IN_EEPROM_AT_INDEX(0, 0);  // Clear the first slot
            } else {
                // Shift the remaining student IDs to fill the gap
                for (uint16_t j = i; j < num_of_students - 1; j++) {
                    unsigned long int next_student = READ_STU_ID_FROM_EEPROM(j + 1);
                    WRITE_STU_ID_IN_EEPROM_AT_INDEX(next_student, j);  // Shift the data
                    _delay_ms(4);  
                }
                // Clear the last slot
                WRITE_STU_ID_IN_EEPROM_AT_INDEX(0, num_of_students - 1);
                _delay_ms(4);  
            }
            // Decrease the number of students
            num_of_students--;
            SET_NUM_OF_STUDENTS(num_of_students);
            _delay_ms(4);  
            break;
        }
    }
    return found;
}
//**********************************************************************************************************
void WRITE_STU_ID_IN_EEPROM_AT_INDEX(unsigned long int stuID, uint16_t index) {
    uint16_t address = index * 4;  // Calculate the address based on the index

    // Write the student ID in 4 bytes
    EEPROM_Write(address, (stuID >> 24) & 0xFF);      // Write the first byte
    EEPROM_Write(address + 1, (stuID >> 16) & 0xFF);  // Write the second byte
    EEPROM_Write(address + 2, (stuID >> 8) & 0xFF);   // Write the third byte
    EEPROM_Write(address + 3, stuID & 0xFF);          // Write the fourth byte
}
//**********************************************************************************************************
void CLEAR_EEPROM(void) {
    uint16_t eeprom_size = 1024;  // Maximum size of EEPROM (adjust based on your microcontroller)
    for (uint16_t address = 0; address < eeprom_size; address++) {
        EEPROM_Write(address, 0x00);  // Write 0x00 to each byte
        _delay_ms(4);  // Small delay to ensure write completion
    }

    // Reset the number of students to zero
    SET_NUM_OF_STUDENTS(0);
}
