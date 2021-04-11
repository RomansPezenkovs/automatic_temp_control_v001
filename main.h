#ifndef MAIN_H_
#define MAIN_H_

#define F_CPU 16000000UL
#include <math.h>
#include <avr/io.h>
#include <inttypes.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.h"
#include "PI_algorithm.h"
#include "filtering.h"
#include "user_interface.h"

//Display
// display E input is connected to PC5
// display RS input is connected to PC4
// half-byte is from PD0 to PD3
unsigned char char_arrow;
unsigned char char_accept;
unsigned char char_deg;
unsigned char char_empty;
unsigned char char_selected;

char disp_val; // variable to switch interface
char prev_disp_val; // previous value of interface

float temp_1; // temperature of channel 1, PC1, controls PWM OC0A
float temp_2; // temperature of channel 2, PC2, controls PWM OC0B
float temp_3; // temperature of channel 3, PC3, controls PWM OC2A

float set_temp_1; // value of set temperature for channel 1
float set_temp_2; // value of set temperature for channel 2
float set_temp_3; // value of set temperature for channel 3

char ch_1_filter; // selected filtering method for channel 1
char ch_2_filter; // selected filtering method for channel 2
char ch_3_filter; // selected filtering method for channel 3

float temp_step; // variable to change value of set temperature
float coef_step; // variable to change value of PI coefficients

char temp_step_char; // variable to change value of temp_step (to do not float == float)
char coef_step_char; // variable to change value of coef_step (to do not float == float)

// for Kalman filtering -------channel 1------------//
float R_1;  // covariance matrix cov(A:A,A:A)=variance(A:A)
float Q_1; // process noise covariance matrix, speed of reaction (chose manually)
float P_k1_1;          // Updated covarience estimation
float P_k2_1;           // Predicted next covarience
float K_1;           // Kalman Gain
float x_k_1;          // Kalman estimate of the sensor's temperature
// ------------------------------------------------//

// for Kalman filtering -------channel 2------------//
float R_2;  // covariance matrix cov(A:A,A:A)=variance(A:A)
float Q_2; // process noise covariance matrix, speed of reaction (chose manually)
float P_k1_2;          // Updated covarience estimation
float P_k2_2;           // Predicted next covarience
float K_2;           // Kalman Gain
float x_k_2;          // Kalman estimate of the sensor's temperature
// ------------------------------------------------//

// for Kalman filtering -------channel 3------------//
float R_3;  // covariance matrix cov(A:A,A:A)=variance(A:A)
float Q_3; // process noise covariance matrix, speed of reaction (chose manually)
float P_k1_3;          // Updated covarience estimation
float P_k2_3;           // Predicted next covarience
float K_3;           // Kalman Gain
float x_k_3;          // kalman estimate of the sensor's temperature
// ------------------------------------------------//

// for Moving average filtering using 5 elements ------channel 1-------------//
#define t_numb_1 5 // how many temperatures will be saved in array
float temper_arr_1[t_numb_1];  // to sum up several temperature values
// ------------------------------------------------//

// for Moving average filtering using 5 elements ------channel 2-------------//
#define t_numb_2 5 // how many temperatures will be saved in array
float temper_arr_2[t_numb_2];  // to sum up several temperature values
// ------------------------------------------------//

// for Moving average filtering using 5 elements ------channel 3-------------//
#define t_numb_3 5 // how many temperatures will be saved in array
float temper_arr_3[t_numb_3];  // to sum up several temperature values
// ------------------------------------------------//

float temp_arr_med_1[3]; // for Median filter for 3 elements --- channel 1 -----
float temp_arr_med_2[3]; // for Median filter for 3 elements --- channel 2 -----
float temp_arr_med_3[3]; // for Median filter for 3 elements --- channel 3 -----

float integr_step; // step for integral calculation 5ms

//---- For PID ----channel 1------//
int pwm_val_1; // calculated value of PWM duty cycle for channel 1
float ek_1;           // the delta between set_temp_1 and temp_1
float integr_val_1;   // the value of calculated integral
float C_1;            // Proportional component
float Tintegr_1;     // Integral component
//-----------------------------//

//---- For PID ----channel 2------//
int pwm_val_2; // calculated value of PWM duty cycle for channel 2
float ek_2;           // the delta between set_temp_2 and temp_2
float integr_val_2;   // the value of calculated integral
float C_2;            // Proportional component
float Tintegr_2;     // Integral component
//-----------------------------//

//---- For PID ----channel 3------//
int pwm_val_3; // calculated value of PWM duty cycle for channel 3
float ek_3;           // the delta between set_temp_3 and temp_3
float integr_val_3;   // the value of calculated integral
float C_3;            // Proportional component
float Tintegr_3;     // Integral component
//-----------------------------//

#endif /* MAIN_H_ */