/**
@author Richard Chomjak
@file 	ports_range_parser.c
@brief  Parse and controll port range.
@date 	28/9/2012
*/


#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "ports_range_parser.h"
#include "ports_operations.h"



/**
* \brief Maximum value of UDP/TCP port is 65535 in base 10. Initialize is as strlen(65535) + 1 because NUL byte.
*/
#define MAX_DIGIT 6

/**
* \brief Global variable for increments input_ports.
*/
size_t via_ports  = 0;

/**
* \brief Global variable for state of automata.
*/
int previous_state = Pstart;



/*
\fn parser_automata_state_0
\brief Nul state of automata for parsing port range from argv.
@param char *input_ports, unsigned short *ports
@return enum_code
*/
int 
parser_automata_state_0(char *input_ports, unsigned short **ports){

	
	int ret;
	char *buffer_1=NULL;
	char *buffer_2=NULL;

	if( isdigit(input_ports[0])){
		
		buffer_1 = calloc(MAX_DIGIT, sizeof(char));
		if( !buffer_1 ){

			return EPaMEM;
		}
		buffer_2 = calloc(MAX_DIGIT, sizeof(char));
		if( !buffer_2 ){

			return EPaMEM;
		}
		
		previous_state = Pdigit;
		
		ret = parser_digit_state_1( input_ports, ports, buffer_1, buffer_2);

		parser_state_0_free(buffer_1);
		parser_state_0_free(buffer_2);

		return ret;
	}else
		return EPaOPT;


	return EPaOK;
}

/*
\fn parser_digit_state_1
\brief First state of automata for parsing port range from argv.
@param char *input_ports, unsigned short *ports, char *buffer_1, char *buffer_2
@return enum_code
*/
int 
parser_digit_state_1(char *input_ports, unsigned short **ports, char *buffer_1, char *buffer_2){
	int ret = EPaOK;
	
	if( isdigit(input_ports[via_ports])){
		
		if (  strlen(buffer_2) > 0  || previous_state == Pdash ){
			if ( strlen(buffer_2) >= MAX_DIGIT -1){
				return EPaOPT;
			}
			buffer_2[strlen(buffer_2)] = input_ports[via_ports];

		}
		else {
			if ( strlen(buffer_1) >= MAX_DIGIT -1){
				return EPaOPT;
			}
			buffer_1[strlen(buffer_1)] = input_ports[via_ports];
		}

		previous_state = Pdigit;

		via_ports++;
		ret = parser_digit_state_1(input_ports, ports, buffer_1, buffer_2);
		if ( ret != EPaOK )
			return ret;
	}
	else if ( input_ports[via_ports] == '-' ){
		previous_state = Pdash;
		
		via_ports++;
		ret = parser_dash_state_2(input_ports, ports, buffer_1, buffer_2);
		if ( ret != EPaOK ){
			return ret;
		}
	}
	else if ( input_ports[via_ports] == ',' ){
		// Function for compare buffer_1 and buffer_2 => and save to ports
		if( strlen(buffer_2) > 0){
			
			ret = ports_compare(ports, buffer_1, buffer_2);
			memset(buffer_1,'\0',MAX_DIGIT);
			memset(buffer_2,'\0',MAX_DIGIT);	
			if ( ret != EPOOK){
					ports_error(ret);
					return ret;
			}
		}else if ( ports_convert( buffer_1 ) == ( MAXIMUM_PORT_VALUE + 1) ) {

			return EPORANGE;

		}else {
			ret = ports_push( ports,(unsigned short) ports_convert( buffer_1 ));
			if (ret != EPOOK){
				ports_error(ret);
				return ret;
			}

			memset(buffer_1,'\0',MAX_DIGIT);
		}


		previous_state = Pcomma;
		via_ports++;

		ret = parser_comma_state_3(input_ports, ports, buffer_1, buffer_2);
		if ( ret != EPaOK ){
				ports_error(ret);
				return ret;
	}}
	else if ( input_ports[via_ports] == '\0'){
		// Function for compare buffer_1 and buffer_2 => and save to ports
		if( strlen(buffer_2) > 0){

			ret = ports_compare( ports, buffer_1, buffer_2);
			memset(buffer_1,'\0',MAX_DIGIT);
			memset(buffer_2,'\0',MAX_DIGIT);	
			if ( ret != EPOOK){
					ports_error(ret);	
					return ret;
			}
		}else if ( ports_convert( buffer_1 ) == ( MAXIMUM_PORT_VALUE + 1) ) {

			ports_error(EPORANGE);
			return EPORANGE;

		}else {
			ret = ports_push( ports,(unsigned short) ports_convert( buffer_1 ));
			if (ret != EPOOK){
				ports_error(ret);
				return ret;
			}	
			memset(buffer_1,'\0',MAX_DIGIT);
		}


		return EPaOK;
	}
	else
		return EPaOPT;
	
	return EPaOK;
}

/*
\fn parser_dash_state_2
\brief Second state of automata for parsing port range from argv.
@param char *input_ports, unsigned short *ports, char *buffer_1, char *buffer_2
@return enum_code
*/
int
parser_dash_state_2(char *input_ports, unsigned short **ports, char *buffer_1, char *buffer_2){
	int ret = EPaOK;

	if( isdigit(input_ports[via_ports])){

		ret = parser_digit_state_1(input_ports, ports, buffer_1, buffer_2);
		if ( ret != EPaOK)
			return ret;
	}
	else
		return EPaOPT;

	return EPaOK; 
}

/*
\fn parser_comma_state_3
\brief Third state of automata for parsing port range from argv.
@param char *input_ports, unsigned short *ports, char *buffer_1, char *buffer_2
@return enum_code
*/
int
parser_comma_state_3(char *input_ports, unsigned short **ports, char *buffer_1, char *buffer_2){
	int ret = EPaOK;
	
	if( isdigit(input_ports[via_ports])){
		
		ret = parser_digit_state_1(input_ports, ports, buffer_1, buffer_2);
		if ( ret != EPaOK)
			return ret;
	}
	else
		return EPaOPT;

	return EPaOK;
}

/*
\fn parser_state_0_free
\brief Function for remove allocated memory.
@param void *mem
*/
void
parser_state_0_free( void *mem){

	if( mem != NULL){
		free(mem);
	}
	return;
}

/**
\fn parse_errors
\brief Printing error to stderr.
@param int value
@return void
*/
void
parse_errors(int value){

	switch(value){
		case EPaOK:

			break;
		case EPaLEN:
			fprintf(stderr," Error with len of input arguments. Parse of ports (ports_range_parser.c).\n");
			break;
		case EPaOPT:
			fprintf(stderr,"Error, bad input arguments. Parse of ports (ports_range_parser.c).\n" );
			break;
		case EPaMEM:
			fprintf(stderr," Error, memory allocation. Parse of ports (ports_range_parser.c)\n");
			break;

		default:

			break;

	}

	return;
}
