#include "globals.h"

// Define global variables
volatile unsigned char key_pressed = 0;
volatile int current_page = 1;
volatile int menu_needs_update = 1;

// Define the keypad array
const unsigned char keypad[4][3] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};
