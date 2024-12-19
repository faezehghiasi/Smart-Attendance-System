#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define LCD_PORT PORTC
#define LCD_DDR DDRC
#define RS PD0
#define EN PD3

#define KEY_PRT PORTA
#define KEY_DDR DDRA
#define KEY_PIN PINA

volatile unsigned char key_pressed = 0;  
volatile int current_page = 1;           
volatile int menu_needs_update = 1;      

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
unsigned char scan_keypad(void);
void init_interrupt(void);
void display_menu(int page);

int main(void) {
    LCD_init();
    keypad_init();
    init_interrupt();
    sei();

    while (1) {
        if (menu_needs_update) {
            LCD_clear();
            display_menu(current_page);
            menu_needs_update = 0;
        }

        if (key_pressed) {
            LCD_clear();
            if (key_pressed == '1') {
                LCD_string("Attendance Init");
            } else if (key_pressed == '2') {
                LCD_string("Student Manage");
            } else if (key_pressed == '3') {
                LCD_string("View Present");
            } else if (key_pressed == '4') {
                LCD_string("Temp Monitoring");
            } else if (key_pressed == '5') {
                LCD_string("Retrieve Data");
            } else if (key_pressed == '6') {
                LCD_string("Traffic Monitor");
            } else if (key_pressed == '#') {
                current_page++;
                if (current_page > 3) current_page = 1;
                menu_needs_update = 1;
            } else if (key_pressed == '*') {
                current_page--;
                if (current_page < 1) current_page = 3;
                menu_needs_update = 1;
            } else {
                LCD_string("Invalid Key");
            }
            key_pressed = 0;
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
    KEY_PRT = 0x70;  
}

unsigned char scan_keypad(void) {
    unsigned char row, colloc;

    for (row = 0; row < 4; row++) {
        KEY_PRT = 0xFF;
        KEY_PRT &= ~(1 << row);  
        _delay_us(5);  
        colloc = (KEY_PIN & 0x70);  
        if (colloc != 0x70) {  
            if ((colloc & 0x10) == 0) return keypad[row][0];
            else if ((colloc & 0x20) == 0) return keypad[row][1];
            else if ((colloc & 0x40) == 0) return keypad[row][2];
        }
    }
    return 0xFF;  
}

void init_interrupt(void) {
    // Configure INT0 for rising edge detection
    MCUCR |= (1 << ISC01) | (1 << ISC00);  // Set ISC01 and ISC00 for rising edge
    GICR |= (1 << INT0);    // Enable INT0 interrupt
    PORTD |= (1 << PD2);    // Enable pull-up resistor for INT0 pin (PD2)
}

// ISR for INT0 interrupt
ISR(INT0_vect) {
    // Scan the keypad when INT0 is triggered
    unsigned char temp_key = scan_keypad();
    if (temp_key != 0xFF) {  // Check if a valid key is detected
        key_pressed = temp_key;  // Store the detected key
    }

    KEY_PRT = 0x70; //اینجا دوباره حالت سطر هارو به حالت اولیه بر میکردونم
}