#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdbool.h>

#define LCD_PORT PORTC
#define LCD_DDR DDRC
#define RS PD0
#define EN PD3

#define KEY_PRT PORTA
#define KEY_DDR DDRA
#define KEY_PIN PINA

const unsigned char keypad[4][3] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};

void LCD_init(void);
void LCD_command(unsigned char cmd);
void LCD_data(unsigned char data);
void LCD_string(const char *str);
void LCD_clear(void);

void keypad_init(void);
unsigned char detect_columns(void);
unsigned char scan_keypad(void);
unsigned char get_key(void);

void display_menu(int page);
void Attendance_Ready();
void Submit_Student_Code();
uint8_t EEPROM_Read(uint16_t address);
uint16_t GET_EEPROM_LAST_ADDRESS();
void EEPROM_Write(uint16_t address, uint8_t data);
void WRITE_STU_ID_IN_EEPROM(unsigned long int stuId);
void BUZZER();
bool check_stu_id(char * stuId);
void Wrong_format_of_stu_ID();

int main(void) {
    LCD_init();
    keypad_init();
    unsigned char key;
    int current_page = 1;

    while (1) {
        LCD_clear();
        display_menu(current_page);
        key = get_key();

        if (key == '1') {
            while(1){
                LCD_clear();
                Attendance_Ready();
                unsigned char key2=get_key();
                if (key2 == '1'){
                    LCD_clear();
                    Submit_Student_Code();
                }
                else if(key2 =='2'){
                    break;
                }
            }
        } else if (key == '2') {
            LCD_clear();
            LCD_string("Student Manage");
            _delay_ms(2000);
        } else if (key == '3') {
            LCD_clear();
            LCD_string("View Present");
            _delay_ms(2000);
        } else if (key == '4') {
            LCD_clear();
            LCD_string("Temp Monitoring");
            _delay_ms(2000);
        } else if (key == '5') {
            LCD_clear();
            LCD_string("Retrieve Data");
            _delay_ms(2000);
        } else if (key == '6') {
            LCD_clear();
            LCD_string("Traffic Monitor");
            _delay_ms(2000);
        } else if (key == '#') {
            current_page++;
            if (current_page > 3) current_page = 1;
        } else if (key == '*') {
            current_page--;
            if (current_page < 1) current_page = 3;
        }
    }
    return 0;
}

void display_menu(int page) {
    LCD_clear();
    if (page == 1) {
        LCD_string("1. Attendance Initialization");
        LCD_command(0xC0);
        LCD_string("2. Student Management");
    } else if (page == 2) {
        LCD_string("3. View Present Students");
        LCD_command(0xC0);
        LCD_string("4. Temperature Monitoring");
    } else if (page == 3) {
        LCD_string("5. Retrieve Student Data");
        LCD_command(0xC0);
        LCD_string("6. Traffic Monitoring");
    } else {
        LCD_clear();
        LCD_string("Invalid Page");
        _delay_ms(2000);
    }
}

void LCD_init(void) {
    LCD_DDR = 0xFF;
    DDRD |= (1 << RS) | (1 << EN);
    _delay_ms(20);

    LCD_command(0x38);
    LCD_command(0x0C);
    LCD_command(0x06);
    LCD_command(0x01);
    _delay_ms(2);
}

void LCD_command(unsigned char cmd) {
    LCD_PORT = cmd;
    PORTD &= ~(1 << RS);
    PORTD |= (1 << EN);
    _delay_us(1);
    PORTD &= ~(1 << EN);
    _delay_ms(2);
}

void LCD_data(unsigned char data) {
    LCD_PORT = data;
    PORTD |= (1 << RS);
    PORTD |= (1 << EN);
    _delay_us(1);
    PORTD &= ~(1 << EN);
    _delay_ms(2);
}

void LCD_string(const char *str) {
    while (*str) {
        LCD_data(*str++);
    }
}

void LCD_clear(void) {
    LCD_command(0x01);
    _delay_ms(2);
}

void keypad_init(void) {
    KEY_DDR = 0x0F;
    KEY_PRT = 0xFF;
}

unsigned char detect_columns(void) {
    return (KEY_PIN & 0x70);
}

unsigned char scan_keypad(void) {
    unsigned char row, colloc;

    for (row = 0; row < 4; row++) {
        KEY_PRT = ~(1 << row);
        _delay_us(5);

        colloc = detect_columns();
        if (colloc != 0x70) {
            if ((colloc & 0x10) == 0) return keypad[row][0];
            else if ((colloc & 0x20) == 0) return keypad[row][1];
            else if ((colloc & 0x40) == 0) return keypad[row][2];
        }
    }
    return 0xFF;
}

unsigned char get_key(void) {
    unsigned char key;

    do {
        key = scan_keypad();
    } while (key == 0xFF);

    while (scan_keypad() != 0xFF);
    return key;
}
void Attendance_Ready(){
    LCD_clear();
    LCD_string("1.Submit Student Code");
    LCD_command(0xC0);
    LCD_string("2.EXIT");

}
void Submit_Student_Code(){
    LCD_string("Enter your Student Code:");
    char Student_Code[8];
    unsigned char i=0;
    while(1){
        unsigned char digit;
        digit=get_key();
        if(i>=8 && digit !='#'){
            Wrong_format_of_stu_ID();
            break;
        }
        LCD_data(digit);
         if(i==8 && digit=='#'){
            unsigned long int stuID=atoi(Student_Code);

            if(check_stu_id(Student_Code)==1){
                LCD_command(0xC0);
                LCD_string("Your Student Code has been submitted");
                WRITE_STU_ID_IN_EEPROM(stuID);
                _delay_ms(20);
                break;
            }
            else{
                Wrong_format_of_stu_ID();
                break;
            }
        }
        else if(-1<i<8 && digit=='#'){
            Wrong_format_of_stu_ID();
            break;
        }

        if(0<=i<8 ){
            
            
            

        Student_Code[i]=digit;
        i++;
        }
    }


}
bool check_stu_id(char * stuId){
    if (stuId[0]!='4' || stuId[1]!='0' || stuId[2]!='1')
        return false;
    else
        return true;
}
void Wrong_format_of_stu_ID(){
    BUZZER();
    LCD_command(0xC0);
    LCD_string("Your Student Code format is wrong");
    _delay_ms(20);
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
void BUZZER(){
    DDRB |=(1<<2);
    PORTB |=(1<<2);
    _delay_ms(50);
    PORTB &=~(1<<2);

}