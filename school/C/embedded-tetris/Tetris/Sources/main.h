/**
@author Richard Chomjak
@file 	main.h
@brief  Main source of project.
@date 	11/11/2012
*/
#ifndef _MAIN_H
#define _MAIN_H




#define TRUE 1
#define FALSE 0

#define BTN_PORT1D PTGD
#define BTN_PORT2D PTDD
#define LED_PORTD PTCD
#define LED_PORTDD PTCDD

#define BTN_UP 1
#define BTN_DOWN 2 
#define BTN_LEFT 4
#define BTN_RIGHT 8 
#define BTN_OK 16


#define MATRIX_R_SIZE 8
#define MATRIX_C_SIZE 8
#define ELEMENT_SIZE 4
#define NUMBER_OF_BLOCKS 7

void 
delay_var(unsigned int);

void 
set_simplex(); 

void 
set_simplex_port_A(); 

void 
set_simplex_port_B();

void 
set_simplex_port_C();

void 
set_simplex_port_D();

void 
set_simplex_port_E();

void 
set_simplex_port_F();

void
row_map(int);

void 
column_matrix_2_reg();

void 
column_map(unsigned char );

void 
keyboard_interrupt_set();

void 
tmp1ovf_settings();

int 
random_element();

unsigned int 
btn_state();





typedef struct block{
	char 	x;
	char	y;
	char type_of_element;
	char current_rotation;
    char element_chose;
} block_t;

#endif