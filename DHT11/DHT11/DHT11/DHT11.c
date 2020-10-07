/*
 * DHT11.c
 *
 * Created: 2020-10-07 오전 9:26:48
 *  Author: user
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
#define BV(bit) (1 << bit)
#define DHT11_PIN 6

uint8_t c=0,I_RH,D_RH,I_Temp,D_Temp,CheckSum;

void COMMAND(unsigned char byte);	// COMMAND 함수 선언
void DATA(unsigned char byte);		// DATA 함수 선언
void LCD_INIT(void);			// LCD_INIT 함수 선언
void LCD_CLEAR();
void DATASTR(const char* str); 
void MOVE(int y, int x);
void Request();				// ATMega128로 스타트 펄스 전달 & 응답 과정
void Response();			// 온습도 센서로부터 응답
uint8_t Receive_data();

int main(void)
{
	DDRB = 0xff;
	PORTB = 0x00;
	
	char data[5];
	LCD_INIT();
	MOVE(1,1);
	DATASTR("Humidity =");
	MOVE(2,1);
	DATASTR("Temp =");
	
    while(1)
    {
        Request();
		Response();
		I_RH = Receive_data(); // 습도의 정수
		D_RH = Receive_data(); // 습도의 실수
		I_Temp = Receive_data(); //온도의 정수
		D_Temp = Receive_data(); //온도의 실수
		CheckSum = Receive_data(); //모든 세그먼트의 체크섬
		
		if((I_RH+D_RH+I_Temp+D_Temp)!=CheckSum){
			LCD_CLEAR();
			DATASTR("ERROR");
		}
		else{
			itoa(I_RH, data, 10);
			MOVE(1, 12);
			DATASTR(data);
			DATA('.');
			
			itoa(D_RH, data, 10);
			DATASTR(data);
			DATA('%');
			
			itoa(I_Temp, data, 10);
			MOVE(2, 8);
			DATASTR(data);
			DATA('.');
			
			itoa(D_Temp, data, 10);
			DATASTR(data);
			DATA(0xDF);
			DATA('C');
		}
		_delay_ms(3000);
    }
}

void Request()				// ATMega128로 스타트 펄스 전달 & 응답 과정
{
	DDRD |= BV(DHT11_PIN);
	PORTD &= ~BV(DHT11_PIN);	// PD4 LOW
	_delay_ms(20);
	PORTD |= BV(DHT11_PIN);	// PD4 HIGH
}

void Response()				// 온습도 센서로부터 응답
{
	DDRD &= ~BV(DHT11_PIN); // PD4 LOW
	while(PIND & BV(DHT11_PIN));
	while((PIND & BV(DHT11_PIN))==0);
	while(PIND & BV(DHT11_PIN));
}

uint8_t Receive_data()
{
	for (int q=0; q<8; q++)
	{
		while((PIND & BV(DHT11_PIN)) == 0); //비트가 0인지 1인지 체크
		_delay_us(30);
		if(PIND & BV(DHT11_PIN)) //HIGH가 30ms보다 크면
		c = (c<<1)|(0x01);	 //HIGH 상태
		else
		c = (c<<1); //LOW 상태
		while(PIND & BV(DHT11_PIN));
	}
	return c;
}

void COMMAND(unsigned char byte)
{
	_delay_ms(2);

	PORTB=byte&0xf0;// 상위4비트 명령어 쓰기
	PORTB&=0b11111100;  // RS = 0, RW = 0, 명령어를 쓰도록 설정
	_delay_us(1);
	PORTB|=0b00000100; // E = 1, lcd 동작
	_delay_us(1);
	PORTB&=0b11111011;// E = 0

	PORTB=(byte<<4)&0xf0;// 하위4비트 명령어 쓰기
	PORTB&=0b11111100;// RS = 0, RW = 0, 명령어를 쓰도록 설정
	_delay_us(1);
	PORTB|=0b00000100; // E = 1, lcd 동작
	_delay_us(1);
	PORTB&=0b11111011;// E = 0
}
void DATA(unsigned char byte)
{
	_delay_ms(2);

	PORTB=byte&0xf0;// 상위4비트 명령어 쓰기
	PORTB |= 0b00000001;// RS = 1, 데이터 설정
	PORTB &= 0b11111101;// RW = 0, 쓰기 설정
	_delay_us(1);
	PORTB|=0b00000100; // E = 1, lcd 동작
	_delay_us(1);
	PORTB&=0b11111011;// E = 0

	PORTB=(byte<<4)&0xf0;// 하위4비트 데이터 쓰기
	PORTB |= 0b00000001;// RS = 1, 데이터 설정
	PORTB &= 0b11111101;// RW = 0, 쓰기 설정
	_delay_us(1);
	PORTB|=0b00000100; // E = 1, lcd 동작
	_delay_us(1);
	PORTB&=0b11111011;// E = 0
}
void LCD_INIT(void)
{
	_delay_ms(30);	// 전원 투입 후 30ms 이상 지연
	
	//Function set
	COMMAND(0b00101000);
	// 인터페이스(DL)=0(4bit), 라인(N)=1(2라인), 폰트(F)=0(5*8 dot)
	_delay_us(39);	// 39us 이상 지연

	//Display ON/OFF Control
	COMMAND(0b00001100);
	// 화면 표시(D)=1(on), 커서(C)=0(off), 블링크(B)=0(off)
	_delay_us(39);	// 39us 이상 지연

	//Clear Display
	COMMAND(0b00000001);
	// 화면을 클리어하고 , 커서가 홈위치인 0번지로 돌아감.
	_delay_ms(1.53);	// 1.53ms 이상 지연

	//Entry Mode Set
	COMMAND(0b00000110);
	// 커서방향(I/D)=1(address증가), 표시이동(S)=0(이동하지 않음)
}

void LCD_CLEAR(){
	COMMAND(0b00000001);
	// 화면을 클리어하고 , 커서가 홈위치인 0번지로 돌아감.
	_delay_ms(1.53);	// 1.53ms 이상 지연
}

void DATASTR(const char* str){
	
	for(int i =0; str[i]; i++){
		DATA(str[i]);
	}
}

void MOVE(int y, int x)     //커서 이동
{
	unsigned char data;
	if(y==1) 	data=0x80+x-1;           //1행
	else     	data=0xc0+x-1;            //2행
	COMMAND(data);
}