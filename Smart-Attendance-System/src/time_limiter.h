#ifndef TIME_LIMITER_H
#define TIME_LIMITER_H

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>

// Function prototypes
void init_timer(uint32_t time_limit_sec);
bool is_time_up(void);

#endif // TIME_LIMITER_H
