/**
@author Richard Chomjak
@file 	host_t.h
@brief  Operation on host_t.
@date 	26/9/2012
*/
#ifndef __HOST_T_H_
#define __HOST_T_H_

#include "tcpsearch.h"

void
host_t_init(host_t *);

void
host_t_my_free(void *);

void
host_t_free(host_t *);

void
host_t_free_linked(nodemain_t *);

int
host_t_create_result(host_t *);


#endif
