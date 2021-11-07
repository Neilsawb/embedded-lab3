#include <avr/io.h>
#include <stdio.h>
#include <stdbool.h>

#include "serial.h"
#include "timer.h"

bool buttonReleased;
bool prevState;
bool debounceOver;
int counter;

static FILE uart_stdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

void uart_init(void) {
	UBRR0H = (unsigned char)(UBRR >> 8);
	UBRR0L = (unsigned char)(UBRR);
	UCSR0A = 0;
	UCSR0B = (1 << TXEN0 | 1 << RXEN0);
	UCSR0C = (1 << UCSZ01 | 1 << UCSZ00);

	stdout = &uart_stdout;
}

void timer_init() {

    TCCR0A |= (1<<WGM02);
    TCCR0A &= ~(1 << WGM00);
    TCCR0A &= ~(1 << WGM01);

    //  16MHz (clock cycle) /1024 (prescaler) = 15.625Hz
    // 15.625/((1/16)*1000) = 62 (use 65 instead, 62 result is too big) => OCR0A = 240 )
    TCCR0B |= (1 << CS02)| (0 << CS01) | (1 << CS00); // Prescaler = 1024.

    TCNT0 = 0;

    OCR0A = 255;

}

int uart_putchar(char chr, FILE *stream) {
	if (chr == '\n') {
		uart_putchar('\r', NULL);
	}
	while (!(UCSR0A & (1 << UDRE0)))
		;
	UDR0 = chr;
	return 0;
}

char uart_getchar(void) {
	while (!(UCSR0A & (1 << RXC0)))
		;
	return UDR0;
}

void uart_putstr(char *str)
{
        unsigned char j=0;

        while (str[j]!=0)               /* Send string till null */
        {
                uart_putchar(str[j], NULL);
                j++;
        }
}

bool checkDebounce(bool debounceOngoing) {
	
	if (PINB & (1<<PB4)) {
		PORTB |= (1<<PB3); // led on PB3
		debounceOngoing = true;
		//uart_putstr("debounce ongoing is true\r\n");
		return debounceOngoing;
	} else {
		//if (!PINB && (1<<PB4)) {  // Not true.
		PORTB &= ~(1<<PB3); // led off on PB3
		//uart_putstr("button released now\r\n");
		if (debounceOngoing == true) {
			buttonReleased = true;	
			uart_putstr("button released is true\r\n");
		}
	}
}

