#ifndef SERIAL_H_
#define SERIAL_H_

#define BAUDRATE 38400
#define UBRR (F_CPU/16/BAUDRATE-1)

void uart_init(void);
int uart_putchar(char chr, FILE *stream);
char uart_getchar(void);
bool checkDebounce(bool debounceOngoing);
void uart_putstr(char *str);
void init_adc0();
void fastPWMmode(void);
void startConversion();

#endif

