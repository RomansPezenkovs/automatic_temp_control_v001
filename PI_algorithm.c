#include "PI_algorithm.h"


void calc_PI_pwm(int chan)
{
	switch(chan)
	{
		case 1: // channel 1
		ek_1 = temp_1 - set_temp_1; // the delta between set_Temp_1 and temp_1
		integr_val_1 += ek_1 * integr_step; // calculating integral value
		if(integr_val_1*Tintegr_1 > 255) // integral should be less than 256
		{
			integr_val_1 = 255 / Tintegr_1;
		}
		pwm_val_1 = C_1*ek_1 + Tintegr_1*integr_val_1;
		if(pwm_val_1 > 255)
		{
			pwm_val_1 = 255;
		}
		if(pwm_val_1 < 0)
		{
			pwm_val_1 = 0;
			integr_val_1 = 0;
		}
		OCR0A = pwm_val_1;
		break;
		
		case 2: // channel 2
		ek_2 = temp_2 - set_temp_2; // the delta between set_Temp_2 and temp_2
		integr_val_2 += ek_2 * integr_step; // calculating integral value
		if(integr_val_2*Tintegr_2 > 255) // integral should be less than 256
		{
			integr_val_2 = 255 / Tintegr_2;
		}
		pwm_val_2 = C_2*ek_2 + Tintegr_2*integr_val_2;
		if(pwm_val_2 > 255)
		{
			pwm_val_2 = 255;
		}
		if(pwm_val_2 < 0)
		{
			pwm_val_2 = 0;
			integr_val_2 = 0;
		}
		OCR0B = pwm_val_2;
		break;
		
		case 3: // channel 3
		ek_3 = temp_3 - set_temp_3; // the delta between set_Temp_3 and temp_3
		integr_val_3 += ek_3 * integr_step; // calculating integral value
		if(integr_val_3*Tintegr_3 > 255) // value of integral component should less than 256
		{
			integr_val_3 = 255 / Tintegr_3;
		}
		pwm_val_3 = C_3*ek_3 + Tintegr_3*integr_val_3;
		if(pwm_val_3 > 255)
		{
			pwm_val_3 = 255;
		}
		if(pwm_val_3 < 0)
		{
			pwm_val_3 = 0;
			integr_val_3 = 0;
		}
		OCR2A = pwm_val_3;
		break;
	}
}


void set_PI_coef_step(void)
{
	switch(coef_step_char)
	{
		case 1:
		coef_step = 1;
		break;
		
		case 2:
		coef_step = 2;
		break;
		
		case 3:
		coef_step = 5;
		break;
		
		case 4:
		coef_step = 10;
		break;
		
		case 5:
		coef_step = 20;
		break;
	}
}



