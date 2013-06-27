/**
@author Richard Chomjak
@file 	file_read_parser.h
@brief  Parse and controll port range.
@date 	4/10/2012
*/

#include <stdio.h>
#include <stdlib.h>
#include "tcpsearch.h"

enum fparse_error{
	
	EPfOK=EXIT_SUCCESS,
	EPfLEN,
	EPfOPT,
	EPfMEM,
	EPfFILE

};
enum fparser_state{

	Fstart = 0,
	Fget,
	Fignore, 
	Fnewline
	

};

int 
file_read( FILE *, nodemain_t *, void *);

int
file_buffer_push(char **, int , size_t *, size_t *);

FILE *
file_open(char *);

void
file_fclose(FILE *);

void
file_error(int );
