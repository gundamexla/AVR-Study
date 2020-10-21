/*
 * SoundSensor.c
 *
 * Created: 2020-10-21 오전 11:10:32
 *  Author: user
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define BV(bit) (1 << bit)

int main(void)
{
	DDRA=0x00;          // PD0 = Sound Sensor
	DDRF=0x00;			//PF0 = Switch
	DDRC=0xff;            // PB = LED
	while(1)
	{
		
		if(~PINF & BV(0)) PORTC = 0x00;
		
		if(~PINA & BV(0)){
			PORTC += 0x01;
			_delay_ms(500);
		}
	}
}