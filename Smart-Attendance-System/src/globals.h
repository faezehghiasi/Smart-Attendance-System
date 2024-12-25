#ifndef GLOBALS_H
#define GLOBALS_H

// Declare global variables with 'extern' to make them accessible in other files
extern volatile unsigned char key_pressed;
extern volatile int current_page;
extern volatile int menu_needs_update;


// Declare the keypad array
extern const unsigned char keypad[4][3];

#endif // GLOBALS_H
