#include "time_limiter.h"

volatile uint32_t elapsed_time = 0; // Elapsed time in milliseconds
volatile bool time_up = false;
static uint32_t time_limit = 0; // Time limit in milliseconds

// Initialize Timer0 for 1 ms ticks
void init_timer(uint32_t time_limit_sec) {
    time_limit = time_limit_sec * 1000; // Convert time limit to milliseconds
    elapsed_time = 0;
    time_up = false;
    TCCR0 = (1 << WGM01);        // CTC mode
    OCR0 = 250;                  // Compare match value for 1 ms (assuming 16 MHz clock, prescaler = 64)
    TIMSK |= (1 << OCIE0);       // Enable Timer0 compare match interrupt
    TCCR0 |= (1 << CS01) | (1 << CS00); // Set prescaler to 64
}

// Timer0 Compare Match Interrupt Service Routine
ISR(TIMER0_COMP_vect) {
    elapsed_time++;              // Increment elapsed time
    if (elapsed_time >= time_limit) {
        time_up = true;          // Set time-up flag
        TCCR0 = 0;               // Stop the timer
    }
}

// Check if the time limit has been reached
bool is_time_up(void) {
    return time_up;
}
