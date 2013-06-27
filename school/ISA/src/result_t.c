/**
@author Richard Chomjak
@file 	result_t.c
@brief  Operations on result_t.
@date 	26/9/2012
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "result_t.h"
#include "tcpsearch.h"
#include "errors.h"

/**
\fn result_t_init
\brief Initialize result_t.
@param	result_t *result
@return void
*/
void
result_t_init(result_t *result){

	result->port 		 = 0;
	result->banner 		 = NULL;
	result->next		 = NULL;

}

/**
\fn result_t_my_free
\brief Own free if mem is not null, freet.
@param	void *mem
@return void
*/
void
result_t_my_free(void *mem){

	if( mem != NULL){

		free(mem);
	}

	return;
}

/**
\fn result_t_free
\brief Free result_t.
@param	result_t *result
@return void
*/
void
result_t_free(result_t *result){

	result->port 		 = 0;
	result_t_my_free(result->banner);
	result->next		 = NULL;

}

/**
\fn result_t_free_linked
\brief Free result_t linked list.
@param	host_t *host
@return void
*/
void
result_t_free_linked(host_t *host){

	struct result *tmp;
	struct result *tmp2;

	tmp = host->LFirst;

	while( tmp != NULL){
		
		tmp2 = tmp->next;
		
		result_t_free(tmp);
		result_t_my_free(tmp);
		tmp = tmp2;
	}
	
	return;
}


/**
\fn result_t_insert
\brief Insert data into result_t
@param result_t *result, unsigned short port, char *buffer
@return int
*/
int
result_t_insert(result_t *result, unsigned short port, char *buffer){

	
	result->banner = calloc(strlen(buffer) +1, sizeof(char));
	if(result->banner == NULL){

		return EMEM;
	}

	strcpy(result->banner, buffer);

	result->port = port;

	return EOK;
}
