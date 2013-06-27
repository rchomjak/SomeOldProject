/**
@author Richard Chomjak
@file 	host_t.c
@brief  Operations on host_t.
@date 	26/9/2012
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "host_t.h"
#include "tcpsearch.h"
#include "errors.h"
#include "result_t.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>



/**
\fn host_t_init
\brief Initialize host_t.
@param	host_t *host
@return void
*/
void
host_t_init(host_t *host){

	host->input_string = NULL;
	
	host->result = NULL;
	host->LAct 	 = NULL;
	host->LFirst = NULL;
	host->LLast  = NULL;

	host->next   = NULL;
	host->exists_host = false;

}

/**
\fn host_t_my_free
\brief Own free if *mem not null free.
@param	void *mem
@return void
*/
void
host_t_my_free(void *mem){

	if( mem != NULL){

		free(mem);
	}

	return;
}

/**
\fn host_t_free
\brief Free host_t.
@param	host_t *host
@return void
*/
void
host_t_free(host_t *host){

	host_t_my_free(host->input_string);
	if(host->host_info !=NULL)
		freeaddrinfo(host->host_info);
	
	host->result = NULL;
	host->LAct 	 = NULL;
	host->LFirst = NULL;
	host->LLast  = NULL;

	host->next   = NULL;

	return;
}

/**
\fn host_t_free_linked
\brief Free linked list
@param	nodemain_t *main
@return void
*/
void
host_t_free_linked(nodemain_t *main){

	struct host *tmp  =	NULL;
	struct host *tmp2 = NULL;

	tmp = main->LFirst;

	while( tmp != NULL){


		tmp2 = tmp->next;
		
		result_t_free_linked(tmp);
		
		host_t_free(tmp);
		host_t_my_free(tmp);
		tmp = tmp2;
	}

	
	return;
}

/**
\fn host_t_create_result
\brief Create result_t.
@param	host_t *main
@return int
*/
int
host_t_create_result(host_t *main){

	if(main->result == NULL){
		main->result=calloc(1, sizeof(struct result));
		if(main->result == NULL){

			return EMEM;
		}

		main->LAct 		= main->result;
		main->LFirst 	= main->result;
		main->LLast 	= main->result;

	}else if( main->LAct->next == NULL ){
		
		main->LAct->next=calloc(1, sizeof(struct result));
		if(main->LAct->next == NULL){

			return EMEM;
		}

		result_t_init(main->LAct->next);

		main->LAct  		=	main->LAct->next;
		main->LLast     	=   main->LAct;
		main->LAct->next	= 	NULL;
		
	}

	return EOK;
}


