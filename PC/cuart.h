#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <string.h>

#define MAX_BUFFER 120
#define CORRECT 40

// CORRECT 40 "\ntemp:%04d_humi:%04d_water:%04d_food:%04d\n"

HANDLE hSerial;

int UART_Init();
int WriteUART(char *_string);
int ReadUART(char *_string);
void DataVarification(char *data, int data_length, char *string);
void QuitUART();
