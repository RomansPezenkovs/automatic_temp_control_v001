#ifndef USER_INTERFACE_H_
#define USER_INTERFACE_H_

#include "main.h"

void print_temp(float temp); // print three symbols of temperature XX.X
void print_3_dig_int(int number); // print number of three digits
void print_menu(int number); // print LCD user interface
void print_filt(char method); // print filtration method
void print_char_sel_temp(void); // print '>' for temperature step
void print_char_sel_PI(void); // print '>' for PI coefficient's step
void print_char_arrow_temp(void); // print arrow char for temperature step
void print_char_arrow_PI(void); // print arrow char for PI coefficient's step
void clear_char_arrow_temp(void); // clear arrows for temperature step
void clear_char_arrow_PI(void); // clear arrows for PI coefficient's step
void print_interface(void); // print user interface according to Pin Change Interrupt changes

#endif