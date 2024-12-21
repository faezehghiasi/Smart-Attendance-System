#include "Keypad.h"

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