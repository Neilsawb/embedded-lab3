#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdbool.h>

#include "led.h"
#include "serial.h"
#include "timer.h"

uint16_t dutyCycle = 0;

ISR(ADC_vect) {
	    dutyCycle = ADC;
		OCR2A = dutyCycle;
		startConversion(); // start ADC after each interupt.
}


// Konfigurera upp pinne PD2 som ingång 
// som använder ett externt pulldown motstånd
// Part 2, Using Pin12 (key2) as PB4 not PD2.
bool debounceOngoing = false ;
bool buttonReleased = false;
bool prevState = false;
bool debounceOver = false ;
int counter;
char dutyChar[4];

// Bit 3 – ADIE: ADC Interrupt Enable
// Bit 6 – ADC0: ADC Conversion Result

int main () {

	DDRB |= (1<<PB3); // output pin
	PORTB |= (1<<PB3); // light led on PB3. Red led.

    fastPWMmode(); // setup fast PWM mode non inverting.

    timer_init(); // setup simple CTC timer with 16ms delay.
	init_adc0();
	startConversion(); // start ADC0 sampling on ADC0.

	sei(); // Enable interupts

    while (1) { // Loop infinitely
                
    }
}

