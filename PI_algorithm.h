#ifndef PI_ALGORITHM_H_
#define PI_ALGORITHM_H_

#include "main.h"

void calc_PI_pwm(int chan); // calculate PWM value using PI algorithm
void set_PI_coef_step(void); // set PI coefficient's step corresponding to coef_step_char

#endif