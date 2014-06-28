/*
IPK PROJEKT 2
ABOUT: CLIENT DNS CLIENT
Autor: Richard Chomjak xchomj00
TODO GETOPT_LONG
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
#include <getopt.h>

#define DNS_PATTERN "^(([^/?#:]*)(:([0-9]{1,5})))$"
#define BUFFER_RECV 200*2
#define PARAM_BUFFER 2
#define ERROR_SIZE  
#define IPV4_SIZE 32
typedef struct server_Reply{
  char ipv4_ipv6[BUFFER_RECV];
 
}Tserver_Reply;

/**
* Struktura pre DNS
**/
typedef struct parse_DNS{
  char *host_proto;
  char *host;
  char *protocol_num;
  char *server_address;
} Tparse_DNS;

/**  
* Struktura na argumenty
**/
typedef struct global_arg {
  int IPv4_flag;
  int IPv6_flag;
  char input_opt[PARAM_BUFFER];
  char *address;
  char *dns_address;
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

};
static struct option longopts[]={
  { "4", no_argument, NULL, '4'},
  { "6", no_argument, NULL, '6'}
  
};



int parse_Opt(Tglobal_arg *argums,int argc, char** argv);
void print_Help();
int parse_DNS(Tparse_DNS *dns, char *address_ns);
int network_operation(Tparse_DNS *uri,struct addrinfo *hints,struct sockaddr *ipv4,Tserver_Reply *msg);
int free_parse_DNS(Tparse_DNS *dns);
void my_free(void *mem);
void print_error(int ret);
void print_result(Tserver_Reply *msg,Tglobal_arg *argums);


void print_result(Tserver_Reply *msg,Tglobal_arg *argums){
 int i=0;
  for(i=0;i<PARAM_BUFFER;i++){
    switch(argums->input_opt[i]){
      case '4':
	      if(msg->ipv4_ipv6[0]=='Z'){
		fprintf(stderr,"%s\n",msg->ipv4_ipv6+1);
	      }else{
		printf("%s\n",msg->ipv4_ipv6);
	      }
		break;
      case  '6':
		if(msg->ipv4_ipv6[strlen(msg->ipv4_ipv6)+1]=='Z'){
		    fprintf(stderr,"%s\n",msg->ipv4_ipv6+2+strlen(msg->ipv4_ipv6));
		}else{
		   printf("%s\n",msg->ipv4_ipv6+strlen(msg->ipv4_ipv6)+1);
		}
		  break;
      default:
	break;
    }
  }
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
   case EDNS:
     fprintf(stderr,"Error: DNS error\n");
	  break;
   case EGETADDR:
      fprintf(stderr,"Error: GET Addrinfo error\n");
	  break;
     case ECONNECT:
    fprintf(stderr,"Error: Connection error\n");
	  break;
   case ERECV:
      fprintf(stderr,"Error: RECV error\n");
	  break;
}
}

void my_free(void *mem){
  if(mem!=NULL){
   free(mem); 
  }
}
int free_parse_DNS(Tparse_DNS *dns){
  
  my_free(dns->host);
  dns->host=NULL;
  my_free(dns->host_proto);
  dns->host_proto=NULL;
  my_free(dns->protocol_num);
  dns->protocol_num=NULL;
  my_free(dns->server_address);
  dns->server_address=NULL;
  return EOK;
}

/**
* Vypis help hlasky
**/
void print_Help(){
 printf("Usage: ./client server:port [-4|-6] hostname\n\n"
 "-4 IPv4 address\n-6 IPv6 address\n"); 
}
/**
* Parsovanie argumentov
**/
int parse_Opt(Tglobal_arg *argums,int argc, char** argv){
  
  int c=-1;
  int ch, fd;
  int i=0;
  argums->address=argv[1];
  for(;i<PARAM_BUFFER;i++){
      argums->input_opt[i]=0;
  }
   while ((ch = getopt_long(argc, argv, "46",longopts,NULL)) != -1) {
           
	    switch (ch) {
             case '4':		
			if(argums->IPv4_flag==0){
			  c++;	  
			  argums->IPv4_flag=1;
			  argums->input_opt[c]='4';

			} break;
             case '6':
		      if(argums->IPv6_flag==0){
			c++;
		         argums->IPv6_flag=1;
		         argums->input_opt[c]='6';      
		      }
                     break;
	
	     case 'h':
		      print_Help();
		      return EOPT;;
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
  
     if(argc!=2) {
      
       print_Help();
       return EOPT;
     }
     else {
       if(!(argums->IPv4_flag || argums->IPv6_flag))
	 return EOPT;
	  argums->address=calloc(sizeof(char),strlen(argv[0])+1);
	  strcpy(argums->address,argv[0]);
	  argums->dns_address=calloc(sizeof(char),strlen(argv[1])+1);
	  strcpy(argums->dns_address,argv[1]); 
    }
    
     return EOK;
}

/**
* Parsovanie vstupu
**/
int parse_DNS(Tparse_DNS *dns,char *address_ns){
  
 
  regmatch_t DNS_pmatch[5];
  regex_t re_DNS;
 
    int status_DNS=0;
      
  if(regcomp( &re_DNS,DNS_PATTERN, REG_EXTENDED)!= 0) {
    return EREGEXP;
  }
     
  status_DNS=regexec(&re_DNS,address_ns,5,DNS_pmatch,0);
 
  if(!status_DNS){
    /* Ulozenie Host a protokolu */  
     dns->host_proto= (char *)calloc((DNS_pmatch[1].rm_eo-DNS_pmatch[1].rm_so+1),sizeof(char));
      if(dns->host_proto==NULL){
	return EMEM;
      }
      /* Ulozenie host a protokolu*/
       strncpy(dns->host_proto,address_ns+DNS_pmatch[1].rm_so,DNS_pmatch[1].rm_eo-DNS_pmatch[1].rm_so);
     /* Ulozenie Host */  
     dns->host= (char *)calloc((DNS_pmatch[2].rm_eo-DNS_pmatch[2].rm_so+1),sizeof(char));
      if(dns->host==NULL){
	return EMEM;
      }
      /* Ulozenie host */
       strncpy(dns->host,address_ns+DNS_pmatch[2].rm_so,DNS_pmatch[2].rm_eo-DNS_pmatch[2].rm_so);
     /* Ulozenie portu */
     dns->protocol_num= (char *)calloc((DNS_pmatch[4].rm_eo-DNS_pmatch[4].rm_so+1),sizeof(char));
      if(dns->protocol_num==NULL){
	return EMEM;
      }
      /* Ulozenie cisla protokolu*/
       strncpy(dns->protocol_num,address_ns+DNS_pmatch[4].rm_so,DNS_pmatch[4].rm_eo-DNS_pmatch[4].rm_so);
      
       
}else if(status_DNS){ 
    regfree(&re_DNS);
    return EDNS;
 }
 
 /*Uvolnenie pamate */
regfree(&re_DNS);
 return EOK;
}

int network_operation(Tparse_DNS *dns, struct addrinfo *hints,struct sockaddr *ipv,Tserver_Reply *msg){

  
  int socketfd=0, numbytes=0, ret=0;
  
  struct addrinfo *servinfo, *p;
  
  bzero(hints,sizeof(*hints));
  hints->ai_family=AF_INET;
  hints->ai_socktype = SOCK_STREAM;
  hints->ai_flags=AI_PASSIVE;	
  if((ret= getaddrinfo(dns->host,dns->protocol_num, hints, &servinfo)) != 0) {
    return EGETADDR ; 
  }

  for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((socketfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            continue;
        }
        if (connect(socketfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(socketfd);  
						continue;
        }
        break;
    }
  
  if(p==NULL){
   freeaddrinfo(servinfo);
   return ECONNECT; 
  }
  /* 2 = \n\n */
  char sprintf_buf[strlen(dns->server_address)];
  sprintf(sprintf_buf,"%s",dns->server_address);
  send(socketfd,sprintf_buf,strlen(sprintf_buf),0);
  
  numbytes = recv(socketfd, msg->ipv4_ipv6, 2*BUFFER_RECV-1, 0);

  
 
 if(numbytes==-1){
  freeaddrinfo(servinfo);
  close(socketfd);
  return ERECV;
}


 freeaddrinfo(servinfo);
  close(socketfd);
  return EOK;
}


int main(int argc, char** argv){

/*Navratova adresa*/
 int ret=EOK;
/*Inicializovanie argumentov*/  
    Tglobal_arg argum;
  
  argum.IPv4_flag=0;
  argum.IPv6_flag=0;
  argum.address=NULL;
  argum.dns_address=NULL;
  
 /*Struktura na spracovanie adresy */
    Tparse_DNS DNS_parse;
  DNS_parse.host_proto=NULL;
  DNS_parse.host=NULL;
  DNS_parse.protocol_num=NULL;
  DNS_parse.server_address=NULL;
  /*Struktury na zistenie adresy a dalsich informacii*/
 
    struct addrinfo hints;
    struct sockaddr ipv;
    Tserver_Reply msg;
 
    bzero(&msg.ipv4_ipv6,sizeof(msg.ipv4_ipv6));
   
    
 ret=parse_Opt(&argum,argc,argv);
 if(ret!=EOK){
  print_error(ret);
  free_parse_DNS(&DNS_parse);
  return ret;
 }
 /*Skopirovanie adresy do inej struktury*/
 DNS_parse.server_address=calloc(sizeof(char),strlen(argum.dns_address)+1);
 strcpy(DNS_parse.server_address,argum.dns_address);
 
 ret=parse_DNS(&DNS_parse,argum.address); 
 if(ret!=EOK){
   print_error(ret);
   my_free(argum.address);
   my_free(argum.dns_address);
   free_parse_DNS(&DNS_parse);
   return ret;
 }
 
 ret=network_operation(&DNS_parse,&hints,&ipv,&msg);
  if(ret!=EOK){
    print_error(ret);
    my_free(argum.address);
    my_free(argum.dns_address);
    free_parse_DNS(&DNS_parse);
    return ret;
  }
    print_result(&msg,&argum);
    free_parse_DNS(&DNS_parse);
    my_free(argum.address);
    my_free(argum.dns_address);
return EOK;

}
