#include "time_limiter.h"

volatile uint32_t elapsed_time = 0; // in milliseconds
volatile bool time_up = false;
static uint32_t time_limit = 0; // in milliseconds
//**********************************************************************************************************
void init_timer(uint32_t time_limit_sec) {
    time_limit = time_limit_sec * 1000; 
    elapsed_time = 0;
    time_up = false;

    // prescaler = 64 -> f = 0.25 MHz
    //هر کلاک ساعت 4 میکرو ثانیه طول میکشه
    //برای شمارش 1 میلی ثانیه باید 249 مقایسه بشه یعنی 250 تا بشماره

    TCCR0 = (1 << WGM01);        // CTC mode
    OCR0 = 249;                  // Compare match value for 1 ms (assuming 16 MHz clock, prescaler = 64)
    TIMSK |= (1 << OCIE0);       // Enable Timer0 compare match interrupt
    TCCR0 |= (1 << CS01) | (1 << CS00); // Set prescaler to 64

}
//**********************************************************************************************************
ISR(TIMER0_COMP_vect) {
    elapsed_time++;              // Increment elapsed time
    if (elapsed_time >= time_limit) {
        time_up = true;          // Set time-up flag
        TCCR0 = 0;               // Stop the timer
    }
    TIFR |= (1 << OCF0);         // Clear the OCF0 flag
}
//**********************************************************************************************************
bool is_time_up(void) {
    return time_up;
}
