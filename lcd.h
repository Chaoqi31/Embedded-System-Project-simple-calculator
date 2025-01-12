#ifndef LCD_H
#define LCD_H

void LCD_Init(void);

void LCD_SendByte(unsigned char byte, unsigned char control);

void LCD_SendNibble(unsigned char nibble, unsigned char control);

void LCD_Clear(void);

void LCD_DisplayChar(char c);

void LCD_DisplayString(char* str);

void LCD_MoveCursor(unsigned char row, unsigned char col);

#endif

