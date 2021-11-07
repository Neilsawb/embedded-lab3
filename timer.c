#include <avr/io.h>
#include "timer.h"

void timer_init() {

    TCCR0A |= (1<<WGM02);
    TCCR0A &= ~(1 << WGM00);
    TCCR0A &= ~(1 << WGM01);

    //  16MHz (clock cycle) /1024 (prescaler) = 15.625Hz
    // 15.625/((1/16)*1000) = 62 (use 65 instead, 62 result is too big) => OCR0A = 240 )
    TCCR0B |= (1 << CS02)| (0 << CS01) | (1 << CS00); // Prescaler = 1024.

    TCNT0 = 0;

    OCR0A = 240;

}

