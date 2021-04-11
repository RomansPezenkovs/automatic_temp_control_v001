#include "main.h"


void lcd_port_ini(void); // initialize ports for LCD 20x4
void pcint_ini(void); // initialize pin change interrupts for buttons
void pwm_ini(void); // initialize registers for PWM: OC0A, OC0B, OC2A
void timer_ini(void); // initialize registers for timers
void adc_ini(void); // initialize registers for ADC
unsigned int adc_conv(int adc_chan); // read value of ADC channel
void write_eeprom_data(void); // write values of variable to eeprom
void read_eeprom_data(void); // read values of variables from eeprom
float calc_temp(int chan); // convert ADC value to temperature
void set_temp_step(void); // set temperature step corresponding to temp_step_char


int main (void)
{
	char_arrow = 0b01111111;
	char_accept = 0b11111111;
	char_deg = 0b11011111;
	char_empty = ' ';
	char_selected = '>';

	disp_val = 1; // variable to switch interface
	prev_disp_val = 1; // previous value of interface

	set_temp_1 = 20.0; // value of set temperature for channel 1
	set_temp_2 = 20.0; // value of set temperature for channel 2
	set_temp_3 = 20.0; // value of set temperature for channel 3

	ch_1_filter = 2; // selected filtering method for channel 1
	ch_2_filter = 2; // selected filtering method for channel 2
	ch_3_filter = 2; // selected filtering method for channel 3

	temp_step = 0.1; // variable to change value of set temperature
	coef_step = 1; // variable to change value of PI coefficients

	temp_step_char = 1; // variable to change value of temp_step (to do not float == float)
	coef_step_char = 1; // variable to change value of coef_step (to do not float == float)

	// for Kalman filtering -------channel 1------------//
	R_1 = 0.021;  // covariance matrix cov(A:A,A:A)=variance(A:A)
	Q_1 = 0.002; // process noise covariance matrix, speed of reaction (chose manually)
	P_k1_1 = 1.0;          // Updated covarience estimation
	P_k2_1 = 0.0;           // Predicted next covarience
	K_1 = 0.0;           // Kalman Gain
	x_k_1 = 0.0;          // Kalman estimate of the sensor's temperature
	// ------------------------------------------------//

	// for Kalman filtering -------channel 2------------//
	R_2 = 0.021;  // covariance matrix cov(A:A,A:A)=variance(A:A)
	Q_2 = 0.002; // process noise covariance matrix, speed of reaction (chose manually)
	P_k1_2 = 1.0;          // Updated covarience estimation
	P_k2_2 = 0.0;           // Predicted next covarience
	K_2 = 0.0;           // Kalman Gain
	x_k_2 = 0.0;          // Kalman estimate of the sensor's temperature
	// ------------------------------------------------//

	// for Kalman filtering -------channel 3------------//
	R_3 = 0.021;  // covariance matrix cov(A:A,A:A)=variance(A:A)
	Q_3 = 0.002; // process noise covariance matrix, speed of reaction (chose manually)
	P_k1_3 = 1.0;          // Updated covarience estimation
	P_k2_3 = 0.0;           // Predicted next covarience
	K_3 = 0.0;           // Kalman Gain
	x_k_3 = 0.0;          // kalman estimate of the sensor's temperature
	// ------------------------------------------------//

	integr_step = 0.005; // step for integral calculation 5ms

	//---- For PID ----channel 1------//
	pwm_val_1 = 0; // calculated value of PWM duty cycle for channel 1
	ek_1 = 0;           // the delta between set_temp_1 and temp_1
	integr_val_1 = 0;   // the value of calculated integral
	C_1 = 5;            // Proportional component
	Tintegr_1 = 80;     // Integral component
	//-----------------------------//

	//---- For PID ----channel 2------//
	pwm_val_2 = 0; // calculated value of PWM duty cycle for channel 2
	ek_2 = 0;           // the delta between set_temp_2 and temp_2
	integr_val_2 = 0;   // the value of calculated integral
	C_2 = 5;            // Proportional component
	Tintegr_2 = 80;     // Integral component
	//-----------------------------//

	//---- For PID ----channel 3------//
	pwm_val_3 = 0; // calculated value of PWM duty cycle for channel 3
	ek_3 = 0;           // the delta between set_temp_3 and temp_3
	integr_val_3 = 0;   // the value of calculated integral
	C_3 = 5;            // Proportional component
	Tintegr_3 = 80;     // Integral component
	//-----------------------------//
	
	//  interrupts are forbidden to forbid PCINT in first 100ms
	cli();
	_delay_ms(100);
	
	lcd_port_ini(); // initialize outputs for lcd display
	LCD_ini(); // initialize LCD
	pcint_ini(); // initialize Pin Change interrupts for buttons
	adc_ini(); // initialize registers for ADC
	pwm_ini(); // initialize registers for PWM
	timer_ini();// initialize Timer1;
	
	//write_eeprom_data(); // use only when first time program MCU chip
	read_eeprom_data();
	
	set_temp_step();
	set_PI_coef_step();
	
	temp_1 = calc_temp(1);
	temp_2 = calc_temp(2);
	temp_3 = calc_temp(3);
	
	x_k_1 = temp_1; // Kalman estimate for channel 1
	x_k_2 = temp_2; // Kalman estimate for channel 2
	x_k_3 = temp_3; // Kalman estimate for channel 3
	
	int i;
	
	for(i = 0; i < t_numb_1; i++)
	{
		temper_arr_1[i] = temp_1; // for Moving average filtering channel 1
	}
	
	for(i = 0; i < t_numb_2; i++)
	{
		temper_arr_2[i] = temp_2; // for Moving average filtering channel 2
	}
	
	for(i = 0; i < t_numb_3; i++)
	{
		temper_arr_3[i] = temp_3; // for Moving average filtering channel 3
	}
	
	temp_arr_med_1[1] = temp_1; // for Median filtering channel 1
	temp_arr_med_1[2] = temp_1; // for Median filtering channel 1
	
	temp_arr_med_2[1] = temp_2; // for Median filtering channel 2
	temp_arr_med_2[2] = temp_2; // for Median filtering channel 2
	
	temp_arr_med_3[1] = temp_3; // for Median filtering channel 3
	temp_arr_med_3[2] = temp_3; // for Median filtering channel 3
	
	print_menu(1);
	
	setPos(4, 1);
	sendChar(char_arrow);
	
	sei(); // interrupts are enabled
	
	while(1)
	{	
		temp_1 = calc_temp(1);
		switch(ch_1_filter)
		{
			case 1:
			temp_1 = kalman_filter(temp_1, 1);
			break;
			
			case 2:
			temp_1 = moving_aver_filt(temp_1, 1);
			break;
			
			case 3:
			temp_1 = median_filter(temp_1, 1);
			break;
		}
		calc_PI_pwm(1);
		
		temp_2 = calc_temp(2);
		switch(ch_2_filter)
		{
			case 1:
			temp_2 = kalman_filter(temp_2, 2);
			break;
			
			case 2:
			temp_2 = moving_aver_filt(temp_2, 2);
			break;
			
			case 3:
			temp_2 = median_filter(temp_2, 2);
			break;
		}
		calc_PI_pwm(2);
		
		temp_3 = calc_temp(3);
		switch(ch_3_filter)
		{
			case 1:
			temp_3 = kalman_filter(temp_3, 3);
			break;
			
			case 2:
			temp_3 = moving_aver_filt(temp_3, 3);
			break;
			
			case 3:
			temp_3 = median_filter(temp_3, 3);
			break;
		}
		calc_PI_pwm(3);
		
	}
}


void lcd_port_ini(void) // PORTC and PORTD initialization
{
	PORTC &= 0b11001111; // e and rs
	DDRC |= 0b00110000; // e and rs
	PORTD &= 0b11110000; // DB4-DB7
	DDRD |= 0b00001111; // DB4-DB7
}


void pcint_ini(void)
{
	DDRB &= 0b11101001; // PB1, PB2, PB4 inputs
	
	// pull up resistors for PB1, PB2, PB4 inputs
	PORTB |= (1 << PORTB4) | (1 << PORTB2) | (1 << PORTB1);
	
	// inputs as Pin Change interrupts (this to recieve variable from reciever-MCU)
	PCMSK0 |= (1 << PCINT4) | (1 << PCINT2) | (1 << PCINT1);
	
	// Pin Change interrupts enabled PCINT1, PCINT2, PCINT4
	PCICR |= (1 << PCIE0);
}


void pwm_ini(void)
{	
	DDRD |= (1 << PORTD6) | (1 << PORTD5); // PD6 PWM OC0A output, PD5 PWM OC0B output
	DDRB |= (1 << PORTB3); // PB3 PWM OC2A output
	TCCR0A |= (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00); // Timer settings for OC0A and OC0B, using Fast PWM 8 bit
	TCCR0B |= (1 << CS00); // prescaler 1 for 0. timer
	TCCR2A |= (1 << COM2A1) | (1 << WGM21) | (1 << WGM20); // Timer settings for OC2A, using Fast PWM 8 bit
	TCCR2B |= (1 << CS20); // prescaler 1 for 2. timer
}


void timer_ini(void)
{
	TCCR1A |= (1 << WGM12); // CTC mode
	TIMSK1 |= (1 << OCIE1A); // Output compare A match interrupt enabled
	OCR1AH = 0x1E; //  0.5s when 16MHZ
	OCR1AL = 0x84; //  0.5s when 16MHZ
	TCCR1B |= (1 << CS12) | (1 << CS10); // prescaler 1024 for 1. timer	
}


void adc_ini(void)
{
	DDRC &= 0b11110001; //  ADC3; ADC2; ADC1	
	ADCSRA |= (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1); // ADEN = 1; allows to use ADC, 
	ADMUX &= 0b00111111; // AREF, Internal Vref turned off (0<<REFS1) | (0<<REFS0)
	SMCR &= 0b11110011; // ADC noise reduction 
	SMCR |= 0b00000010; // ADC noise reduction 
}


unsigned int adc_conv(int adc_chan)
{
	ADMUX &= 0b11110000;
	ADMUX |= adc_chan; // select channel
	ADCSRA |= (1<<ADSC);  // start conversion
	while(ADCSRA & (1<<ADSC)); // for delay, to ADC value calculation
	return ADC;
}


void write_eeprom_data(void)
{
	eeprom_write_float(0x00, set_temp_1);
	eeprom_write_float(0x04, set_temp_2);
	eeprom_write_float(0x08, set_temp_3);
	
	eeprom_write_byte(0x0C, ch_1_filter);
	eeprom_write_byte(0x0D, ch_2_filter);
	eeprom_write_byte(0X0E, ch_3_filter);
	
	eeprom_write_byte(0x0F, temp_step_char);
	eeprom_write_byte(0x10, coef_step_char);
	
	eeprom_write_float(0x11, C_1);
	eeprom_write_float(0x15, C_2);
	eeprom_write_float(0x19, C_3);
	
	eeprom_write_float(0x1D, Tintegr_1);
	eeprom_write_float(0x21, Tintegr_2);
	eeprom_write_float(0x25, Tintegr_3);
}


void read_eeprom_data(void)
{
	set_temp_1 = eeprom_read_float(0x00);
	if(set_temp_1 < 0 || set_temp_1 > 99.9)
	{
		set_temp_1 = 20;
	}
	
	set_temp_2 = eeprom_read_float(0x04);
	if(set_temp_2 < 0 || set_temp_2 > 99.9)
	{
		set_temp_2 = 20;
	}
	
	set_temp_3 = eeprom_read_float(0x08);
	if(set_temp_3 < 0 || set_temp_3 > 99.9)
	{
		set_temp_3 = 20;
	}
	
	ch_1_filter = eeprom_read_byte(0x0C);
	if(ch_1_filter < 1 || ch_1_filter > 3)
	{
		ch_1_filter = 2;
	}
	
	ch_2_filter = eeprom_read_byte(0x0D);
	if(ch_2_filter < 1 || ch_2_filter > 3)
	{
		ch_2_filter = 2;
	}
	
	ch_3_filter = eeprom_read_byte(0x0E);
	if(ch_3_filter < 1 || ch_3_filter > 3)
	{
	
	ch_3_filter = 2;
	}
	
	temp_step_char = eeprom_read_byte(0x0F);
	if(temp_step_char < 1 || temp_step_char > 5)
	{
		temp_step_char = 1;
	}
	
	coef_step_char = eeprom_read_byte(0x10);
	if(coef_step_char < 1 || coef_step_char > 5)
	{
		coef_step_char = 1;
	}
	
	C_1 = eeprom_read_float(0x11);
	if(C_1 < 0 || C_1 > 500)
	{
		C_1 = 5;
	}
	
	C_2 = eeprom_read_float(0x15);
	if(C_2 < 0 || C_2 > 500)
	{
		C_2 = 5;
	}
	
	C_3 = eeprom_read_float(0x19);
	if(C_3 < 0 || C_3 > 500)
	{
		C_3 = 5;
	}
	
	Tintegr_1 = eeprom_read_float(0x1D);
	if(Tintegr_1 < 0 || Tintegr_1 > 500)
	{
		Tintegr_1 = 80;
	}
	
	Tintegr_2 = eeprom_read_float(0x21);
	if(Tintegr_2 < 0 || Tintegr_2 > 500)
	{
		Tintegr_2 = 80;
	}
	
	Tintegr_3 = eeprom_read_float(0x25);
	if(Tintegr_3 < 0 || Tintegr_3 > 500)
	{
		Tintegr_3 = 80;
	}
	
}


float calc_temp(int chan)
{
	return -1481.96 + sqrt(2196200 + (1.839 - adc_conv(chan) / 1024.0 * 4.94)/0.00000388 );
}


void set_temp_step(void)
{
	switch(temp_step_char)
	{
		case 1:
		temp_step = 0.1;
		break;
		
		case 2:
		temp_step = 0.5;
		break;
		
		case 3:
		temp_step = 1;
		break;
		
		case 4:
		temp_step = 5;
		break;
		
		case 5:
		temp_step = 10;
		break;
	}
}


ISR(TIMER1_COMPA_vect)
{
	switch(disp_val)
	{
		case 1:
		case 2:
		case 3:
		setPos(0, 3);
		print_temp(temp_1);
		setPos(7, 3);
		print_temp(temp_2);
		setPos(14, 3);
		print_temp(temp_3);
		break;
		
		case 4:
		case 5:
		case 6:
		setPos(2, 3);
		print_3_dig_int(pwm_val_1);
		setPos(8, 3);
		print_3_dig_int(pwm_val_2);
		setPos(14, 3);
		print_3_dig_int(pwm_val_3);
		break;
		
		case 15:
		case 16:
		case 17:
		setPos(0, 3);
		print_temp(temp_1);
		setPos(7, 3);
		print_temp(temp_2);
		setPos(14, 3);
		print_temp(temp_3);
		break;
		
		case 18:
		case 19:
		case 20:
		setPos(2, 3);
		print_3_dig_int(pwm_val_1);
		setPos(8, 3);
		print_3_dig_int(pwm_val_2);
		setPos(14, 3);
		print_3_dig_int(pwm_val_3);
		break;
	}
	
	TCNT1H = 0x00; // clear timer1 after calculations
	TCNT1L = 0x00; // clear timer1 after calculations
}


ISR(PCINT0_vect)
{	
	_delay_ms(50);
	
	if(!(PINB & 0x02)) // right PB1
	{
		if(disp_val == 14)
		{
			prev_disp_val = disp_val;
			disp_val = 1;
		}
		else if(disp_val < 14)
		{
			prev_disp_val = disp_val;
			disp_val += 1;
		}
		
		switch(disp_val)
		{
			case 15:
			set_temp_1 += temp_step;
			if(set_temp_1 > 99.9)
			{
				set_temp_1 = 99.9;
			}
			setPos(0, 1);
			print_temp(set_temp_1);
			eeprom_write_float(0x00, set_temp_1);
			break;
			
			case 16:
			set_temp_2 += temp_step;
			if(set_temp_2 > 99.9)
			{
				set_temp_2 = 99.9;
			}
			setPos(7, 1);
			print_temp(set_temp_2);
			eeprom_write_float(0x04, set_temp_2);
			break;
			
			case 17:
			set_temp_3 += temp_step;
			if(set_temp_3 > 99.9)
			{
				set_temp_3 = 99.9;
			}
			setPos(14, 1);
			print_temp(set_temp_3);
			eeprom_write_float(0x08, set_temp_3);
			break;
			
			case 18:
			ch_1_filter += 1;
			if(ch_1_filter > 3)
			{
				ch_1_filter = 3;
			}
			setPos(0, 1);
			print_filt(ch_1_filter);
			eeprom_write_byte(0x0C, ch_1_filter);
			break;
			
			case 19:
			ch_2_filter += 1;
			if(ch_2_filter > 3)
			{
				ch_2_filter = 3;
			}
			setPos(7, 1);
			print_filt(ch_2_filter);
			eeprom_write_byte(0x0D, ch_2_filter);
			break;
			
			case 20:
			ch_3_filter += 1;
			if(ch_3_filter > 3)
			{
				ch_3_filter = 3;
			}
			setPos(14, 1);
			print_filt(ch_3_filter);
			eeprom_write_byte(0X0E, ch_3_filter);
			break;
			
			case 21:
			C_1 += coef_step;
			if(C_1 > 500)
			{
				C_1 = 500;
			}
			setPos(2, 1);
			print_3_dig_int((int) C_1);
			eeprom_write_float(0x11, C_1);
			break;
			
			case 22:
			C_2 += coef_step;
			if(C_2 > 500)
			{
				C_2 = 500;
			}
			setPos(8, 1);
			print_3_dig_int((int) C_2);
			eeprom_write_float(0x15, C_2);
			break;
			
			case 23:
			C_3 += coef_step;
			if(C_3 > 500)
			{
				C_3 = 500;
			}
			setPos(14, 1);
			print_3_dig_int((int) C_3);
			eeprom_write_float(0x19, C_3);
			break;
			
			case 24:
			Tintegr_1 += coef_step;
			if(Tintegr_1 > 500)
			{
				Tintegr_1 = 500;
			}
			setPos(2, 3);
			print_3_dig_int((int) Tintegr_1);
			eeprom_write_float(0x1D, Tintegr_1);
			break;
			
			case 25:
			Tintegr_2 += coef_step;
			if(Tintegr_2 > 500)
			{
				Tintegr_2 = 500;
			}
			setPos(8, 3);
			print_3_dig_int((int) Tintegr_2);
			eeprom_write_float(0x21, Tintegr_2);
			break;
			
			case 26:
			Tintegr_3 += coef_step;
			if(Tintegr_3 > 500)
			{
				Tintegr_3 = 500;
			}
			setPos(14, 3);
			print_3_dig_int((int) Tintegr_3);
			eeprom_write_float(0x25, Tintegr_3);
			break;
			
			case 27:
			temp_step_char += 1;
			if(temp_step_char > 5)
			{
				temp_step_char = 5;
			}
			print_char_arrow_temp();
			break;
			
			case 28:
			coef_step_char += 1;
			if(coef_step_char > 5)
			{
				coef_step_char = 5;
			}
			print_char_arrow_PI();
			break;
		}
	}
	else if(!(PINB & 0x04))	// accept PB2
	{
		if(disp_val <= 14)
		{
			switch(disp_val)
			{
				case 1:
				disp_val = 15;
				setPos(4, 1);
				sendChar(char_accept);
				break;
			
				case 2:
				disp_val = 16;
				setPos(11, 1);
				sendChar(char_accept);
				break;
				
				case 3:
				disp_val = 17;
				setPos(18, 1);
				sendChar(char_accept);
				break;
				
				case 4:
				disp_val = 18;
				setPos(5, 1);
				sendChar(char_accept);
				break;
				
				case 5:
				disp_val = 19;
				setPos(12, 1);
				sendChar(char_accept);
				break;
				
				case 6:
				disp_val = 20;
				setPos(19, 1);
				sendChar(char_accept);
				break;
				
				case 7:
				disp_val = 21;
				setPos(5, 1);
				sendChar(char_accept);
				break;
				
				case 8:
				disp_val = 22;
				setPos(11, 1);
				sendChar(char_accept);
				break;
				
				case 9:
				disp_val = 23;
				setPos(17, 1);
				sendChar(char_accept);
				break;
				
				case 10:
				disp_val = 24;
				setPos(5, 3);
				sendChar(char_accept);
				break;
				
				case 11:
				disp_val = 25;
				setPos(11, 3);
				sendChar(char_accept);
				break;
				
				case 12:
				disp_val = 26;
				setPos(17, 3);
				sendChar(char_accept);
				break;
				
				case 13:
				disp_val = 27;
				setPos(19, 0);
				sendChar(char_accept);
				print_char_arrow_temp();
				break;
				
				case 14:
				disp_val = 28;
				setPos(19, 2);
				sendChar(char_accept);
				break;
			}
		}
		else if(disp_val > 14 && disp_val < 27)
		{
			disp_val -= 14;
		}
		else if(disp_val == 27)
		{
			disp_val -= 14;
			clear_char_arrow_temp();
			set_temp_step();
			print_char_sel_temp();
			eeprom_write_byte(0x0F, temp_step_char);
		}
		else if(disp_val == 28)
		{
			disp_val -= 14;
			clear_char_arrow_PI();
			set_PI_coef_step();
			print_char_sel_PI();
			eeprom_write_byte(0x10, coef_step_char);
		}
		
	}
	else if(!(PINB & 0x10)) // left PB4
	{
		if(disp_val == 1)
		{
			prev_disp_val = disp_val;
			disp_val = 14;
		}
		else if(disp_val > 1 && disp_val <= 14)
		{
			prev_disp_val = disp_val;
			disp_val -= 1;
		}
		
		switch(disp_val)
		{
			case 15:
			set_temp_1 -= temp_step;
			if(set_temp_1 < 0)
			{
				set_temp_1 = 0;
			}
			setPos(0, 1);
			print_temp(set_temp_1);
			eeprom_write_float(0x00, set_temp_1);
			break;
			
			case 16:
			set_temp_2 -= temp_step;
			if(set_temp_2 < 0)
			{
				set_temp_2 = 0;
			}
			setPos(7, 1);
			print_temp(set_temp_2);
			eeprom_write_float(0x04, set_temp_2);
			break;
			
			case 17:
			set_temp_3 -= temp_step;
			if(set_temp_3 < 0)
			{
				set_temp_3 = 0;
			}
			setPos(14, 1);
			print_temp(set_temp_3);
			eeprom_write_float(0x08, set_temp_3);
			break;
			
			case 18:
			ch_1_filter -= 1;
			if(ch_1_filter < 1)
			{
				ch_1_filter = 1;
			}
			setPos(0, 1);
			print_filt(ch_1_filter);
			eeprom_write_byte(0x0C, ch_1_filter);
			break;
			
			case 19:
			ch_2_filter -= 1;
			if(ch_2_filter < 1)
			{
				ch_2_filter = 1;
			}
			setPos(7, 1);
			print_filt(ch_2_filter);
			eeprom_write_byte(0x0D, ch_2_filter);
			break;
		
			case 20:
			ch_3_filter -= 1;
			if(ch_3_filter < 1)
			{
				ch_3_filter = 1;
			}
			setPos(14, 1);
			print_filt(ch_3_filter);
			eeprom_write_byte(0X0E, ch_3_filter);
			break;
			
			case 21:
			C_1 -= coef_step;
			if(C_1 < 0)
			{
				C_1 = 0;
			}
			setPos(2, 1);
			print_3_dig_int((int) C_1);
			eeprom_write_float(0x11, C_1);
			break;
			
			case 22:
			C_2 -= coef_step;
			if(C_2 < 0)
			{
				C_2 = 0;
			}
			setPos(8, 1);
			print_3_dig_int((int) C_2);
			eeprom_write_float(0x15, C_2);
			break;
			
			case 23:
			C_3 -= coef_step;
			if(C_3 < 0)
			{
				C_3 = 0;
			}
			setPos(14, 1);
			print_3_dig_int((int) C_3);
			eeprom_write_float(0x19, C_3);
			break;
			
			case 24:
			Tintegr_1 -= coef_step;
			if(Tintegr_1 < 0)
			{
				Tintegr_1 = 0;
			}
			setPos(2, 3);
			print_3_dig_int((int) Tintegr_1);
			eeprom_write_float(0x1D, Tintegr_1);
			break;
			
			case 25:
			Tintegr_2 -= coef_step;
			if(Tintegr_2 < 0)
			{
				Tintegr_2 = 0;
			}
			setPos(8, 3);
			print_3_dig_int((int) Tintegr_2);
			eeprom_write_float(0x21, Tintegr_2);
			break;
			
			case 26:
			Tintegr_3 -= coef_step;
			if(Tintegr_3 < 0)
			{
				Tintegr_3 = 0;
			}
			setPos(14, 3);
			print_3_dig_int((int) Tintegr_3);
			eeprom_write_float(0x25, Tintegr_3);
			break;
			
			case 27:
			temp_step_char -= 1;
			if(temp_step_char < 1)
			{
				temp_step_char = 1;
			}
			print_char_arrow_temp();
			break;
			
			case 28:
			coef_step_char -= 1;
			if(coef_step_char < 1)
			{
				coef_step_char = 1;
			}
			print_char_arrow_PI();
			break;
		}
	}
	
	print_interface();	
	
	_delay_ms(75);
}