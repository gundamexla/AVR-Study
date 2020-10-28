/*
 * TouchSensor_with_Buzzer.c
 *
 * Created: 2020-10-28 오전 8:55:11
 *  Author: user
 */ 


#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#define BV(bit) (1 << bit)
volatile int tone;
char f_table[9] = {17,43,66,77,97,114,127,137,255};
ISR(TIMER0_OVF_vect){
	PORTB ^= 1<<4;
	TCNT0 = f_table[tone];
}

int main(void)
{
	int cnt = 0;
	
    while(1)
    {
        DDRA=0x00;          // PD0 = Sound Sensor
        DDRB=0x10;
		TCCR0 = 0x03;
		TIMSK = 0x01;
		sei();
		tone=8; 
        while(1)
        {
			if(PINA & 0x01){
				tone = cnt;
				_delay_ms(500);
				cnt ++;
				if(cnt==8) cnt=0;
			}
        }
    }
}