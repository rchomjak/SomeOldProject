/**
@author knajt
@file 	errors.h
@brief  List of errors.
@date 	26/9/2012
*/
#ifndef __ERRORS_H_
#define __ERRORS_H_

#include <stdlib.h>
/**
	\enum errors
	\brief Global errros for tcpsearch.

*/

enum errors{
	
	EOK=EXIT_SUCCESS,			 	
	EARGV,				
	EARGC,
	EMEM
	
	


};
#endif
