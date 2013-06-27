/**
@author Richard Chomjak
@file 	network.h
@brief  Network operation.
@date 	11/10/2012
*/

#ifndef __NETWORK_H_
#define __NETWORK_H_

#include <stdlib.h>
#include "tcpsearch.h"
/** MINIMUM 1 !!! */
#define NETWORK_BUFFER 500

enum network_error{
	
	ENOK=EXIT_SUCCESS,
	ENGETADDR,
	ENSOCKET,
	ENCONNECT,
	ENLEN,
	ENOPT,
	ENMEM

};
int
network_operation(nodemain_t *);

void
insert_port_2_sockaddr(struct addrinfo *, unsigned short);

void
connect_signal_alrm_check();

int
wait_to_read(int , struct timeval);

void
convert_pton(struct addrinfo *, host_t *);

void 
hostname_of_host(struct addrinfo *,char *);

void
network_error( int );

#endif
