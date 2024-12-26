#include "ULTRASONIC.h"

volatile int count=0;

void HCSR04Init() {
    US_DDR |= (1 << US_TRIG_POS); // Trigger pin as output
    US_DDR &= ~(1 << US_ECHO_POS); // Echo pin as input
}

void HCSR04Trigger() {
    US_PORT |= (1 << US_TRIG_POS); // Set trigger pin high
    _delay_us(15);                  // Wait for 15 microseconds
    US_PORT &= ~(1 << US_TRIG_POS); // Set trigger pin low
}

uint16_t GetPulseWidth() {
    uint32_t i, result;

    // Wait for rising edge on Echo pin
    for (i = 0; i < 600000; i++) {
        if (!(US_PIN & (1 << US_ECHO_POS)))
            continue;
        else
            break;
    }

    if (i == 600000)
        return US_ERROR; // Timeout error if no rising edge detected

    // Start timer with prescaler 8
    TCCR1A = 0x00;
    TCCR1B = (1 << CS11);
    TCNT1 = 0x00; // Reset timer

    // Wait for falling edge on Echo pin
    for (i = 0; i < 600000; i++) {
        if (!(US_PIN & (1 << US_ECHO_POS)))
            break;  // Falling edge detected
        if (TCNT1 > 60000)
            return US_NO_OBSTACLE; // No obstacle in range
    }

    result = TCNT1; // Capture timer value
    TCCR1B = 0x00; // Stop timer

    if (result > 60000)
        return US_NO_OBSTACLE;
    else
        return (result >> 1); // Return the measured pulse width
}