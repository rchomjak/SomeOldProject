/**
@author Richard Chomjak
@file 	file_read_parser.c
@brief  Parse and controll port range.
@date 	4/10/2012
*/


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "file_read_parser.h"
#include "tcpsearch.h"
#include "errors.h"



/*
\fn file_read
\brief Reading input file and pushing data into structure.
@param  FILE *fp, nodemain_t *main, void *stdin
@return enum code
*/
int 
file_read( FILE *fp, nodemain_t *main, void *stdin){

	int byte_value = 0;
	int fprevious_state = Fstart;
	
	int ret = EPfOK;

	size_t buffer_size  = 1;
	size_t buffer_value = 0;
	char *buff=NULL;

	if ( fp == NULL && stdin == NULL){

		return EPfFILE;
	}

	if( stdin != NULL){

		fp = stdin;
	}

	buff = calloc(buffer_size, sizeof(char));
	if(buff == NULL){

		return EPfMEM;
	}

	while( (byte_value = fgetc(fp)) != EOF ){

		

		 if( (isalnum(byte_value) || byte_value == ':' || byte_value == '.') && (fprevious_state != Fignore)){
				
				
		 		fprevious_state = Fget;
				ret = file_buffer_push(&buff, byte_value, &buffer_size, &buffer_value);
				if( ret != EPfOK){

					return ret;
				}
				
		}else if( byte_value == '\n'){

			if( fprevious_state == Fget){

				// insert into structure
				if ( buffer_value != 0){
					
					ret = nodemain_t_create_and_cpy_str(main, buff);
					if( ret != EOK){
						tcpsearch_error(ret);

						return ret;
					}

				}
				else {
					free(buff);
					return EPfOPT;
				}
					
				
				memset(buff, '\0', buffer_size );
				buffer_value = 0;
			}

			fprevious_state = Fnewline;

		}
		else if( !isalnum(byte_value) || byte_value != ':' || byte_value != '.' ){

			if( fprevious_state == Fget){

				// insert into structure
				if ( buffer_value != 0){
					
					ret = nodemain_t_create_and_cpy_str(main, buff);
					if( ret != EOK){
						//NODEMAIN ERROR

						return ret;
					}

				}
				else {
					free(buff);
					return EPfOPT;
				}
				
				memset(buff, '\0', buffer_size);
				buffer_value = 0;
			}

			fprevious_state = Fignore;

		}	
		
	}

	if ( buffer_value != 0){

		// insert into structure
		ret = nodemain_t_create_and_cpy_str(main, buff);
		if( ret != EOK){
			tcpsearch_error(ret);
			
			return ret;
		}
			 
	}

	free(buff);

return EPfOK;
}

/*
\fn file_buffer_push
\brief Append value into array.
@param  char *buffer, char value, size_t buff_len
@return enum_code
*/
int
file_buffer_push(char  **buffer, int value, size_t *buff_len, size_t *buff_ptr){

char *help = NULL;

	if ( buffer == NULL){

		return EPfMEM;
	}
	else {
		
			if ( *buff_ptr + 1 >= *buff_len ){
			
				char *tmpptr=NULL;
				
				*buff_len += 1;	
				tmpptr = realloc(*buffer, *buff_len );
				if (tmpptr != NULL){

					*buffer = tmpptr;

				}
				else {
					return EPfMEM;
				}
				
			}
			
			/** Because array != pointer */
			help = *buffer + *buff_ptr ;
			*help = value;
			*buff_ptr += 1;	
			help = *buffer + *buff_ptr ;
			*help = '\0';
			help = NULL;
		
		}
	
	return EPfOK;
}


/*
\fn file_open
\brief Open file.
@param  char *filename
@return FILE *
*/
FILE *
file_open(char *filename){

	FILE *fp = NULL;
	if( strlen(filename) == 1 && filename[0] == '-'){

	}
	else{
			fp =fopen(filename,"r");
	}
	return fp;
}

/*
\fn file_close
\brief Close file.
@param  FILE *fp
@return void
*/
void
file_fclose(FILE *fp){

	if(fp != NULL)
		fclose(fp);

	return;
}

/**
\fn file_error
\brief Error params.
@messages int ret
@return error msg
*/
void
file_error( int ret){

	
	switch(ret){
		case EPfOK:

			break;
		case EPfLEN:
			fprintf(stderr," Error with len of input arguments. (file_read_parser.h EPfLEN). Try %s -h.\n" 
				"For more information look into file_read_parser.c and file_read_parser.h \n", PROGNAME);
			break;
		case EPfOPT:
			fprintf(stderr,"Error, bad input arguments. (file_read_parser.h EPfOPT). Try %s -h.\n" 
				"For more information look into file_read_parser.c and file_read_parser.h \n", PROGNAME);
			break;
		case EPfMEM:
			fprintf(stderr," Error, memory allocation . (file_read_parser.h EPfMEM). \n" 
				"For more information look into file_read_parser.c and file_read_parser.h\n");
			break;
		case EPfFILE:
			fprintf(stderr," Error, with file. (file_read_parser.h EPfFILE). \n" 
				"For more information look into file_read_parser.c and file_read_parser.h \n");
			break;



		default:

			break;
	}
	

	return;
}
