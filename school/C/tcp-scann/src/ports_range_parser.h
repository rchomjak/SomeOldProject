/**
@author Richard Chomjak
@file 	ports_range_parser.h
@brief  Ports_range_parser header file.
@date 	28/9/2012
*/

#ifndef __PORTS_RANGE_PARSER_H_
#define __PORTS_RANGE_PARSER_H_

#include <stdlib.h>


int 
parser_automata_state_0(char *, unsigned short **);

int 
parser_digit_state_1(char *, unsigned short **,char *, char *);

int 
parser_dash_state_2(char *, unsigned short **, char *, char *);

int 
parser_comma_state_3(char *, unsigned short **, char *, char *);

void
parser_state_0_free(void *);

void
parse_errors(int value);

enum parse_error{
	
	EPaOK=EXIT_SUCCESS,
	EPaLEN,
	EPaOPT,
	EPaMEM

};

enum parser_state{

	Pstart = 0,
	Pdigit, 
	Pdash,
	Pcomma

};


#endif


