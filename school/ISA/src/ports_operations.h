/**
@author Richard Chomjak
@file 	ports_operations.h
@brief  Basic operations with ports.
@date 	3/10/2012
*/


#ifndef __PORTS_OPERATIONS_H_
#define __PORTS_OPERATIONS_H_

#include <stdlib.h>

#define MAXIMUM_PORT_VALUE 65535

int 
ports_push( unsigned short **, unsigned short);

int 
ports_sort(unsigned short *);

int 
ports_compare(unsigned short ** , char *, char* );

int 
ports_push_range(unsigned short **, unsigned short, unsigned short );

unsigned long
ports_convert( char *);

void
ports_error(int value);


enum ports_op_error{
	
	EPOOK=EXIT_SUCCESS,
	EPOLEN,
	EPOOPT,
	EPOMEM,
	EPORANGE,
	
};
#endif
