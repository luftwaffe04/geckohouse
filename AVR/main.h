#ifndef MAIN_H_
#define MAIN_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "uart0.h" 	// UART0 관련 헤더
#include "DHT.h"	// DHT11 관련 헤더
#include "avr_lcd.h"// LCD 관련 헤더

// Global Variable
FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);
volatile unsigned int count = 0;
volatile short servo_flag = 0;
volatile short data_send = 0;

// Extern Variable
extern volatile int i;
extern volatile unsigned char buffer[400];
extern volatile int rxReadyFlag;

// Function Area
void InitSystem(void);
void Timer0_Init(void);
void Timer1_Init(void);
unsigned int ReadADC(uint8_t channel);
int StringSame(char *data, char *string, int length);

#endif /* MAIN_H_ */
