/**
@author Richard Chomjak
@file 	print.c
@brief  Print results.
@date 	11/10/2012
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include "arpa/inet.h"
#include "print.h"

/**
\fn print_results
\brief Prints results.
@param	nodemain_t *main
@return void 
*/
void
print_results(nodemain_t *main){

	
	struct host *tmp_host=NULL;

	tmp_host = main->LFirst;

	

	struct result *tmp_result = NULL;
	struct servent *appl_name = NULL;
	
	//via host
	while(tmp_host != NULL) {


		fprintf(stdout,"Host address from user: %s \n",tmp_host->input_string);
		if(tmp_host->exists_host){
			
			fprintf(stdout,"\t Remote address: \n\t\t IPv4: %s IPv6: %s \n \t\t Hostname: %s \n\n", tmp_host->ipv4, tmp_host->ipv6, tmp_host->nsname);
			
			
			tmp_result = tmp_host->LFirst;
			//via results
			while( tmp_result != NULL){

				fprintf(stdout,"\t Port: %u ",tmp_result->port);
				if(main->service_name){
					appl_name = getservbyport(htons(tmp_result->port), NULL);
					if(!appl_name )
						fprintf(stdout,"Application: unknown ");
					else
						fprintf(stdout,"Application: %s",appl_name->s_name);
				}
				fprintf(stdout,"\n");
				fprintf(stdout,"\t Banner: %s \n\n",tmp_result->banner);


				tmp_result=tmp_result->next;
			}	

		}else{
			fprintf(stdout,"\t Host does not exist.\n\n");

		}
		
		tmp_host = tmp_host->next;
	}

}

