#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>

#define F_CPU 16000000UL // Clock speed for atmega32
#define PB0 0  // Pin 0 of PORTB (Trigger)
#define PB1 1  // Pin 1 of PORTB (Echo)
#define US_PORT PORTB     // Ultrasonic sensor connected to PORTB
#define US_PIN PINB       // Ultrasonic PIN register
#define US_DDR DDRB       // Ultrasonic data direction register

#define US_TRIG_POS PB0   // Trigger pin connected to PB0
#define US_ECHO_POS PB1   // Echo pin connected to PB1

// Error indicators
#define US_ERROR -1       // Error indicator
#define US_NO_OBSTACLE -2 // No obstacle indicator

volatile int count; // Counting variable

void HCSR04Init();
void HCSR04Trigger();
uint16_t GetPulseWidth();





#endif

