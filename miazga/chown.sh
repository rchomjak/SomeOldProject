#!/usr/bin/env bash 

#Name:		chown.sh
#Author: 	knajt, zic.spse(AT)gmail.com
#Date: 		2013
#Version: 	2013.2

#export LC_ALL = your_enviroment

VERSION="2013.2"

`true`
EXIT_SUCCESS=$?

SCRIPT_NAME=`basename $0`
ARGC=$#

##
#
#	 Functions
#	
##

print_help {

	echo " 
		${SCRIPT_NAME} script for changes owner/group folder
	options:

		[-v] - print version of program
		[-h] - print this message
		[-f] - path to file with [users,group,path](csv format)
		[-b] - base path with parameter -f [user,group](csv format)		
	examples:

		${SCRIPT_NAME}	
			#${SCRIPT_NAME} implicit works with /home directories.

		${SCRIPT_NAME} -f file.txt
		${SCRIPT_NAME} -f file.txt -b /users
"
}

enum_error {

        case $1 in

                E_OK) ret_val=$EXIT_SUCCESS;; #Everything is OK

                E_BPAR) ret_val=`expr $EXIT_SUCCESS + 1 `;; #Bad parameters

                E_FILE) ret_val=`expr $EXIT_SUCCESS + 2 `;; #Something wrong with file

        esac

        return $ret_val
}

enum_string {

        case $1 in

                E_OK) ;; #Everything is OK

                E_PHELP) print_help ;;

                E_BPAR) echo >&2  "Bad parameters.";;

                E_VER) echo "${VERSION}";; 

                E_FILE) echo >&2 "Something is wrong with file.";;
        esac
}

check_params {


	if [ $help_flag = "on" ] ; then

                if [ $file_flag = "on" ] || [ $ver_flag = "on" ] ; then

                        enum_string E_BPAR; exit $(enum_error E_BPAR)
                fi;

                enum_string E_PHELP; exit $(enum_error E_OK)
        fi;

        if [ $ver_flag = "on" ] ; then

                if  [ $file_flag = "on" ] || [ $help_flag = "on"  ]  ; then

                        enum_string E_BPAR; exit $(enum_error E_BPAR)
                fi;
        
	        enum_string E_VER; exit $(enum_error E_OK)
        fi;
	
	if [ $file_flag = "on" ] ; then

		if [ ! -f "$file_path" ] && [ ! -r "$file_path" ] ; then

			enum_string E_FILE; exit $(enum_error E_FILE)
		fi;
	fi;
	
	if [ $path_flag = "on" ] && [ $file_flag = "off" ] ; then

		enum_string E_BPAR; exit $(enum_error E_FILE)
	fi;

}

default_change_owner {

	for temp in $1 
	do
		user=`basename "$temp"`
	  	group=$user	
	
		chown -R "${user}:${group}" "${path_value}/${user}"
	done
}

find_owner_group {

	maxdepth=1
	value=`find ${path_value} -maxdepth ${maxdepth}` 
	without_find_path=` echo $value | sed s#${path_value}##`	

	default_change_owner "$without_find_path"
}

pathCSV_notspec_read_file {
	
	OLD_IFS=$IFS
	IFS=,

	while read user_csv group_csv 
	do
		chown -R "${user_csv}:${group_csv}" "${path_value}"

	done < "$file_path" 

	IFS=$OLD_IFS
}

pathCSV_spec_read_file {
	
	OLD_IFS=$IFS
	IFS=,

	while read user_csv group_csv path_csv 
	do
		chown -R "${user_csv}:${group_csv}" "${path_csv}"

	done < "$file_path" 

	IFS=$OLD_IFS
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

#pATH flag
path_flag="off"
path_value="/home"

while getopts vhf:p:  opt
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
        p)      if [ $path_flag = "on" ] ; then
                        enum_string E_BPAR ; exit $(enum_error E_BPAR)
                fi;
                path_flag="on"; path_value="$OPTARG"
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

if [ $file_flag = "off" ] && [ $path_flag = "off"  ]  ; then

	find_owner_group
elif [ $file_flag = "on" ] ; then

	pathCSV_notspec_read_file	
elif [ $path_flag = "on" ] ; then

	pathCSV_spec_read_file
fi;


exit $(enum_error E_OK)

