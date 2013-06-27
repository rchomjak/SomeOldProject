/**
@author Richard Chomjak
@file 	tcpsearch.h
@brief  Structures of results.
@date 	26/9/2012
*/
#ifndef __TCPSEARCH_H_
#define __TCPSEARCH_H_

#include <sys/time.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "mgetopt.h"

#define MINIMAL_ARGC 4
#define PROGNAME "tcpsearch"

#define PORT_ALLOC_SIZE 10000

#ifndef NI_MAXHOST
#define NI_MAXHOST 1025
#endif


struct nodemain;
struct host;
struct result;




/**
	\struct nodemain
	\brief Main structure.

	This is a main structure of program.
	Include pointer to other structure, which is a one-way list.
 */
typedef struct nodemain{
	
	unsigned short *ports; 			/** Defined ports by user. */
	struct timeval time_sec; 		/** Struct time library. */
	size_t connect_timeout;			/** Time for connect timeout. */
		
	struct host *host;				/** Pointer to host. */
	struct host *LAct;
	struct host *LFirst;
	struct host *LLast;
	
	bool verbose;					/** Verbose output. */
	bool service_name; 				/** Translate port id into service name. */
	bool connect_timeout_opt;
	bool timeout;

}nodemain_t;

/**
	\struct host
 	\brief Structure for every read host.
 	Every host record has own one way linked list. 
 */

typedef struct host{
	
	char *input_string;			/** Input string from user (remote node). IPv4, IPv6 address or hostname.*/
	struct addrinfo *host_info;	/** Address of remote node. */

	char ipv6[INET6_ADDRSTRLEN];
	char ipv4[INET_ADDRSTRLEN];
	char nsname[NI_MAXHOST];

	struct result *result;		/** Pointer to struct of result for every "live" port. */
	struct result *LAct;
	struct result *LFirst;
	struct result *LLast;

	bool exists_host;			/** If getaddrinfo OK. */
	struct host *next;			/** Linked list. */
	
}host_t;

/** \struct result
	\brief Structure for results after connect to socket(host).

*/
typedef struct result{
	
	unsigned short port;		/** Which port used. */
	char *banner;				/** Banner message from server(service). */
	
	struct result *next;		/** Linked list. */

}result_t;

void
print_help();

void
usage();

void
nodemain_t_init(nodemain_t *);

void
nodemain_t_cpy_getopt_t( nodemain_t *, getopt_t *);

void
nodemain_t_my_free(void *);

void
nodemain_t_free(nodemain_t *);

void *
nodemain_t_ports_alloc(size_t);

int
nodemain_t_first_alloc(nodemain_t *);

int
nodemain_t_create_and_cpy_str(nodemain_t *, char *);

void
tcpsearch_error( int );

#endif
