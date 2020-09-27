#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define LCD_DDR DDRC
#define LCD_PORT PORTC
#define LCD_RS PC0
#define LCD_EN PC1

void LCD_Command(unsigned char lcd_cmd);
void LCD_Data(unsigned char lcd_data);
void LCD_Init(void);
void LCD_String (unsigned char *lcd_str);
void LCD_YX(unsigned char lcd_y, unsigned char lcd_x);
void LCD_Clear(void);
