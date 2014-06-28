/*
IPK PROJEKT 2
ABOUT: SERVER, AAAA/A preklad
Autor: Richard Chomjak xchomj00
*/
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <locale.h>
#include <regex.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
#include <netinet/in.h>
#define BUFFER_RECV 200
#define NUMBER_OF_CLIENTS 20

/**
* Struktura pre URI
**/
typedef struct parse_Server{
  char *port;
  char host[BUFFER_RECV];
} Tparse_Server;

/**  
* Struktura na argumenty
**/
typedef struct global_arg {
 int p_flag;  
 char *port;
 char input_opt[1];
} Tglobal_arg;
/**
* Enum pre chyby
**/
enum errors{
  
  EOK=EXIT_SUCCESS,
  EMEM,
  EOPT,
  EREGEXP,
  EDNS,
  EGETADDR,
  ECONNECT,
  ERECV,
  ESETSOCK,
  EBIND,
  ELISTEN
};
int parse_Opt(Tglobal_arg *argums,int argc, char** argv);
void print_Help();
int network_operation(Tparse_Server *server,struct addrinfo *hints,struct sockaddr *ipv4);
void my_free(void *mem);
void print_error(int ret);
void term_handler(int n);
void term_handler(int n){
  
while(waitpid(-1,NULL,WNOHANG)>0);

exit(0);
}
void print_error(ret){
  
 switch(ret){
   case EMEM:
     fprintf(stderr,"Error: Alocation error\n");
	  break;
   case EOPT:
     fprintf(stderr,"Error: Input option error\n");
	  break;	  
   case EREGEXP:
     fprintf(stderr,"Error: Regular expression error\n");
	  break;	  
   
   case EGETADDR:
      fprintf(stderr,"Error: GET Addrinfo error\n");
    break;
     case ECONNECT:
      fprintf(stderr,"Error: Connection error\n");
    break;
    case ESETSOCK:
      fprintf(stderr,"Error: Set socket error\n");
    break; 
	
    case EBIND:
      fprintf(stderr,"Error: Bind error\n");
    break; 
    case ELISTEN:
     fprintf(stderr,"Error: Listen error\n");
    break;
}

}
/**
 * Uvolnenie pamate
 **/
void my_free(void *mem){
 if(mem !=NULL) 
  free(mem);
  
}

/**
* Vypis help hlasky
**/
void print_Help(){
 printf("Usage: ./server -p PORT\n\n"
 "-p port for listening on this PORT :) \n\n"); 
}




/**
* Parsovanie argumentov
**/
int parse_Opt(Tglobal_arg *argums,int argc, char** argv){
  
  int c=-1;
  int ch, fd;
  int i=0;
  for(;i<1;i++){
      argums->input_opt[i]=0;
  }
   while ((ch = getopt(argc, argv, "p")) != -1) {
           
	    switch (ch) {
             case 'p':
			if(argums->p_flag==0){
			  c++;	  
			  argums->p_flag=1;
			  argums->input_opt[c]='p';

			} break;
	     case 'h': 	
			print_Help();
			break;
           
	     case '?':
			print_Help();
			return EOPT;;
			break;
	     default:
			print_Help();
			return EOPT;
	       break;
             }
     }
     argc -= optind;
     argv += optind;
  
     if(argc!=1) {
       print_Help();
       return EOPT;
     }
     else {
        argums->port=calloc(strlen(argv[0])+1,sizeof(char));
	strcpy(argums->port,argv[0]);
       
    } 
     return EOK;
}

/**
* Parsovanie vstupu
**/

int network_operation(Tparse_Server *server, struct addrinfo *hints,struct sockaddr *ipv){

  signal(SIGTERM,term_handler);
  int socketfd=0, numbytes=0, ret=0;
  int client_fd=0;
  int yes=1;
 
 
  struct addrinfo *servinfo, *p;
  struct sockaddr_storage client_address;
  struct addrinfo client_hints,*client_info,*client_p;
  socklen_t sin_size;
 
  int is_ipv4=0;
  int is_ipv6=0;
  
  bzero(&client_hints,sizeof(client_hints));
  client_hints.ai_family=AF_UNSPEC;
  client_hints.ai_socktype=SOCK_STREAM;
  client_hints.ai_flags=AI_PASSIVE;
  
  bzero(hints,sizeof(*hints));
  hints->ai_family=AF_INET;
  hints->ai_socktype = SOCK_STREAM;
  hints->ai_flags=AI_PASSIVE;
 
  

  if((ret= getaddrinfo(NULL,server->port, hints, &servinfo)) != 0) {
    return EGETADDR; 
  }
  
  for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((socketfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            continue;
        }
	if( setsockopt(socketfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==-1){
	  
	return ESETSOCK; 
	}
	if (bind(socketfd,p->ai_addr,p->ai_addrlen)==1){
	  
	return EBIND; 
	}
  break;
  }

      if(p==NULL){
   freeaddrinfo(servinfo);
   return ECONNECT; 
  }
  
  
  freeaddrinfo(servinfo);
  
  
  if(listen(socketfd,NUMBER_OF_CLIENTS)==-1){
    
   return ELISTEN; 
  }
  
  while(1){
  
   
   sin_size=sizeof client_address;
   
   client_fd=accept(socketfd,(struct sockaddr *)&client_address,&sin_size);
   if(client_fd==-1){
    continue;
   }
   
   
   if(!fork()){
    close(socketfd);
     
      
     char sprintf_buf4[BUFFER_RECV];
     char sprintf_buf6[BUFFER_RECV];
     char ipv4_ipv6[3*BUFFER_RECV];
     bzero(sprintf_buf4,sizeof(sprintf_buf4));
     bzero(sprintf_buf6,sizeof(sprintf_buf6));
     bzero(ipv4_ipv6,sizeof(ipv4_ipv6));
  
   numbytes = recv(client_fd, server->host, BUFFER_RECV-1, 0);
   getaddrinfo(server->host,NULL,&client_hints,&client_info);
    
    void *address4=NULL;
    void *address6=NULL;
    for(client_p=client_info;client_p!=NULL;client_p=client_p->ai_next){     
      if(client_p->ai_family==AF_INET && !is_ipv4){
	is_ipv4=1;
	struct sockaddr_in *ipv4=(struct sockaddr_in *)client_p->ai_addr;
	address4=&(ipv4->sin_addr);
	
	inet_ntop(client_p->ai_family,address4,sprintf_buf4,sizeof(sprintf_buf4));
      }else if(client_p->ai_family==AF_INET6 && !is_ipv6){
	is_ipv6=1;
	struct sockaddr_in6 *ipv6=(struct sockaddr_in6 *)client_p->ai_addr;
	address6=&(ipv6->sin6_addr);
	inet_ntop(client_p->ai_family,address6,sprintf_buf6,sizeof(sprintf_buf6)); 
      }
      
      if(is_ipv4 && is_ipv6)
	break;
    }
   if(!is_ipv4){
    sprintf(sprintf_buf4,"ZErr4: Nenalezeno.");
 }
  if(!is_ipv6){
    sprintf(sprintf_buf6,"ZErr6: Nenalezeno.");
  }
    sprintf(ipv4_ipv6,"%s%c%s",sprintf_buf4,'\0',sprintf_buf6);
    send(client_fd,ipv4_ipv6,strlen(sprintf_buf4)+strlen(sprintf_buf6)+2,0);

freeaddrinfo(client_info);
 close(client_fd);	
  return EOK;
  }
 close(client_fd); 
  }
  return EOK;

}


int main(int argc, char** argv){
setbuf(stdout,NULL);
 
/*Navratova hodnota*/
 int ret=0;

    Tglobal_arg argum;
  
  argum.p_flag=0;
  argum.port=NULL;
  
    Tparse_Server server_parse;

    server_parse.port=NULL;
    bzero(server_parse.host,sizeof(server_parse.host));
  
 
    struct addrinfo hints;
    struct sockaddr ipv;
    
    
 ret=parse_Opt(&argum,argc,argv);
 if(ret!=EOK){
    print_error(ret);
    return ret;
 }
  
  server_parse.port=calloc(strlen(argum.port)+1,sizeof(argum.port));
  strcpy(server_parse.port,argum.port);
  
  
 ret=network_operation(&server_parse,&hints,&ipv);
  if(ret!=EOK){
    print_error(ret);
    my_free(argum.port);
    my_free(server_parse.port);
    return ret;
  }
  
 
 my_free(argum.port);
 my_free(server_parse.port);
  
  
return EOK;

}
