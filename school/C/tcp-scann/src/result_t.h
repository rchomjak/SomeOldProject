/**
@author Richard Chomjak
@file 	result_t.h
@brief  Operations on result_t.
@date 	26/9/2012
*/
#ifndef __RESULT_T_H_
#define __RESULT_T_H_

#include "tcpsearch.h"


void
result_t_init(result_t *);

void
result_t_my_free(void *);

void
result_t_free(result_t *);

void
result_t_free_linked(host_t *);

int
result_t_insert(result_t *, unsigned short , char *);

#endif


