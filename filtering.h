#ifndef FILTERING_H_
#define FILTERING_H_

#include "main.h"

float kalman_filter(float z_k, int chan); // Kalman filter
float moving_aver_filt(float curr_temp, int chan); // Moving average filtering
float median_filter(float curr_temp, int chan); // Median filtering

#endif