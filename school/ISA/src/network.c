/**
@author Richard Chomjak
@file 	network.c
@brief  Network operation.
@date 	11/10/2012
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>

#include "tcpsearch.h"
#include "host_t.h"
#include "result_t.h"
#include "network.h"
#include "errors.h"

extern size_t ports_len;


/**
\fn print_results
\brief Network operation.
Create sockets,connect, receive data
@param	nodemain_t *main
@return int 
*/
int
network_operation(nodemain_t *main){

	struct host *tmp = NULL;
	tmp = main->LFirst;
	int ret = 0;
	
	/** For "local" socket */
	struct addrinfo hints; 
	int socketfd = 0;
	
	
	/** Alarm for connect() */
	struct sigaction sact;
	sigemptyset(&sact.sa_mask);
	sact.sa_flags = 0;
	sact.sa_handler = connect_signal_alrm_check;
	sigaction(SIGALRM, &sact, NULL);

	/* Buffer for recv */
	char *network_buffer = calloc(NETWORK_BUFFER,sizeof(char));
	
	if(network_buffer == NULL){

		return ENMEM;
	}

	size_t current_network_buffer_len = NETWORK_BUFFER ;
	size_t old_network_buffer_len 	  = current_network_buffer_len;
	size_t buffer_ptr				  = 0;
	ssize_t number_recv_bytes 		  = 0;
	///via hosts

	while( tmp !=NULL ){

		struct addrinfo *p = NULL;
   		ret = 0;		
   		
   		close(socketfd);
   	
   		memset(&hints, 0, sizeof(hints));
		hints.ai_family   = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;

   		if (( ret = getaddrinfo( tmp->input_string, NULL, &hints, &(tmp->host_info) )) != 0 ){
   			fprintf(stderr, "Error with destination: %s , getaddrinfo: %s \n",tmp->input_string ,gai_strerror(ret));
   			tmp = tmp->next;
   			continue;
   		}

   		tmp->exists_host = true;

   		
		/// via ports 
		for(size_t i = 0; i < ports_len; i++){


			close(socketfd);

			for( p = tmp->host_info ; p != NULL  ; p = p->ai_next ){			

   			if((socketfd = socket( p->ai_family,p->ai_socktype, p->ai_protocol) < 0)){
   				if( main->verbose){
   					fprintf(stderr, "Error with destination: %s \n",tmp->input_string);
   					perror("Socket error");
    			}
    			continue;
   			}

   			break;
   			}
   	

   			if( socketfd < 0 ){
   				tmp = tmp->next;
   				continue;
   			}
   			
   			/// Insert destination IP into host_t

   			
   			insert_port_2_sockaddr(p, *(main->ports + i));


			/// connect
			if(main->connect_timeout_opt){
				if(alarm(main->connect_timeout)!=0 ){
					fprintf(stderr,"Connect alarm was already set.");

				}
			}
			if ((ret = connect(socketfd,p->ai_addr, p->ai_addrlen)) < 0)
			{
				if(errno == EINTR){
					errno = ETIMEDOUT;
					
				}

				if(main->verbose){
					fprintf(stderr, "Error with destination: %s:%u \n",tmp->input_string,*(main->ports +i));
					perror("Connect error");
				}
				alarm(0);
				continue;
			}
			 
			 alarm(0);

			

   			if(ret < 0){
   				fprintf(stderr," connect(); error\n");
   				return ret;
   			}
   			
   			/// Create structure for result
   			ret = host_t_create_result(tmp);
			if ( ret != EOK){
				fprintf(stderr,"Internal error\n");
				exit(ret);
   			}
			//select and non-block socket
				
			buffer_ptr = 0;
			fcntl(socketfd, F_SETFL,O_NONBLOCK);
			while(wait_to_read(socketfd, main->time_sec) > 0){

				//recv
				number_recv_bytes = recv(socketfd,network_buffer + buffer_ptr, NETWORK_BUFFER -1, 0 );

				if(number_recv_bytes == 0){
					break;
				}

				//Reallocation without realloc
				buffer_ptr = buffer_ptr + number_recv_bytes;
				old_network_buffer_len = current_network_buffer_len;
				current_network_buffer_len = current_network_buffer_len + number_recv_bytes;

				char *new_network_buffer = calloc(current_network_buffer_len, sizeof(char));
				if( new_network_buffer == NULL){

					return ENMEM;
				}
				memcpy(new_network_buffer, network_buffer, old_network_buffer_len);
				
				
				if(network_buffer != NULL)
				 free(network_buffer);
				network_buffer = new_network_buffer;
				new_network_buffer = NULL;
			

			}
			
			close(socketfd);
			//insert into structure.
			ret = result_t_insert(tmp->LAct, *(main->ports + i), network_buffer );
			if( ret != EOK){
				fprintf(stderr,"Internal error\n");
				exit(ret);
			}
			buffer_ptr = 0;
			memset(network_buffer, 0, current_network_buffer_len);
		}
		//next host
		convert_pton(p, tmp);
		hostname_of_host(p, tmp->nsname);
		tmp = tmp->next;

	}
	if(network_buffer)
		free(network_buffer);
	return ENOK;
}

/**
\fn insert_port_2_sockaddr
\brief Set destination port in sockaddr
@param	struct addrinfo *address, unsigned short port
@return void 
*/
void
insert_port_2_sockaddr(struct addrinfo *address, unsigned short port){


	if( address->ai_family == AF_INET6){

		((struct sockaddr_in6 *)address->ai_addr)->sin6_port = htons(port);
	}
	else if ( address->ai_family == AF_INET){
	
		((struct sockaddr_in *)address->ai_addr)->sin_port = htons(port);
	}

}
/**
\fn connect_signal_alrm_check
\brief Connect() SIG_ALRM handler.
*/
void
connect_signal_alrm_check(){

	return;
}

/**
\fn wait_to_read
\brief Timeout for recv via select 
@param	int s, struct timeval timeout
@return int
*/
int wait_to_read(int s, struct timeval timeout)
{
   fd_set fds;
  
   /*Set descriptor and select */
   
   FD_ZERO(&fds);
   FD_SET(s, &fds);
   
   return select(sizeof(fds), &fds, NULL, NULL, &timeout);   
}

/**
\fn convert_pton
\brief Convert IP from network format to printable(human) format.
@param	struct sockaddr *sock_result,host_t *result
@return void
*/
void
convert_pton(struct addrinfo *sock_result,host_t *result){

	 switch(sock_result->ai_family){
        case AF_INET:
    		inet_ntop(AF_INET, (&((struct sockaddr_in *)sock_result->ai_addr)->sin_addr),
                    result->ipv4, INET_ADDRSTRLEN);
            break;

        case AF_INET6:
            inet_ntop(AF_INET6, (&((struct sockaddr_in6 *)sock_result->ai_addr)->sin6_addr),
                    result->ipv6, INET6_ADDRSTRLEN);
            break;

    }
  	
    return;

}


/**
\fn hostname_of_host
\brief Hostname of hosts.
@param struct addrinfo *res,char *buffer
@return void
*/
void 
hostname_of_host(struct addrinfo *res,char *buffer){

 	getnameinfo(res->ai_addr, res->ai_addrlen, buffer, NI_MAXHOST, NULL, 0, 0); 
}

/**
\fn network_error
\brief Error params.
@param int ret
@return error msg
*/
void
network_error( int ret){

	
	switch(ret){
		case ENOK:

			break;
		case ENCONNECT:
			fprintf(stderr," Error with connect. (network.h ECONNECT). Try %s -h.\n" 
				"For more information look into network.c and network.h\n", PROGNAME);
			break;
		case ENSOCKET:
			fprintf(stderr,"Error, socket() (network.h ENSOCKET). Try %s -h.\n" 
				"For more information look into network.c and network.h \n", PROGNAME);
			break;
		case ENGETADDR:
			fprintf(stderr,"Error, getaddrinfo() (network.h ENGETADDR). Try %s -h.\n" 
				"For more information look into network.c and network.h \n", PROGNAME);

			break;
		case ENLEN:
			fprintf(stderr,"Error, len of parameters (network.h ENGETADDR). Try %s -h.\n" 
				"For more information look into network.c and network.h \n", PROGNAME);

			break;

		case ENOPT:
			fprintf(stderr,"Error, options (network.h ENOPT). Try %s -h.\n" 
				"For more information look into network.c and network.h \n", PROGNAME);

			break;

		case ENMEM:
			fprintf(stderr," Error, memory allocation. (network.h ENMEM). \n" 
				"For more information look into network.c and network.h\n");
			break;
		

		default:

			break;
	}
}

