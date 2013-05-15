#!/usr/bin/env sh

#Name:		chmod.sh
#Author: 	knajt, zic.spse(AT)gmail.com
#Date: 		2013
#Version: 	2013.0

#export LC_ALL = your_enviroment

VERSION="2013.0"

`true`
EXIT_SUCCESS=$?

SCRIPT_NAME=`basename $0`
ARGC=$#

FIND_PATH="/home"


##
#
# Functions
#	
##

function print_help {

	echo " 
		${SCRIPT_NAME} script for changes owner/group folder
	options:

		[v] - print version of program
		[h] - print this message
		[f] - path to file with [users,group,path](csv format)
		
	examples:

		${SCRIPT_NAME}	
			#${SCRIPT_NAME} implicit works with /home directories.

		${SCRIPT_NAME} -f file.txt"
}

function enum_error {

        case $1 in

                E_OK) ret_val=$EXIT_SUCCESS;; #Everything is OK

                E_BPAR) ret_val=`expr $EXIT_SUCCESS + 1 `;; #Bad parameters

                E_FILE) ret_val=`expr $EXIT_SUCCESS + 2 `;; #Something wrong with file

        esac

        return $ret_val
}

function enum_string {

        case $1 in

                E_OK) ;; #Everything is OK

                E_PHELP) print_help ;;

                E_BPAR) echo >&2  "Bad parameters.";;

                E_VER) echo "${VERSION}";; 

                E_FILE) echo >&2 "Something is wrong with file.";;
        esac
}



function check_params {


	if [ $help_flag = "on" ] ; then

                if [ $file_flag = "on" ] ; then

                        enum_string E_BPAR; exit $(enum_error E_BPAR)


                elif [ $ver_flag = "on" ] ; then

                        enum_string E_BPAR; exit $(enum_error E_BPAR)
                fi;

                enum_string E_PHELP; exit $(enum_error E_OK)

        fi;

        if [ $ver_flag = "on" ] ; then

                if  [ $file_flag = "on" ] ; then

                        enum_string E_BPAR; exit $(enum_error E_BPAR)


                elif [ $help_flag = "on" ] ; then

                        enum_string E_BPAR; exit $(enum_error E_BPAR)
                fi;
        
	        enum_string E_VER; exit $(enum_error E_OK)
        fi;
}



function change_owner {

	for temp in $1 
	do
		user=`basename "$temp"`
	  	group=$user	
	
		chown -R "${user}:${group}" "${FIND_PATH}/${user}"
	done
}

function find_owner_group {

	maxdepth=1
	value=`find ${FIND_PATH} -maxdepth ${maxdepth}` 
	without_find_path=` echo $value | sed s#${FIND_PATH}##`	

	change_owner "$without_find_path"
}

##
#
#	GET PARAMETERS (ARGV)
#
##

#fILE flag
file_flag="off"
file_path=""

#hELP flag
help_flag="off"

#vERSION flag
ver_flag="off"


while getopts vhf:  opt
do
    case "$opt" in
        h)      if [ $help_flag = "on" ] ; then
                        enum_string E_BPAR ; exit $(enum_error E_BPAR)
                fi;
                help_flag="on";
                ;;

        f)      if [ $file_flag = "on" ] ; then
                        enum_string E_BPAR ; exit $(enum_error E_BPAR)
                fi;
                file_flag="on"; file_path="$OPTARG"
                ;;

        v)      if [ $ver_flag = "on" ] ; then
                        enum_string E_BPAR ; exit $(enum_error E_BPAR)
                fi;
                ver_flag="on"
                ;;

        \?|*)
                 enum_string E_BPAR ; exit $(enum_error E_BPAR);;
    esac
done
shift $( expr $OPTIND - 1 ) 

##
#
#	Main program
#
##
check_params
find_owner_group

exit $(enum_error E_OK)

