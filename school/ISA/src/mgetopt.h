/**
@author Richard Chomjak
@file 	mgetopt.h
@brief  Getopt header file.
@date 	26/9/2012
*/

#ifndef __mGETOPT_H_
#define __mGETOPT_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


/**	\typedef getopt_t
 	\brief Data type of structure for every read host. Own getopt with options.
 */
 /** \struct getopt
 	 \brief Structure for every read host. Own getopt with options.
 */
typedef struct mgetopt{
	
	char *file;					/** Name of input filename. */
	char *ports;				/** List of ports. */
	char *timeout;				/** Specify timeout. */
	char *connect_timeout;		/** Connect timeout. */
							
	bool connect_timeout_opt;	/** If connect() timeout defined. */
	bool file_opt;				/** If file defined. */
	bool port_opt;				/** If port defined. */
	bool timeout_opt;			/** If timeout defined. */
	bool verbose_opt;			/** If verbose output. */
	bool service_name_opt;		/** If user want to translate port id to name of service from /etc/service. */
	bool help_opt;				/** If user defined help */


}getopt_t;

/**
	\enum getopt_errors
	\brief Enum for getopt erros.

*/
enum getopt_errors{
	
	EPOK=EXIT_SUCCESS,
	EPLEN,
	EPOPT,
	EPMEM

};


int 
mgetopt( getopt_t *, int, char ** );
int
arg_check( int , size_t );
void
getopt_t_init( getopt_t *);
void 
getopt_t_my_free(void *);
void
getopt_t_free(getopt_t *);
int 
getopt_t_required_arguments(getopt_t *);
int 
getopt_t_check_number(char *);
void
getopt_error( int );
unsigned long
string_to_ul(char *);

#endif
