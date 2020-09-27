/*
	<AVR이 할 일>
	1. 2초마다 DHT 모듈로 온습도 읽기
	2. 2초마다 수위 센서로 수위 읽기
	3. 2초마다 조도 센서로 조도 읽기
		- AREF에 5V 연결했는지 확인할 것
	4. 읽은 값을 PC로 전송
	5. 온도가 낮으면 LED 점등
	6. 읽은 값을 LCD에 표시
	7. PC로부터 doservo 값을 받으면 servo 회전
*/
#include "main.h"

ISR(USART0_RX_vect)
{
	char *receiveData;

	UART0_ISR_Receive();

	if(rxReadyFlag == 1) {
		receiveData = getRxString();
		if(!StringSame(receiveData, "doservo", strlen("doservo"))) {
			servo_flag = 1;
		}
		if(!StringSame(receiveData, "senddata", strlen("senddata"))) {
			data_send = 1;
		}
	}
}

ISR(TIMER0_COMP_vect)
{
	static int j = 1;
	count++;
	if(servo_flag) {
		if((count % 1000) == 0) {
			if(j) {
				OCR1A = 125;
				j = 0;
			}
			else {
				OCR1A = 625;
				j = 1;
				servo_flag = 0;
			}
		}
	}
}


int main(void)
{
	InitSystem();

	char data[45] = {0, };
	char line[20] = {' ', };

	unsigned char temp[2] = {0, };
	unsigned char hum[2] = {0, };
	unsigned int water = 0;
	unsigned int light = 0;

	short shiftline = 0;

	while(1)
	{
		if((count%1000) == 0 && !data_send) {
			// Get Sensor Value
			DHT_Read(temp, hum);
			water = ReadADC(0);	_delay_us(10);
			light = ReadADC(1);	_delay_us(10);

			// LED Work
			if(temp[0] >= 28) {
				PORTD = 0x00;
			}
			else {
				PORTD = 0xff;
			}

			// UART
			memset(data, 0, sizeof(data));
			sprintf(data, "\ntemp:%04d_humi:%04d_water:%04d_food:%04d\n", temp[0], hum[0], water, light);

			// LCD Work
			sprintf(line, "Temp %2d  Humi %2d ", temp[0], hum[0]);
			LCD_YX(0, 0);	LCD_String(line);
			if(light > 100) {
				sprintf(line, "Water %03d  Food none    ", water);
			}
			else {
				sprintf(line, "Water %03d  Food FULL    ", water);
			}
			LCD_YX(1, 0);	LCD_String(line+shiftline);		shiftline = (shiftline+1) % 6;
		}

		if(data_send) {
			printf("%s", data);
			data_send = 0;
		}
	}

	return 0;
}

int StringSame(char *data, char *string, int length)
{
	int flag = 0;
	for(int i = 0; i<length; i++) {
		if(data[i] == string[i]) {
			flag = 0;
		}
		else {
			return 1;
		}
	}
	return flag;
}

unsigned int ReadADC(uint8_t channel)
{
	// YOU MUST INPUT CORD "_delay_us(10)" AFTER THIS FUNCTION!!!
	ADMUX = (0x00 | (channel & 0x07));
	ADCSRA |= 0xc7; // 1100 0111 : ADEN ADSC 0 0 0 ADPS2 ADPS1 ADPS0

	while(!(ADCSRA & (1 << ADIF))); // Waiting ADC Complete
	ADCSRA |= (1 << ADIF); // End of Converting

	return ADC;
}

void Timer1_Init()
{
	TCCR1A = (1 << COM1A1) | (1 << WGM11);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11) | (1 << CS10);
	ICR1 = 4999;
	OCR1A = 625;
	TCNT1 = 0X00;
}

void Timer0_Init()
{
	TCCR0 |= (1 << CS02) | (1 << CS01) | (1 << WGM01);
	OCR0 = 124;
	TIMSK |= (1 << OCIE0) | (0 << TOIE0);
	sei();
}

void InitSystem()
{
	Timer0_Init();
	Timer1_Init();
	UART0_init();
	DHT_Setup();
	LCD_Init();

	stdout = &OUTPUT;

	DDRB = 0xFF;	PORTB = 0x00;
	DDRD = 0xFF;
	DDRF = 0x00;
}
