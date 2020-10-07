/*
 * UltraSonic_Wave.c
 *
 * Created: 2020-09-23 오전 9:59:59
 *  Author: user
 */ 


#include <avr/io.h>
#define F_CPU 16000000UL
#define BAUDRATE(x) ((F_CPU / 16 / x) - 1)
#include <util/delay.h>

#define TRIG 6
#define ECHO 7
#define SOUND_VELOCITY 340UL


#define BV(bit) (1 << bit)

void uart_init(unsigned int baud)
{
	UBRR1H = (unsigned char)(baud >> 8);
	UBRR1L = (unsigned char)baud;
	UCSR1B = (1 << TXEN1) | (1 << RXEN1);
}

void uart_write(unsigned char data)
{
	while (!(UCSR1A & (1 << UDRE1)));
	UDR1 = data;
}

int main(void)
{
	uart_init(BAUDRATE(9600));
	
	unsigned int distance;
	DDRC = 0x00;
	DDRE = ((DDRE | (1<<TRIG)) & ~(1<<ECHO));
	
    while(1)
    {
        TCCR1B = 0x03;
        PORTE &= ~(1<<TRIG);
        _delay_us(10);
        PORTE |= (1<<TRIG);
        _delay_us(10);
        PORTE &= ~(1<<TRIG);
        while(!(PINE & (1<<ECHO))); 
        TCNT1 = 0x0000;
        while(PINE & (1<<ECHO));
        TCCR1B = 0x00;
        distance = (unsigned int)(SOUND_VELOCITY * (TCNT1 * 4 / 2) / 1000);
		
		if(distance < 30){
			PORTC = BV(0);
			uart_write('1');
		}
		else if(distance < 70){
			PORTC = BV(1);
			uart_write('2');
		}
		else if(distance < 100){
			PORTC = BV(2);
			uart_write('3');
		}
		else if(distance < 130){
			PORTC = BV(3);
			uart_write('4');
		}
		else if(distance < 200){
			PORTC = BV(4);
			uart_write('5');
		}
		else if(distance > 200){
			PORTC = BV(5);
			uart_write('6');
		}
		
    }
}