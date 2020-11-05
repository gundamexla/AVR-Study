/*
 * BallSwitch_with_IR.c
 *
 * Created: 2020-11-04 오전 9:48:38
 *  Author: user
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#define BAUDRATE(x) ((F_CPU / 16 / x) - 1)

#define BV(bit) (1 << bit)

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

void send_data(char ball, char ir){
	uart_write(0x02);
	uart_write(ball);
	uart_write(ir);
	uart_write(0x03);
}

int main(void)
{
	DDRA=0x00;
	uart_init(BAUDRATE(9600));
	DDRC=0xff; 
	
    while(1)
    {
        //TODO:: Please write your application code
		if(~PINA & BV(0)){
			PORTC = 0x01;
			send_data(1,0);
			_delay_ms(100);
			PORTC = 0x00;
		} 
		if(~PINA & BV(1)){
			PORTC = 0x04;
			send_data(0,1);
			_delay_ms(100);
			PORTC = 0x00;
		}
    }
}