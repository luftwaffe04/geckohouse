#include "avr_lcd.h"

void LCD_Command(unsigned char lcd_cmd)
{
	LCD_PORT = (LCD_PORT & 0x0F) | (lcd_cmd & 0xF0);
	LCD_PORT &= ~ (1<<LCD_RS);
	LCD_PORT |= (1<<LCD_EN);
	_delay_us(10);
	LCD_PORT &= ~ (1<<LCD_EN);
	_delay_us(300);
	LCD_PORT = (LCD_PORT & 0x0F) | (lcd_cmd << 4);
	LCD_PORT |= (1<<LCD_EN);
	_delay_us(10);
	LCD_PORT &= ~ (1<<LCD_EN);
	_delay_ms(10);
}

void LCD_Data( unsigned char lcd_data )
{
	LCD_PORT = (LCD_PORT & 0x0F) | (lcd_data & 0xF0);
	LCD_PORT |= (1<<LCD_RS);
	LCD_PORT|= (1<<LCD_EN);
	_delay_us(10);
	LCD_PORT &= ~ (1<<LCD_EN);
	_delay_us(300);
	LCD_PORT = (LCD_PORT & 0x0F) | (lcd_data << 4);
	LCD_PORT |= (1<<LCD_EN);
	_delay_us(10);
	LCD_PORT &= ~ (1<<LCD_EN);
	_delay_ms(10);
}

void LCD_Init (void)
{
	LCD_DDR = 0xFF;
	_delay_ms(20);

	LCD_Command(0x32);
	LCD_Command(0x02);
	LCD_Command(0x28);
	LCD_Command(0x0c);
	LCD_Command(0x06);
	LCD_Command(0x01);
}

void LCD_String (unsigned char *lcd_str)
{
	for(int i = 0; lcd_str[i] != 0; i++)
	LCD_Data(lcd_str[i]);
}

void LCD_YX(unsigned char lcd_y, unsigned char lcd_x)
{
	LCD_Command(0x80 | (lcd_x+lcd_y * 0x40));
}

void LCD_Clear()
{
	LCD_Command(0x01);
}
