#include "EEROM.h"

void GET_ARRAY_OF_STUDENTS(unsigned long int * students) {
    unsigned char size_of_array = (GET_EEPROM_LAST_ADDRESS() - 1) / 4;
    for (unsigned char i = 0; i < size_of_array; i++) {
        students[i] = READ_STU_ID_FROM_EEPROM(i * 4);
        // Print for debugging
        printf("Student ID %d: %lu\n", i, students[i]);
    }
}

unsigned long int  READ_STU_ID_FROM_EEPROM(uint16_t address){
    long int stuID = 0;

    stuID |= (long int)EEPROM_Read(address) << 24;  // MSB
    stuID |= (long int)EEPROM_Read(address + 1) << 16;
    stuID |= (long int)EEPROM_Read(address + 2) << 8;
    stuID |= (long int)EEPROM_Read(address + 3);     // LSB

    return stuID;
}

uint16_t GET_EEPROM_LAST_ADDRESS(){
    
    uint16_t address_of_EEROM=0xFFFF;
    address_of_EEROM &=(EEPROM_Read(0x03FE));
    address_of_EEROM<<=8;
    address_of_EEROM |= 0xFF;
    address_of_EEROM &=(EEPROM_Read(0x3FF));
    return address_of_EEROM;
}
void WRITE_STU_ID_IN_EEPROM(unsigned long int stuId){
    uint16_t address=GET_EEPROM_LAST_ADDRESS();
    EEPROM_Write(address,(stuId>>24)& 0xFF);
    EEPROM_Write(address+1,(stuId>>16)& 0xFF);
    EEPROM_Write(address+2,(stuId>>8)& 0xFF);
    EEPROM_Write(address+3,(stuId)& 0xFF);
    address+=4;
    EEPROM_Write(0x03fe,(address>>8)& 0xFF);
    EEPROM_Write(0x03ff, address& 0xFF);


}
void EEPROM_Write(uint16_t address, uint8_t data) {
    // Wait until the previous write operation is complete
    while (EECR & (1 << EEWE)) {
        // Wait for EEWE (EEPROM Write Enable) to clear
    }

    // Set up the address register (EEAR)
    EEARH = (address >> 8); // Set the high byte of the address
    EEARL = (address & 0xFF); // Set the low byte of the address

    // Set up the data register (EEDR)
    EEDR = data;

    // Start the write operation by setting the EEWE bit
    EECR |= (1 << EEMWE); // Enable master write
    EECR |= (1 << EEWE);  // Start the write operation
}

uint8_t EEPROM_Read(uint16_t address) {
    while(EECR & (1<<EEWE));
    // Set up the address register (EEAR)
    EEARH = (address >> 8);  // Set the high byte of the address
    EEARL = (address & 0xFF); // Set the low byte of the address

    // Start the read operation
    EECR |= (1 << EERE);  

    // Return the read data
    return EEDR;  // Data is stored in the EEDR (EEPROM Data Register)

}
