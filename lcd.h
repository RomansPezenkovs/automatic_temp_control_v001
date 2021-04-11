#ifndef LCD_H_
#define LCD_H_

#include "main.h"

//Display
#define e1 PORTC |= 0b00100000; // set display E input as '1'
#define e0 PORTC &= 0b11011111;// set display E input as '0'
#define rs1 PORTC |= 0b00010000; // set display RS input as '1'
#define rs0 PORTC &= 0b11101111;// set display RS input as '0'
// display E input is connected to PC5
// display RS input is connected to PC4
// half-byte is from PD0 to PD3

// sending half of byte
void sendHalfByte(unsigned char c);

// sending full byte by splitting it in half-bytes
void sendByte(unsigned char c, unsigned char mode);

// setting position for data
void setPos(unsigned char x, unsigned char y);

// sending one character
void sendChar(unsigned char c);

// initializing display
void LCD_ini(void);

// sending string
void str_LCD(char str1[]);


#endif