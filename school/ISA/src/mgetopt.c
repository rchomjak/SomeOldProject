/**
@author Richard Chomjak
@file 	mgetopt.c
@brief  Parse input option.
@date 	26/9/2012
*/

#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>

#include "errors.h"
#include "mgetopt.h"
#include "tcpsearch.h"
#include "string.h"


/*
\fn getopt
\brief function to parse input parameters, using getopt structure
@param getopt_t *getoptions, int argc, char *argv
@return enum_code
*/
int 
mgetopt( getopt_t *getoptions, int argc, char **argv ){

	for(size_t i = 1 ; i < argc ; i++){

		
		if( !strcmp("-p",argv[i]) ){
			if( ( i + 1 ) >= argc ){
				return EPLEN;
			}
			else{
				
				if ( getoptions->port_opt){
					return EPOPT;
				}
				getoptions->port_opt = true;
				getoptions->ports 	 = calloc( strlen(argv[i + 1]) + 1, sizeof(char) );
				if( getoptions->ports == NULL ){
					return EPMEM;
				}
				memcpy(getoptions->ports, argv[i + 1], strlen(argv[i + 1]) +1 );
				i++;
			}

		}
		else if( !strcmp("-t",argv[i])){
			
			if( ( i + 1 ) >= argc ){
				return EPLEN;
			}
			else{
				if( getoptions->timeout_opt){
					return EPOPT;
				}
				getoptions->timeout_opt = true;
				getoptions->timeout 	= calloc( strlen(argv[i +1]) + 1, sizeof(char) );
				if( getoptions->timeout == NULL ){
					return EPMEM;
				}
				memcpy(getoptions->timeout, argv[i + 1], strlen(argv[i + 1]) +1 );
				i++;
			}
		}	
		else if( !strcmp("-c",argv[i])){
			
			if( ( i + 1 ) >= argc ){
				return EPLEN;
			}
			else{
				if( getoptions->connect_timeout_opt){
					return EPOPT;
				}
				getoptions->connect_timeout_opt = true;
				getoptions->connect_timeout 	= calloc( strlen(argv[i +1]) + 1, sizeof(char) );
				if( getoptions->connect_timeout == NULL ){
					return EPMEM;
				}
				memcpy(getoptions->connect_timeout, argv[i + 1], strlen(argv[i + 1]) +1 );
				i++;
			}
		}	
		else if (! strcmp("-s",argv[i])) {
			
			if( getoptions->service_name_opt){	
					return EPOPT;
			}
			getoptions->service_name_opt = true;
						

		}
		else if (! strcmp("-h",argv[i])) {
			
			getoptions->help_opt = true;
			return EOK;
		}
		else if (! strcmp("-v",argv[i])) {
			
			if( getoptions->verbose_opt){	
				return EPOPT;
			}

			getoptions->verbose_opt = true;

		}	
		else{
			
			if( getoptions->file_opt){
				return EPOPT;
			}
			getoptions->file_opt = true;
			
			getoptions->file 	 = calloc( strlen(argv[i]) +1, sizeof(char));
			if( getoptions->file == NULL ){
				return EPMEM;
			}

			memcpy(getoptions->file, argv[i], strlen(argv[i]) +1 );

		}		

		
	}


	return getopt_t_required_arguments(getoptions);
	
}

/**
\fn arg_check
\brief check size of argc
@param int argc, size_t max_arg
@return enum_code
*/
int
arg_check( int argc, size_t minimal_arg){

	if ( argc < minimal_arg ){
		return EPOPT;
	}
	return EPOK;
}

/**
\fn getopt_t_init
\brief initialize structure
@param getopt_t *getoptions
@return void
*/
void
getopt_t_init( getopt_t *getoptions){

	getoptions->file=NULL;
	getoptions->ports=NULL;
	getoptions->timeout=NULL;
	getoptions->connect_timeout=NULL;

	getoptions->file_opt=false;
	getoptions->port_opt=false;
	getoptions->help_opt=false;
	getoptions->verbose_opt=false;
	getoptions->service_name_opt=false;
	getoptions->timeout_opt=false;
	getoptions->connect_timeout_opt=false;
	return;
}

/**
\fn getopt_t_free
\brief initialize structure
@param getopt_t *getoptions
@return void
*/
void
getopt_t_free(getopt_t *getoptions){

	getopt_t_my_free(getoptions->file);
	getopt_t_my_free(getoptions->ports);
	getopt_t_my_free(getoptions->timeout);
	getopt_t_my_free(getoptions->connect_timeout);

	return;
}

/**
\fn getopt_t_my_free
\brief Free memory
@param void *mem
*/
void
getopt_t_my_free(void *mem){

	if( mem != NULL ){

		free(mem);
	}
	return;
}

/**
\fn getopt_error
\brief Error params.
@messages int ret
@return error msg
*/
void
getopt_error( int ret){

	
	switch(ret){
		case EPOK:

			break;
		case EPLEN:
			fprintf(stderr," Error with len of input arguments. (getopt.h EPLEN). Try %s -h.\n" 
				"For more information look into getopt.c and getopt.h \n", PROGNAME);
			break;
		case EPOPT:
			fprintf(stderr,"Error, bad input arguments. (getopt.h EPOPT). Try %s -h.\n" 
				"For more information look into getopt.c and getopt.h \n", PROGNAME);
			break;
		case EPMEM:
			fprintf(stderr," Error, memory allocation . (getopt.h EPMEM). \n" 
				"For more information look into getopt.c and getopt.h \n");
			break;

		default:

			break;
	}
	

	return;
}

/**
\fn getopt_t_required_arguments
\brief Checks required arguments.
@param getopt_t *getoptions
@return enom code
*/
int 
getopt_t_required_arguments(getopt_t *getoptions){

	if( !getoptions->file_opt || !getoptions->port_opt )
			return EPOPT;
	
	return EPOK;
}

/**
\fn getopt_t_check_number
\brief Checks if is number.
@param char *time
@return enum code
*/
int
getopt_t_check_number( char *time ){

	size_t i=0;
	while( time[i] != '\0'){

		if(!isdigit(time[i]))
			return EPOPT;
		i++;

	}

return EPOK;

}

/*
\fn ports_convert
\brief Convert string to unsigned long with check maximum value.
@param char *buffer
@return unsigned long convert
*/
unsigned long
string_to_ul(char *buffer){

	return strtoul(buffer, NULL, 0);

	
}

