/*
 * DC_Motor.c
 *
 * Created: 2020-09-09 오전 10:10:25
 *  Author: user
 */ 


#include <avr/io.h>
#define F_CPU 16000000UL
#define BAUDRATE(x) ((F_CPU / 16 / x) - 1)
#include <util/delay.h>
#include <avr/interrupt.h>

#define BV(bit) (1 << bit)
#define GND(bit) (0 << bit)

// 과제 1
/*int main()
{
	int cnt = 0;
	
	DDRE = 0xFF;
	DDRA = 0x00;
	DDRB = 0xFF;  // OC0(PB4)출력설정
	TCCR0 = 0x7A;    // fast PWM mode : 3,6(WGM01,00) - 11, 5,4(COM ) - 11, 2-0(CS) - 010
	OCR0 = 0xFF;
	
	PORTE = BV(0);
	while(1) // 00 : 0%, 3F : 20%, 5F : 40%, 8F : 60%, CF : 80%, FF : 100%
	{
		
		if (~PINA & BV(0)){
			_delay_ms(500);
			cnt ++;
			if(cnt > 5) cnt = 5;
			switch(cnt){
				case 1:
					OCR0 = 0x80;
					PORTE = BV(1);
					break;
				
				case 2:
					OCR0 = 0x6F;
					PORTE = BV(2);
					break;
				case 3:
					OCR0 = 0x4F;
					PORTE = BV(3);
					break;
				case 4:
					OCR0 = 0x30;
					PORTE = BV(4);
					break;
				
				case 5:
					OCR0 = 0x00;
					PORTE = BV(5);
					break;
			}
		}
		if (~PINA & BV(1)){
			_delay_ms(500);
			cnt --;
			if(cnt < 0) cnt = 0;
			switch(cnt){
				case 0:
					OCR0 = 0xFF;
					PORTE = BV(0);
					break;
				case 1:
					OCR0 = 0x80;
					PORTE = BV(1);
					break;
				
				case 2:
					OCR0 = 0x6F;
					PORTE = BV(2);
					break;
				case 3:
					OCR0 = 0x4F;
					PORTE = BV(3);
					break;
				case 4:
					OCR0 = 0x30;
					PORTE = BV(4);
					break;
				
				case 5:
					OCR0 = 0x00;
					PORTE = BV(5);
					break;
			}
		}
		if (~PINA & BV(2)){
			_delay_ms(500);
			cnt = 0;
			OCR0 = 0xFF;
			PORTE = BV(0);
		}
	}
}
*/

// 과제 2

void uart_init(unsigned int baud)
{
	UBRR1H = (unsigned char)(baud >> 8);
	UBRR1L = (unsigned char)baud;
	UCSR1B = (1 << TXEN1) | (1 << RXEN1);
}

unsigned char uart_read()
{
	while (!(UCSR1A & (1 << RXC1)));
	return UDR1;
}

void uart_write(unsigned char data)
{
	while (!(UCSR1A & (1 << UDRE1)));
	UDR1 = data;
}

int main(){
	uart_init(BAUDRATE(9600));
	DDRE = 0xFF;
	DDRA = 0x00;
	DDRB = 0xFF;  // OC0(PB4)출력설정
	TCCR0 = 0x7A;    // fast PWM mode : 3,6(WGM01,00) - 11, 5,4(COM ) - 11, 2-0(CS) - 010
	OCR0 = 0xFF;
	
	PORTE = BV(0);
	while(1)
	{
		unsigned char num = uart_read();
		switch(num){
			case '0':
				OCR0 = 0xFF;
				PORTE = BV(0);
				uart_write('0');
				break;
			case '1':
				OCR0 = 0x80;
				PORTE = BV(1);
				uart_write('1');
				break;
			case '2':
				OCR0 = 0x6F;
				PORTE = BV(2);
				uart_write('2');
				break;
			case '3':
				OCR0 = 0x4F;
				PORTE = BV(3);
				uart_write('3');
				break;
			case '4':
				OCR0 = 0x30;
				PORTE = BV(4);
				uart_write('4');
				break;
			
			case '5':
				OCR0 = 0x00;
				PORTE = BV(5);
				uart_write('5');
				break;
		}
		
	}
}



// 모터 정방향, 역방향
/*
int main(void)
{
	PORTE = 0x00;
	DDRE = 0xFF;
    while(1)
    {
        PORTE = BV(3) | GND(4);
		_delay_ms(1000);
		PORTE = GND(3) | GND(4);
		_delay_ms(4000);
		PORTE = GND(3) | BV(4);
		_delay_ms(1000);
		PORTE = GND(3) | GND(4);
		_delay_ms(4000);
    }
}*/

