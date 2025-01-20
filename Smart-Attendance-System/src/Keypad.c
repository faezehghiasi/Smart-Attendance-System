#include "Keypad.h"

void keypad_init(void) {
    KEY_DDR = (KEY_DDR & 0x80) | 0x0F; // تنظیم 4 بیت اول به 1، 3 بیت بعدی به 0 و بیت آخر بدون تغییر
    KEY_PRT = (KEY_DDR & 0x80) | 0x70; // تنظیم 4 بیت اول به 0، 3 بیت بعدی به 1 و بیت آخر بدون تغییر
    // DDR = 0 & PORT = 1 --> pull up 
    // Columns are pull up
}
//**********************************************************************************************************
unsigned char scan_keypad(void) {
    unsigned char row, colloc;
    for (row = 0; row < 4; row++) {
        KEY_PRT = (KEY_DDR & 0x80) | 0x7F; 
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
//**********************************************************************************************************
void init_interrupt(void) {
    // Configure INT0 for rising edge detection
    MCUCR |= (1 << ISC01) | (1 << ISC00);  // Set ISC01 and ISC00 for rising edge
    GICR |= (1 << INT0);    // Enable INT0 interrupt -- GICR -> General Interrupt Control Register
    PORTD |= (1 << PD2);    // Enable pull-up resistor for INT0 pin (PD2)
}
//**********************************************************************************************************
ISR(INT0_vect) {
    unsigned char temp_key = scan_keypad();
    if (temp_key != 0xFF) {  // Check if a valid key is detected
        key_pressed = temp_key;  
    }
    KEY_PRT = (KEY_DDR & 0x80) | 0x70;
}