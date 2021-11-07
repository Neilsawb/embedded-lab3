#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdbool.h>

#include "led.h"
#include "serial.h"
#include "timer.h"

// Konfigurera upp pinne PD2 som ingång 
// som använder ett externt pulldown motstånd
// Part 2, Using Pin12 (key2) as PB4 not PD2.
bool debounceOngoing = false ;
bool buttonReleased = false;
bool prevState = false;
bool debounceOver = false ;
int counter;

int main () {

	uart_init(); // initialise Uart
	timer_init(); // define time zero. 16ms.
	
	DDRB = 0xFF; // set all pins as output.
	
	DDRB &= ~(1<<PB4); // input pin 12. PB4.
	
	while (1) {
		// check timer and wait
		//checkDebounce(debounceOngoing);
		if (PINB & (1<<PB4)) {
			debounceOngoing = true;
		} else {
						
			if (debounceOngoing == true) {
				buttonReleased = true;	
			}
		}
		if (debounceOver == true) { 
			// Write out message to Uart ’pushed\r\n’
			uart_putstr("pushed\r\n");
		}
		if (buttonReleased == true && debounceOver == true) {
			// Write out message to Uart ’released\r\n’
			uart_putstr("released\r\n");
			debounceOver = false;
			buttonReleased = false;
		}

		if (TCNT0 == 0) { // Wait to timer is reset then update debounceOver
			if (debounceOngoing == true && buttonReleased == true) {
				debounceOver = true;
				debounceOngoing = false;
			}
		}
	}
}

