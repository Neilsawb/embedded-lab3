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

        while (str[j]!=0)  /* Send string till null */
        {
                uart_putchar(str[j], NULL);
                j++;
        }
}

void fastPWMmode(void) {
    // Part2 define Fast PWM mode.
    // WGM21=1, WGM20=1 Fast PWM mode 3 with non inverting. (COM2A1)

     TCCR2A |= (1<<COM2A1) | (1<<COM2A0) | (1<<WGM21) | (1<<WGM20);

    //  16MHz (clock cycle) /64(prescaler) = 250,000Hz
    // OCR2A = 16000000/(256*64)=16,384 => 976.

    //  16MHz (clock cycle) /1024 (prescaler) = 15.625Hz
    // 15.625/((1/10)*1000) = 156,25 (OCR0A = 156)
    //TCCR0B |= (1 << CS02)| (0 << CS01) | (1 << CS00); // Prescaler = 1024.
    TCCR2B |= (1 << CS01) | (1 << CS00); // Prescaler = 64.

    TCNT0 = 0;
    TCNT2 = 0;

    OCR2A = 255 ; // (25) 10% of duty cycle. (255) 100%.

    OCR0A = 156;
    // OCR0A = In Fast PWM mode, the counter is incremented
    // until the counter value matches the TOP value. The
    // counter is then cleared at the following timer clock cycle.
 }

void init_adc0() {
	ADCSRA |= 1<<ADEN;  //Enables ADC
	ADCSRA |= 1<<ADPS1 | 1<<ADPS0;  //Sets ADC Prescalar as 8, 
	ADCSRA &= ~(1<<ADPS2);
	ADCSRA |= 1<<ADIE | 1<<ADSC ;  //Enables ADC interupt and Start the conversion
	ADMUX |= 1<<ADLAR | 1<<REFS0; // ADLAR=1 for left adjusted result and REFS0=1 with REFS1=0 to use 
	ADMUX &= ~(1<<MUX0); // define ADMUX to 0000 for ADC0.  
	ADMUX &= ~(1<<MUX1);
	ADMUX &= ~(1<<MUX2);
	ADMUX &= ~(1<<MUX3);
											// Vcc as reference voltage
	ADCSRB |= (1 << ADTS0) | (1 << ADTS1);	// Timer/Counter0 Compare Match A 
	DIDR0 = (1 << ADC0D); // disable digital input buffer
}

void startConversion () {
	ADCSRA |= (1 <<ADSC);
}
