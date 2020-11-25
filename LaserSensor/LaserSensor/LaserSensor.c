/*
 * LaserSensor.c
 *
 * Created: 2020-11-11 오전 9:14:10
 *  Author: user
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define R 0
#define G 1
#define B 2

#define INTRUSION 2
#define CLEAR 1
#define TURNON 0

#define BAUDRATE(x) ((F_CPU / 16 / x) - 1)

#define BV(bit) (1 << bit)
#define RGB(color) (PORTA = BV(color))

void uart_init(unsigned int baud)
{
	UBRR0H = (unsigned char)(baud >> 8);
	UBRR0L = (unsigned char)baud;
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);
}

void uart_write(unsigned char data)
{
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

void send_data(unsigned char stat){
	uart_write(stat);
}


void ADC_Init()
{
	DDRF = 0x00;
	ADCSRA = 0x87;
	ADMUX = 0x40;
}
int ADC_Read(char channel)
{
	//int ADC_value;
	
	ADMUX = (0x40) | (channel & 0x07);
	_delay_us(50);
	ADCSRA |= (1<<ADSC);
	while((ADCSRA &(1<<ADIF))== 0);
	
	ADCSRA |= (1<<ADIF);
	return ADCW;
}

int main(void)
{
	int cds;
	ADC_Init();
	uart_init(BAUDRATE(9600));
	DDRA = 0xff;
	DDRF = 0x00; // cds
	DDRB = 0xff; // laser
	
	RGB(G);
	_delay_ms(3000);
	send_data(TURNON);
	while(1)
    {
        //TODO:: Please write your application code
		cds = ADC_Read(0);
		
		PORTB = BV(0);
		
		if(cds > 80) {
			RGB(R);
			send_data(INTRUSION);
		}
		else {
			RGB(B);
			send_data(CLEAR);
		}
    }
}