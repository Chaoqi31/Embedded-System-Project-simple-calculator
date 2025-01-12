#include "lcd.h"
#include "tm4c123gh6pm.h"
#include "PLL.h"

#define LCD_DATA_PORT GPIO_PORTB_DATA_R  // PORTB (DB4-DB7)
#define RS 0x08  // PA3 for RS 
#define RW 0x04  // PA2 for R/W 
#define EN 0x10  // PA4 for EN 

void LCD_SendNibble(unsigned char nibble, unsigned char control) {
    LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (nibble & 0xF0);  
    GPIO_PORTA_DATA_R &= ~(RS | RW); 
    GPIO_PORTA_DATA_R |= (control & (RS | RW));  
    GPIO_PORTA_DATA_R |= EN;  
    Delay_us(1); 
    GPIO_PORTA_DATA_R &= ~EN; 
    Delay_us(37);  
}

void LCD_SendByte(unsigned char byte, unsigned char control) {
    LCD_SendNibble(byte & 0xF0, control);
    LCD_SendNibble((byte << 4) & 0xF0, control);
}

void LCD_Init(void) {
    SYSCTL_RCGCGPIO_R |= 0x03;  
    while ((SYSCTL_PRGPIO_R & 0x03) == 0);  

    GPIO_PORTB_DIR_R = 0xF0;  
    GPIO_PORTB_DEN_R = 0xF0;  

    GPIO_PORTA_DIR_R |= (RS | RW | EN);
    GPIO_PORTA_DEN_R |= (RS | RW | EN);

    Delay_ms(20);  
    LCD_SendNibble(0x30, 0);  
    Delay_ms(5);
    LCD_SendNibble(0x30, 0); 
    Delay_us(100);
    LCD_SendNibble(0x30, 0);  
    LCD_SendNibble(0x20, 0);  

    LCD_SendByte(0x28, 0);  
    LCD_SendByte(0x0C, 0);  
    LCD_SendByte(0x06, 0); 
    LCD_SendByte(0x01, 0); 
    Delay_ms(2); 
}

void LCD_Clear(void) {
    LCD_SendByte(0x01, 0);  
    Delay_ms(2);  
}

void LCD_DisplayChar(char c) {
    LCD_SendByte(c, RS); 
}

void LCD_DisplayString(char* str) {
    while (*str) {
        LCD_DisplayChar(*str++);  
    }
}

void LCD_MoveCursor(unsigned char row, unsigned char col) {
    unsigned char address;
    if (row == 0) {
        address = col;
    } else if (row == 1) {
        address = 0x40 + col;
    } else if (row == 2) {
        address = 0x14 + col;
    } else {
        address = 0x54 + col;
    }
    LCD_SendByte(0x80 | address, 0); 
}

