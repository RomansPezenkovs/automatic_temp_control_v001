#include "filtering.h"


float kalman_filter(float z_k, int chan)
{
	float result = 0;
	
	switch(chan)
	{
		case 1: // channel 1
		P_k2_1 = P_k1_1 + Q_1;                // Predict next covarience
		K_1 = P_k2_1/(P_k2_1 + R_1);          // Compute Kalman gain
		x_k_1 = x_k_1 + K_1*(z_k-x_k_1);      // filtred value
		P_k1_1 = (1.0-K_1)*P_k2_1;            // Update covarience estimation
		result = x_k_1;
		break;
		
		case 2: // channel 2
		P_k2_2 = P_k1_2 + Q_2;                // Predict next covarience
		K_2 = P_k2_2/(P_k2_2 + R_2);          // Compute Kalman gain
		x_k_2 = x_k_2 + K_2*(z_k-x_k_2);      // filtred value
		P_k1_2 = (1.0-K_2)*P_k2_2;            // Update covarience estimation
		result = x_k_2;
		break;
		
		case 3: // channel 3
		P_k2_3 = P_k1_3 + Q_3;                // Predict next covarience
		K_3 = P_k2_3/(P_k2_3 + R_3);          // Compute Kalman gain
		x_k_3 = x_k_3 + K_3*(z_k-x_k_3);      // filtred value
		P_k1_3 = (1.0-K_3)*P_k2_3;            // Update covarience estimation
		result = x_k_3;
		break;
	}
	
	return result;
}


float moving_aver_filt(float curr_temp, int chan)
{
	float result = 0;
	float temp_sum = 0;
	int i;
	
	switch(chan)
	{
		case 1: // channel 1
		for(i = 0; i < t_numb_1-1; i++)     // shifting temp_arr[] elements to left
		{
			temper_arr_1[i] = temper_arr_1[i+1];
		}
		temper_arr_1[t_numb_1-1] = curr_temp;
		for(i = 0; i < t_numb_1; i++) // summing all elements
		{
			temp_sum += temper_arr_1[i];
		}
		result = temp_sum / t_numb_1; // calculating average Temp
		temper_arr_1[t_numb_1-1] = result;
		break;
		
		case 2: // channel 2
		for(i = 0; i < t_numb_2-1; i++)     // shifting temp_arr[] elements to left
		{
			temper_arr_2[i] = temper_arr_2[i+1];
		}
		temper_arr_2[t_numb_2-1] = curr_temp;
		for(i = 0; i < t_numb_2; i++) // summing all elements
		{
			temp_sum += temper_arr_2[i];
		}
		result = temp_sum / t_numb_2; // calculating average Temp
		temper_arr_2[t_numb_2-1] = result;
		break;
		
		case 3: // channel 3
		for(i = 0; i < t_numb_3-1; i++)     // shifting temp_arr[] elements to left
		{
			temper_arr_3[i] = temper_arr_3[i+1];
		}
		temper_arr_3[t_numb_3-1] = curr_temp;
		for(i = 0; i < t_numb_3; i++) // summing all elements
		{
			temp_sum += temper_arr_3[i];
		}
		result = temp_sum / t_numb_3; // calculating average Temp
		temper_arr_3[t_numb_3-1] = result;
		break;
	}
	
	return result;
}


float median_filter(float curr_temp, int chan)
{
	float a = 0;
	float b = 0;
	float c = 0;
	float median = 0;
	
	switch(chan)
	{
		case 1: // channel 1
		temp_arr_med_1[0] = temp_arr_med_1[1]; // shifting left
		temp_arr_med_1[1] = temp_arr_med_1[2]; // shifting left
		temp_arr_med_1[2] = curr_temp;
		a = temp_arr_med_1[0];
		b = temp_arr_med_1[1];
		c = temp_arr_med_1[2];
		break;
		
		case 2: // channel 2
		temp_arr_med_2[0] = temp_arr_med_2[1]; // shifting left
		temp_arr_med_2[1] = temp_arr_med_2[2]; // shifting left
		temp_arr_med_2[2] = curr_temp;
		a = temp_arr_med_2[0];
		b = temp_arr_med_2[1];
		c = temp_arr_med_2[2];
		break;
		
		case 3: // channel 3
		temp_arr_med_3[0] = temp_arr_med_3[1]; // shifting left
		temp_arr_med_3[1] = temp_arr_med_3[2]; // shifting left
		temp_arr_med_3[2] = curr_temp;
		a = temp_arr_med_3[0];
		b = temp_arr_med_3[1];
		c = temp_arr_med_3[2];
		break;
	}
	
	if(a <= b && a <= c)
	{
		if(b <= c)
		{ median = b;}
		else
		{ median = c;}
	}
	else if(b <= a && b <=c)
	{
		if(a <= c)
		{ median = a;}
		else
		{ median = c;}
	}
	else
	{
		if(a <= b)
		{ median = a;}
		else
		{ median = b;}
	}
	
	return median;
}
