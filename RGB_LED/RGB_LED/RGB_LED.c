/*
 * RGB_LED.c
 *
 * Created: 2020-11-25 오전 9:13:52
 *  Author: user
 */ 
#define F_CPU 16000000UL
#define BAUDRATE(x) ((F_CPU / 16 / x) - 1)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned int Rx_buffer[5];
int USART1_rx_end = 0;

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

ISR(USART0_RX_vect){
	static uint8_t usart1_chk = 0;
	unsigned char buf = UDR0;
	int USART1_len = 0;
	
	USART1_rx_end = 0;
	switch(usart1_chk){
		case 0 :
			if(buf == 0x02){
				Rx_buffer[USART1_len] = buf;
				USART1_len++;
				usart1_chk = 1;
			}
			else usart1_chk = 0;
			break;
		case 1:
			Rx_buffer[USART1_len]= buf;
			USART1_len++;
			
			if(buf == 0x03){
				USART1_rx_end = 1;
				usart1_chk = 0;
			}
			break;
		default:
			usart1_chk = 0;
			break;
	}
}

void set_rgb(){
	int red, green, blue;
	if(USART1_rx_end == 1){
		red = Rx_buffer[1];
		OCR0 = red;
	
		green = Rx_buffer[2];
		OCR1A = green;

		blue = Rx_buffer[3];
		OCR1B = blue;
	
		uart_write(red);
		uart_write(green);
		uart_write(blue);
		 
		USART1_rx_end = 0;
	}
}

int main(void)
{
	uart_init(BAUDRATE(9600));
	
	DDRB = 0xff;
	PORTB = 0x00;
	TCCR0 = 0x6A;
	TCCR1A = 0x6A;
	TCCR1B = 0x6A;
	
	sei();
	//OCR0 = PB4
	//OCR1A = PB5
	//OCR1B = PB6
    while(1)
    {
		set_rgb();
    }
}