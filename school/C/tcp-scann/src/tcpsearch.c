/**
@author Richard Chomjak
@file 	tcpsearch.c
@brief  Main source of project.
@date 	26/9/2012
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <limits.h>

#include "tcpsearch.h"
#include "errors.h"
#include "mgetopt.h"
#include "ports_range_parser.h"
#include "ports_operations.h"
#include "result_t.h"
#include "host_t.h"
#include "file_read_parser.h"
#include "network.h"
#include "print.h"
static const char progname[] = PROGNAME;

size_t ports_len = 0;
size_t ports_alloc_size = PORT_ALLOC_SIZE;

int
main (int argc, char *argv[] ){
	
	int ret;

	getopt_t getoptions;
	nodemain_t main_list;
	FILE *fp = NULL;
	nodemain_t_init(&main_list);



	getopt_t_init( &getoptions );

	ret = mgetopt(&getoptions, argc, argv);
	if (ret != EPOK){
		if (ret !=EPMEM){
			getopt_t_free(&getoptions);
		}
		
		print_help();
		getopt_error(ret);
		return ret;
	}
	if( getoptions.help_opt){
		usage();
		getopt_t_free(&getoptions);
		getopt_error(ret);
		return EPOK;
	}

	ret = arg_check(argc, MINIMAL_ARGC );
	if (ret != EPOK){
		
		getopt_error(ret);
		return ret;
	}

	


	if (getoptions.timeout_opt){
		
		if( (ret = (getopt_t_check_number(getoptions.timeout)) != EPOK)){
			getopt_t_free(&getoptions);
			getopt_error(ret);
			return ret;
		}
		if(string_to_ul(getoptions.timeout) > 0x7FFFFFFF){
			getopt_t_free(&getoptions);
			getopt_error(EPOPT);
			return EPOPT;
		 }
		main_list.time_sec.tv_sec  =  string_to_ul(getoptions.timeout);	
		main_list.time_sec.tv_usec = 0;
	
	}
	if ( getoptions.connect_timeout_opt){

		if( (ret = (getopt_t_check_number(getoptions.connect_timeout)) != EPOK)){
			getopt_t_free(&getoptions);
			getopt_error(ret);
			return ret;
		}
	}
	

	
	 nodemain_t_cpy_getopt_t(&main_list, &getoptions);

	main_list.ports = nodemain_t_ports_alloc(PORT_ALLOC_SIZE);
	if( main_list.ports == NULL){
		tcpsearch_error(EMEM); 
		return EMEM;
	}
	ret = parser_automata_state_0(getoptions.ports, &(main_list.ports) );
	if ( ret != EPaOK){
		parse_errors(ret);
		getopt_t_free(&getoptions);
		nodemain_t_free(&main_list);
		return ret;
	}


	if ( strlen(getoptions.file) == 1 && getoptions.file[0] == '-'){
		
		ret = file_read(NULL, &main_list, stdin);
		if ( ret != EPfOK){
			file_error(ret);
			getopt_t_free(&getoptions);
			nodemain_t_free(&main_list);
			file_fclose(fp);
			return ret;
		}
		file_fclose(fp);

	}else{
		fp = file_open(getoptions.file);
		if(fp == NULL){

			file_error(EPfFILE);
			getopt_t_free(&getoptions);
			nodemain_t_free(&main_list);
			return EPfFILE;
		}
		ret = file_read(fp, &main_list, NULL );
		if ( ret != EPfOK){
			file_error(ret);
			getopt_t_free(&getoptions);
			nodemain_t_free(&main_list);
			file_fclose(fp);
			return ret;
		}
		file_fclose(fp);
	}
	
	

	///Free getopt_t
	getopt_t_free(&getoptions);

	///network
	
	ret = network_operation(&main_list);
	if(ret != ENOK){

		network_error(ret);
		getopt_t_free(&getoptions);
		nodemain_t_free(&main_list);
		return ret;
	}
	
	print_results(&main_list);
	/// Free linked lists(structures)	
	host_t_free_linked(&main_list);

	///Free nodemain_t
	nodemain_t_free(&main_list);

	return EXIT_SUCCESS;
}

/*
\fn usage
\brief Print usage.
*/
void 
usage(){
	
	fprintf(stdout, "\n%s -h help  write this message \n", progname );
	fprintf(stdout, "%s -c connect timeout [in sec] \n"
		"%s -t recv timeout [in sec] \n"
		"%s -v verbose output \n"
		"%s -p port valid input: 1-5,2,4 \n"
		"%s -s transale port into name of service\n",progname, progname, progname, progname, progname);
	fprintf(stdout, "Example: %s -p 1-5 input_file -t 2 -c 2 -v -s\n",progname);
	fprintf(stdout,"\t If you want read from stdin use input_file \"-\". \n");
	return;
}

/*
\fn print_help
\brief Print usage help.
*/
void 
print_help(){
	fprintf(stderr, "Use %s -h for correct options. \n", progname);
	return;
}

/*
\fn nodemain_t_init
\brief Initialize nodemain_t structure.
@param nodemaint_t *main
*/
void
nodemain_t_init(nodemain_t *main){

	main->ports = NULL;

	main->time_sec.tv_sec = 0x7777777;
	main->time_sec.tv_usec = 0;
	
	main->verbose 	   		  = false;
	main->service_name 		  = false;
	main->timeout 	 		  = false;
	main->connect_timeout_opt = false;
	
	main->host 		= NULL;
	main->LAct 		= NULL;
	main->LFirst 	= NULL;
	main->LLast 	= NULL;

	return;
}

/*
\fn nodemain_t_cpy_getopt_t
\brief Copy value from getopt_t into nodemaint_t.
@param nodemaint_t *main, getopt_t *getoptions
*/
void
nodemain_t_cpy_getopt_t( nodemain_t *main, getopt_t *getoptions){

	main->service_name   		 = getoptions->service_name_opt;
	main->verbose 	   	  		 = getoptions->verbose_opt;
	main->connect_timeout_opt    = getoptions->connect_timeout_opt;
	if(main->connect_timeout_opt)
		main->connect_timeout   = string_to_ul(getoptions->connect_timeout);
	main->timeout 				= getoptions->timeout_opt;

	return;
}

/*
\fn nodemain_t_my_free
\brief Own function for free mem.
@param void *mem
*/
void
nodemain_t_my_free(void *mem){


	if ( mem != NULL){
		free(mem);
	}

	return;
}

/*
\fn nodemain_t_free
\brief Initilize nodemain_t after free.
@param nodemain_t *main
*/
void
nodemain_t_free(nodemain_t *main){

	nodemain_t_my_free(main->ports);

	main->LAct   = NULL;
	main->LLast  = NULL;
	main->LFirst = NULL;
	main->host 	 = NULL;

	return;
}

/*
\fn nodemain_t_ports_alloc
\brief allocation of first port
@param unsigned short *ports
@return void *
*/
void *
nodemain_t_ports_alloc(size_t value){

	return calloc(value, sizeof(unsigned short  ));
}
/*
\fn nodemain_t_first_alloc
\brief First allocation
@param nodemain_t *main
@return int
*/
int
nodemain_t_first_alloc(nodemain_t *main){

	main->host=calloc(1, sizeof(struct host));
	if(main->host == NULL){

		return EMEM;
	}

	main->LAct 		= main->host;
	main->LFirst 	= main->host;
	main->LLast 	= main->host;

	return EOK;
}

/*
\fn nodemain_t_create_and_cpy_str
\brief Allocation host_t and copy string.
@param nodemain_t *main, char *buffer
@return int
*/
int
nodemain_t_create_and_cpy_str(nodemain_t *main, char *buffer){

	if(main->host == NULL){
		main->host=calloc(1, sizeof(struct host));
		if(main->host == NULL){

			return EMEM;
		}

		main->LAct 		= main->host;
		main->LFirst 	= main->host;
		main->LLast 	= main->host;

	}else if( main->LAct->next == NULL ){
		
		main->LAct->next=calloc(1, sizeof(struct host));
		if(main->LAct->next == NULL){

			return EMEM;
		}

		host_t_init(main->LAct->next);

		main->LAct  		=	main->LAct->next;
		main->LLast     	=   main->LAct;
		main->LAct->next	= 	NULL;
		
	}

	 main->LAct->input_string=calloc(strlen(buffer)+1, sizeof(char));
	 if(main->LAct->input_string == NULL){

	 	return EMEM;
	 }

	
	 strcpy(main->LAct->input_string,buffer);

	return EOK;
}

/**
\fn tcpsearch_error
\brief Error params.
@messages int ret
@return error msg
*/
void
tcpsearch_error( int ret){

	
	switch(ret){
		case EPOK:

			break;
		case EARGC:
			fprintf(stderr," Error with len of input arguments. (tcpsearch.h EARGC). Try %s -h.\n" 
				"For more information look into tcpsearch.c and tcpsearch.h\n", PROGNAME);
			break;
		case EARGV:
			fprintf(stderr,"Error, bad input arguments. (tcpsearch.h EARGV). Try %s -h.\n" 
				"For more information look into tcpsearch.c and tcpsearch.h \n", PROGNAME);
			break;
		case EMEM:
			fprintf(stderr," Error, memory allocation. (tcpsearch.h EMEM). \n" 
				"For more information look into tcpsearch.c and tcpsearch.h\n");
			break;


		default:

			break;
	}
	

	return;
}
