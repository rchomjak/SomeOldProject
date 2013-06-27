/**
@author Richard Chomjak
@file 	ports_operations.c
@brief  Basic operations with ports.
@date 	3/10/2012
*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ports_operations.h"
#include "tcpsearch.h"


extern size_t ports_len;
extern size_t ports_alloc_size;

/**
\fn ports_push
\brief Append value into array.
@param unsigned short *ports, unsigned short value
@return enum_code
*/
int
ports_push(unsigned short **ports, unsigned short value){
	unsigned short *help = NULL;
	
	if ( ports == NULL){

		return EPOMEM;
	}
	else {
			if ( ports_len + 1 >= ports_alloc_size){
				unsigned short *tmpptr = NULL;
				
				ports_alloc_size += PORT_ALLOC_SIZE;
				
				tmpptr = realloc(*ports, ports_alloc_size * sizeof(unsigned short ));
				
				if (tmpptr == NULL){
					
					return EPOMEM;
				}else{

					*ports = tmpptr;
					tmpptr = NULL;

				}
				
				
			}
		/** Because array != pointer */
		 help = *ports + ports_len;
		*help = value;

		ports_len = ports_len + 1;
	
	}


	return EPOOK;
}

// Zavrhnute, lunex je zly!
int
ports_sort(unsigned short *ports){

printf("Nezabudni doimplementovat mergesort ports_operations.c \n");

//mergesort(ports , 4, 20 );

	return EPOOK;
}

/**
\fn ports_compare
\brief Compares between value of range.
@param unsigned short *ports, char *buffer_1, char* buffer_2
@return enum_code
*/
int 
ports_compare(unsigned short **ports, char *buffer_1, char* buffer_2){

int ret = EPOOK;
	unsigned long ul_buffer_1;
	unsigned long ul_buffer_2;

		
	ul_buffer_1 = ports_convert(buffer_1);
	ul_buffer_2 = ports_convert(buffer_2);	
	if((ul_buffer_1 >= ul_buffer_2) || ( ul_buffer_1 == MAXIMUM_PORT_VALUE + 1 || ul_buffer_2 == MAXIMUM_PORT_VALUE + 1)) 
		return EPORANGE;
	
	ret = ports_push_range(ports, (unsigned short) ul_buffer_1, (unsigned short) ul_buffer_2);
	 /// Variable ret returns include enum ports_op_error.

	return ret;
;}

/**
\fn ports_push_range
\brief Push value into ports.
@param unsigned short *ports, unsigned short port_range_start, unsigned short port_range_end
@return enum_code
*/
int 
ports_push_range(unsigned short **ports, unsigned short port_range_start, unsigned short port_range_end ){

int ret = EPOOK;
	for ( size_t start = port_range_start; start <= port_range_end; start++){

		ret = ports_push(ports, start);
		if ( ret != EPOOK){
			return ret;
		}
	}

	return EPOOK;
}

/**
\fn ports_convert
\brief Convert string to unsigned long with check maximum value.
@param char *buffer
@return unsigned long convert
*/
unsigned long
ports_convert(char *buffer){

	unsigned long convert;
	return MAXIMUM_PORT_VALUE < (convert = strtoul(buffer, NULL, 0))? (MAXIMUM_PORT_VALUE + 1): convert;

	
}
/**
\fn ports_error
\brief Printing error to stderr.
@param int value
@return void
*/
void
ports_error(int value){


		switch(value){
		case EPOOK:

			break;
		case EPOLEN:
			fprintf(stderr," Error with len of input arguments.Ports operation (ports_operation.c).\n");
			break;
		case EPOOPT:
			fprintf(stderr,"Error, bad input arguments. Ports operation (ports_operation.c).\n" );
			break;
		case EPOMEM:
			fprintf(stderr," Error, memory allocation.Ports operation (ports_operation.c).\n");
			break;
		case EPORANGE:
			fprintf(stderr," Error, range of ports .Ports operation (ports_operation.c).\n");
			break;


		default:

			break;

	}
}




