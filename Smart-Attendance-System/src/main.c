#include <avr/io.h>
#include <util/delay.h>

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
            LCD_clear();
            LCD_string("Attendance Init");
            _delay_ms(2000);
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
