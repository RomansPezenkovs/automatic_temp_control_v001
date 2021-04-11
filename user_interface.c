#include "user_interface.h"


void print_temp(float temp)
{
	sendChar( ((int)temp)/10 + '0');
	sendChar( ((int)temp)%10 + '0');
	sendChar('.');
	sendChar( ((int)(temp*10)) - ((int)temp)*10 +'0');
}


void print_3_dig_int(int number)
{
	int digit_2 = 0; // digit*10^2
	int digit_1 = 0; // digit*10^1
	int digit_0 = 0; // digit*10^0
	
	digit_2 = number / 100;
	number = number % 100;
	
	if(digit_2 != 0)
	{
		sendChar( digit_2 + '0');
	}
	else
	{
		sendChar(char_empty);
	}
	
	digit_1 = number / 10;
	
	if(digit_1 != 0)
	{
		sendChar(digit_1 + '0');
	}
	else if(digit_1 == 0 && digit_2 != 0)
	{
		sendChar(digit_1 + '0');
	}
	else
	{
		sendChar(char_empty);
	}
	
	digit_0 = number % 10;
	sendChar(digit_0 + '0');
}


void print_menu(int number)
{
	switch(number)
	{
		case 1:
		setPos(0, 0);
		str_LCD(" set Temperature    ");
		setPos(17, 0);
		sendChar(char_deg);
		sendChar('C');
		
		setPos(0, 1);
		print_temp(set_temp_1);
		str_LCD("   ");
		print_temp(set_temp_2);
		str_LCD("   ");
		print_temp(set_temp_3);
		str_LCD("  ");
		
		setPos(0, 2);
		str_LCD("curr. Temperature ");
		sendChar(char_deg);
		sendChar('C');
		
		setPos(0, 3);
		print_temp(temp_1);
		sendChar(char_deg);
		str_LCD("C ");
		print_temp(temp_2);
		sendChar(char_deg);
		str_LCD("C ");
		print_temp(temp_3);
		sendChar(char_deg);
		sendChar('C');
		break;
		
		
		case 2:
		setPos(0, 0);
		str_LCD("  Filtering method  ");
		
		setPos(0, 1);
		print_filt(ch_1_filter);
		str_LCD("  ");
		print_filt(ch_2_filter);
		str_LCD("  ");
		print_filt(ch_3_filter);
		sendChar(char_empty);
		
		setPos(0, 2);
		str_LCD(" Pulse width modul. ");
		
		setPos(0, 3);
		str_LCD("  ");
		print_3_dig_int(pwm_val_1);
		str_LCD("   ");
		print_3_dig_int(pwm_val_2);
		str_LCD("   ");
		print_3_dig_int(pwm_val_3);
		str_LCD("   ");
		break;
		
		
		case 3:
		setPos(0, 0);
		str_LCD(" Proportional coef. ");
		
		setPos(0, 1);
		str_LCD("  ");
		print_3_dig_int((int) C_1);
		str_LCD("   ");
		print_3_dig_int((int) C_2);
		str_LCD("   ");
		print_3_dig_int((int) C_3);
		str_LCD("   ");
		
		setPos(0, 2);
		str_LCD(" Integral coeffic.  ");
		
		setPos(0, 3);
		str_LCD("  ");
		print_3_dig_int((int) Tintegr_1);
		str_LCD("   ");
		print_3_dig_int((int) Tintegr_2);
		str_LCD("   ");
		print_3_dig_int((int) Tintegr_3);
		str_LCD("   ");
		break;

		
		case 4:
		setPos(0, 0);
		str_LCD(" Step for temper. C ");
		setPos(17, 0);
		sendChar(char_deg);
		
		setPos(0, 1);
		str_LCD(" 0.1  0.5  1  5  10 ");
		print_char_sel_temp();
		
		
		setPos(0, 2);
		str_LCD(" Step for coeffic.  ");
		
		setPos(0, 3);
		str_LCD("   1  2  5  10  20  ");
		print_char_sel_PI();
		break;
	}
}


void print_filt(char method)
{
	switch(method)
	{
		case 1:
		str_LCD("Kalm.");
		break;
		
		case 2:
		str_LCD("Aver.");
		break;
		
		case 3:
		str_LCD("Medi.");
		break;
	}
}


void print_char_sel_temp(void)
{
	setPos(0, 1);
	sendChar(char_empty);
	
	setPos(5, 1);
	sendChar(char_empty);
	
	setPos(10, 1);
	sendChar(char_empty);
	
	setPos(13, 1);
	sendChar(char_empty);

	setPos(16, 1);
	sendChar(char_empty);
	
	switch(temp_step_char)
	{
		case 1:
		setPos(0, 1);
		break;
		
		case 2:
		setPos(5, 1);
		break;
		
		case 3:
		setPos(10, 1);
		break;
		
		case 4:
		setPos(13, 1);
		break;
		
		case 5:
		setPos(16, 1);
		break;
	}
	
	sendChar(char_selected);
}


void print_char_sel_PI(void)
{
	setPos(2, 3);
	sendChar(char_empty);
	
	setPos(5, 3);
	sendChar(char_empty);
	
	setPos(8, 3);
	sendChar(char_empty);
	
	setPos(11, 3);
	sendChar(char_empty);
	
	setPos(15, 3);
	sendChar(char_empty);
	
	switch(coef_step_char)
	{
		case 1:
		setPos(2, 3);
		break;
		
		case 2:
		setPos(5, 3);
		break;
		
		case 3:
		setPos(8, 3);
		break;
		
		case 4:
		setPos(11, 3);
		break;
		
		case 5:
		setPos(15, 3);
		break;
	}
	
	sendChar(char_selected);
}


void print_char_arrow_temp(void)
{
	clear_char_arrow_temp();
	
	switch(temp_step_char)
	{
		case 1:
		setPos(4, 1);
		break;
		
		case 2:
		setPos(9, 1);
		break;
		
		case 3:
		setPos(12, 1);
		break;
		
		case 4:
		setPos(15, 1);
		break;
		
		case 5:
		setPos(19, 1);
		break;
	}
	
	sendChar(char_arrow);
}


void print_char_arrow_PI(void)
{
	clear_char_arrow_PI();
	
	switch(coef_step_char)
	{
		case 1:
		setPos(4, 3);
		break;
		
		case 2:
		setPos(7, 3);
		break;
		
		case 3:
		setPos(10, 3);
		break;
		
		case 4:
		setPos(14, 3);
		break;
		
		case 5:
		setPos(18, 3);
		break;
	}
	
	sendChar(char_arrow);
}


void clear_char_arrow_temp(void)
{
	setPos(4, 1);
	sendChar(char_empty);
	
	setPos(9, 1);
	sendChar(char_empty);
	
	setPos(12, 1);
	sendChar(char_empty);
	
	setPos(15, 1);
	sendChar(char_empty);

	setPos(19, 1);
	sendChar(char_empty);
}


void clear_char_arrow_PI(void)
{
	setPos(4, 3);
	sendChar(char_empty);
	
	setPos(7, 3);
	sendChar(char_empty);
	
	setPos(10, 3);
	sendChar(char_empty);
	
	setPos(14, 3);
	sendChar(char_empty);
	
	setPos(18, 3);
	sendChar(char_empty);
}


void print_interface(void)
{
	switch(disp_val)
	{
		case 1:
		case 2:
		case 3:
		if(prev_disp_val <= 3)
		{
			setPos(4, 1);
			sendChar(char_empty);
			setPos(11, 1);
			sendChar(char_empty);
			setPos(18, 1);
			sendChar(char_empty);
		}
		else
		{
			print_menu(1);
		}
		switch(disp_val)
		{
			case 1:
			setPos(4, 1);
			sendChar(char_arrow);
			break;
			
			case 2:
			setPos(11, 1);
			sendChar(char_arrow);
			break;
			
			case 3:
			setPos(18, 1);
			sendChar(char_arrow);
			break;
		}
		break;

		
		case 4:
		case 5:
		case 6:
		if( prev_disp_val >= 4 && prev_disp_val <= 6)
		{
			setPos(5, 1);
			sendChar(char_empty);
			setPos(12, 1);
			sendChar(char_empty);
			setPos(19, 1);
			sendChar(char_empty);
		}
		else
		{
			print_menu(2);
		}
		switch(disp_val)
		{
			case 4:
			setPos(5, 1);
			sendChar(char_arrow);
			break;
			
			case 5:
			setPos(12, 1);
			sendChar(char_arrow);
			break;
			
			case 6:
			setPos(19, 1);
			sendChar(char_arrow);
			break;
		}
		break;
		
		
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		if( prev_disp_val >= 7 && prev_disp_val <= 12)
		{
			setPos(5, 1);
			sendChar(char_empty);
			setPos(11, 1);
			sendChar(char_empty);
			setPos(17, 1);
			sendChar(char_empty);
			setPos(5, 3);
			sendChar(char_empty);
			setPos(11, 3);
			sendChar(char_empty);
			setPos(17, 3);
			sendChar(char_empty);
		}
		else
		{
			print_menu(3);
		}
		switch(disp_val)
		{
			case 7:
			setPos(5, 1);
			sendChar(char_arrow);
			break;
			
			case 8:
			setPos(11, 1);
			sendChar(char_arrow);
			break;
			
			case 9:
			setPos(17, 1);
			sendChar(char_arrow);
			break;
			
			case 10:
			setPos(5, 3);
			sendChar(char_arrow);
			break;
			
			case 11:
			setPos(11, 3);
			sendChar(char_arrow);
			break;
			
			case 12:
			setPos(17, 3);
			sendChar(char_arrow);
			break;
		}
		break;
		
		
		case 13:
		case 14:
		if(prev_disp_val >= 13 && prev_disp_val <= 14)
		{
			setPos(19, 0);
			sendChar(char_empty);
			setPos(19, 2);
			sendChar(char_empty);
		}
		else
		{
			print_menu(4);
		}
		if(disp_val == 13)
		{
			setPos(19, 0);
			sendChar(char_arrow);
		}
		else if(disp_val == 14)
		{
			setPos(19, 2);
			sendChar(char_arrow);
		}
		break;
	}
}