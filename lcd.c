#include "lcd.h"


void sendHalfByte(unsigned char c)
{ 
	// sending half-byte
	e1; // set E input as '1'' 
	_delay_us(50);
	PORTD &= 0b11110000; // erasing old output information
	PORTD |= c; //saving half of 'c' byte
	e0; //set E input as '0''
	_delay_us(50);
}


void sendByte(unsigned char c, unsigned char mode)
{
	// sending full byte by splitting it in two bytes
	if(mode == 0)// sending command
	{rs0;}// setting RS as '0'
	else// sending data
	{rs1;}// setting RS as '1' 
	unsigned char hc = 0;// for half byte
	hc = c >> 4; // shifting right for four bits
	sendHalfByte(hc); // sending most significant half byte
	sendHalfByte(c); // sending least significant half byte
}


void setPos(unsigned char x, unsigned char y)
{
	char adress;
	// the 7-th bit should always be '1'   |0b1000 0000	
	if(y == 0)
		adress = (0x80 + x)|0b10000000;
	else if(y == 1)
		adress = (0xC0 + x)|0b10000000;
	else if(y == 2)
		adress = (0x94 + x)|0b10000000;
	else
		adress = (0xD4 + x)|0b10000000;
	// sending command with characters adress	
	sendByte(adress , 0);
}


void sendChar(unsigned char c)
{
	sendByte(c , 1);
}

//------------------------------------------------------------------//
void LCD_ini(void)
{
	// delays and constants like in datasheet
	_delay_ms(15); 
	sendHalfByte(0b00000011);
	_delay_ms(4);
	sendHalfByte(0b00000011);
	_delay_us(100);
	sendHalfByte(0b00000011);
	_delay_ms(1);
	sendHalfByte(0b00000010);
	_delay_ms(1);
	
	// 0b00"1"0 setting 4 bits mode    
	// _"1"000  N = 1  using two lines,   0 - sending command
	sendByte(0b00101000, 0);
	
	_delay_ms(1);
	
	// 0b0000"11"00  setting on display image 
	// 0b000011"00" do not showing any cursors
	//  0 - sending command
	sendByte(0b00001100, 0);
	
	_delay_ms(1);
	
	// symbols are displaying from left side to right side
	sendByte(0b00000110, 0);
	
	_delay_ms(1);
}


void str_LCD(char str1[])
{
	// goes through array till character '\0'
	int length;
	for(length = 0; str1[length] != '\0'; length++)
	sendChar(str1[length]);
}

