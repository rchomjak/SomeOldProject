/*
 *   About: Project for subject IOS, barber shop problem.
 *  Author: Richard Chomjak (xchomj00).
 * Contact: xchomj00{AT}stud.fit.vutbr.cz , knight{AT}fantomas.sk
 *    Date: 2011
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
/* strtoumax()  */
#include <inttypes.h>
/* srand() */
#include <time.h>

#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <signal.h>

#define USLEEPMULTIPLICATION 1000

/* Structure for arguments */

typedef struct {

    unsigned int free_seats;
    unsigned int T_barber;
    unsigned int T_generate;
    unsigned int customer;
    char* name_of_file;

}S_arguments;


/* Structure for sharing data between semaphores  */

typedef struct {

    /* Semaphores for manage process in critical space */

    sem_t waiting_room;
    sem_t barber_room;
    sem_t access_seat;
    sem_t barber_ready;
    sem_t barber_finished;

    sem_t customer_ready;

    sem_t barber_end;
    sem_t ordinary_num;

    /* Variables for sharing in shared mem segment [IPC]  */

    /* Number of all generate customers  */
    unsigned int number_of_customers;
    /* Ordinal number of process */
    unsigned  long  actualy_action;
    /* Number of seats in waiting_room  */
    unsigned int free_seats;
    unsigned int all_free_seats;

    unsigned int all_customer_process;

    /* Number of created customer */
    unsigned  int created_customer;
    /* Time for barber serving */
    unsigned  int T_barber_serve;
    /* Time for creating customer */
    unsigned  int T_customer_create;

    /* PID of barber */
    pid_t barber_PID;

    /*  id of last customer */
    unsigned int last_customer;

    /* last customer for term barber */
    unsigned int kill_customer;

    char* name_of_file;

    FILE* file;


} S_share_data;

/* Prototypes */
void customer_func ( S_share_data *customer, int ordinaly_customer );

unsigned int rand_time ( unsigned int max );

void print_action ( short int ordinaly_num,unsigned int ordinaly_customer, S_share_data *print_action );

int create_customer_process ( S_share_data *customer,pid_t *customer_PIDs );

void customer_func ( S_share_data *customer, int ordinaly_customer );

void barber_func ( S_share_data *barber );

int create_barber_process ( S_share_data *barber );

int semaphore_init ( S_share_data *init_sem,int *sem_init );

int semaphore_deinit ( S_share_data *deinit_sem );

void remove_garbage ( S_share_data *remove_mem, int rem_segment_id,int *sem_init_array ,int main_PID,pid_t *customer_PIDs, int rem_segment_proc_id);

void init_var_shared_mem ( S_arguments *share_arg ,S_share_data *variables );

int open_file ( S_share_data *open_file );

int close_file ( S_share_data *close_file );


void print_action ( short ordinaly_num, unsigned int ordinaly_customer, S_share_data *print_action ) {

    if ( ( strcmp ( print_action->name_of_file,"-" ) ) !=0 )
        setbuf ( print_action->file,NULL );

    /* Customer print msg */
    switch ( ordinaly_num ) {
    case ( 1 ) : {
        if ( ( strcmp ( print_action->name_of_file,"-" ) ) ==0 ) {

            fprintf ( stdout,"%lu: customer %u: created \n",print_action->actualy_action+=1,print_action->created_customer );
            break;
        }
        fprintf ( print_action->file,"%lu: customer %u: created\n",print_action->actualy_action+=1,print_action->created_customer );
        break;
    }
    case ( 2 ) : {
        if ( ( strcmp ( print_action->name_of_file,"-" ) ) ==0 ) {

            fprintf ( stdout,"%lu: customer %u: enters\n",print_action->actualy_action+=1,ordinaly_customer );
            break;
        }

        fprintf ( print_action->file,"%lu: customer %u: enters\n",print_action->actualy_action+=1,ordinaly_customer );
        break;
    }
    case ( 3 ) : {
        if ( ( strcmp ( print_action->name_of_file,"-" ) ) ==0 ) {

            fprintf ( stdout,"%lu: customer %u: ready\n",print_action->actualy_action+=1,ordinaly_customer );
            break;
        }

        fprintf ( print_action->file,"%lu: customer %u: ready\n",print_action->actualy_action+=1,ordinaly_customer );
        break;
    }
    case ( 4 ) : {
        if ( ( strcmp ( print_action->name_of_file,"-" ) ) ==0 ) {
            fprintf ( stdout,"%lu: customer %u: served \n",print_action->actualy_action+=1,ordinaly_customer );
            break;
        }

        fprintf ( print_action->file,"%lu: customer %u: served \n",print_action->actualy_action+=1,ordinaly_customer );
        break;
    }
    case ( 5 ) : {
        if ( ( strcmp ( print_action->name_of_file,"-" ) ) ==0 ) {
            fprintf ( stdout,"%lu: customer %u: refused\n",print_action->actualy_action+=1,ordinaly_customer );
            break;
        }

        fprintf ( print_action->file,"%lu: customer %u: refused\n",print_action->actualy_action+=1,ordinaly_customer );
        break;
    }
    /* Barber print msg */
    case ( 6 ) : {
        if ( ( strcmp ( print_action->name_of_file,"-" ) ) ==0 ) {
            fprintf ( stdout,"%lu: barber: checks\n",print_action->actualy_action+=1 );
            break;
        }
        fprintf ( print_action->file,"%lu: barber: checks\n",print_action->actualy_action+=1 );
        break;
    }
    case ( 7 ) : {
        if ( ( strcmp ( print_action->name_of_file,"-" ) ) ==0 ) {
            fprintf ( stdout,"%lu: barber: ready\n",print_action->actualy_action+=1 );
            break;
        }

        fprintf ( print_action->file,"%lu: barber: ready\n",print_action->actualy_action+=1 );
        break;
    }

    case ( 8 ) : {
        if ( ( strcmp ( print_action->name_of_file,"-" ) ) ==0 ) {
            fprintf ( stdout,"%lu: barber: finished\n",print_action->actualy_action+=1 );
            break;
        }

        fprintf ( print_action->file,"%lu: barber: finished\n",print_action->actualy_action+=1 );
        break;
    }
    }

    return;
}


void customer_func ( S_share_data *customer, int ordinaly_customer ) {

    sem_wait ( &customer->ordinary_num );
    print_action ( 1,0,customer );

    sem_post ( &customer->ordinary_num );

    /* lock access_seat */
    sem_wait ( &customer->access_seat );

    sem_wait ( &customer->ordinary_num );
    print_action ( 2,ordinaly_customer,customer );

    sem_post ( &customer->ordinary_num );
    if ( customer->free_seats > 0 ) {

        customer->free_seats--;
        /* unlock waiting_room */
        sem_post ( &customer->waiting_room );

        /* unlock access_seat */
        sem_post ( &customer->access_seat );

        /* lock barber room */

        sem_post ( &customer->barber_room );

        sem_wait ( &customer->barber_ready );
        sem_wait ( &customer->ordinary_num );
        print_action ( 3,ordinaly_customer,customer );

        sem_post ( &customer->ordinary_num );
        sem_post ( &customer->customer_ready );

        sem_wait ( &customer->barber_finished );
        sem_wait ( &customer->ordinary_num );

        print_action ( 4,ordinaly_customer,customer );

        sem_post ( &customer->ordinary_num );

    } else {
        sem_post ( &customer->access_seat );

        sem_wait ( &customer->ordinary_num );
        print_action ( 5,ordinaly_customer,customer );

        sem_post ( &customer->ordinary_num );
    }

    sem_wait ( &customer->barber_end );
    
		customer->kill_customer++;

    if ( customer->kill_customer== ( customer->last_customer  ) ) {
				kill ( customer->barber_PID,SIGKILL );
		}
    

		sem_post ( &customer->barber_end );

    return;
}

unsigned int rand_time ( unsigned int max ) {

    srand ( ( unsigned ) time ( NULL ) );
    return ( rand() % ( max+1 ) ) *USLEEPMULTIPLICATION;
}

/* Generating new customer process with fork()  */
int create_customer_process ( S_share_data *customer,pid_t *customer_PIDs ) {

    pid_t custom_PID;

    unsigned int a=0;
    for ( a=0; a<customer->number_of_customers;a++ ) {

        /* Generate time with random time in miliseconds */
        if ( customer->T_customer_create!=0 ) {
            usleep ( rand_time ( customer->T_customer_create ) );
        }
        custom_PID=fork();

        if ( custom_PID < 0 ) {

            return 1;
        } else if ( custom_PID == 0 ) {
            customer_PIDs[a]=getpid();
            customer->created_customer+=1;
            customer_func ( customer,customer->created_customer );
            exit ( 0 );
        }
    }

    return custom_PID;
}
void barber_func ( S_share_data *barber ) {


    while ( 1 ) {

        sem_wait ( &barber->ordinary_num );
        print_action ( 6,0,barber );
        sem_post ( &barber->ordinary_num );

        sem_wait ( &barber->waiting_room );
        sem_wait ( &barber->access_seat );

        barber->free_seats++;

        sem_post ( &barber->barber_room );

        sem_wait ( &barber->ordinary_num );
        print_action ( 7,0,barber );
        sem_post ( &barber->ordinary_num );

        sem_post ( &barber->barber_ready );
        sem_post ( &barber->access_seat );

        if ( barber->T_barber_serve!=0 ) {
            usleep ( rand_time ( barber->T_barber_serve ) );
        }

        sem_wait ( &barber->customer_ready );
        sem_wait ( &barber->ordinary_num );
        print_action ( 8,0,barber );
        sem_post ( &barber->ordinary_num );
        sem_post ( &barber->barber_finished );
	 }
    return;

}
/* Generating barber  */
int create_barber_process ( S_share_data *barber ) {

    int barber_PID = fork();

    if ( barber_PID < 0 ) {

        fprintf ( stderr,"Error, creating barber with fork()" );

        return 1;
    } else if ( barber_PID ==0 ) {

        barber_func ( barber );
        /* End process */
        exit ( 0 );
    } else
        return barber_PID;
}


/* Converstion argument  */
int argument ( int argc,char **argv,S_arguments *mem_init ) {

    /*Number of arguments */
    if ( argc!= 6 ) {

        return 1;
    }
    /* Converstion string to integer */
    if ( isdigit ( *argv[1] ) ) {
        mem_init->free_seats=strtoumax ( argv[1],NULL,0 );

        if ( mem_init->free_seats==UINT_MAX ) {

            return 2;
        }
    } else {

        return 1;
    }

    if ( isdigit ( *argv[2] ) ) {
        mem_init->T_generate=strtoumax ( argv[2],NULL,0 );

        if ( mem_init->T_generate==UINT_MAX ) {

            return 2;
        }
    } else {

        return 1;
    }

    if ( isdigit ( *argv[3] ) ) {
        mem_init->T_barber=strtoumax ( argv[3],NULL,0 );

        if ( mem_init->T_barber==UINT_MAX ) {

            return 2;
        }
    } else {

        return 1;
    }
    if ( isdigit ( *argv[4] ) ) {
        mem_init->customer=strtoumax ( argv[4],NULL,0 );
        if ( mem_init->customer==UINT_MAX ) {

            return 2;
        }

    } else {

        return 1;
    }

    mem_init->name_of_file=argv[5];

    return 0;
}

/* Initializing semaphore */
int semaphore_init ( S_share_data *init_sem,int *sem_init_array ) {

    if ( sem_init ( &init_sem->barber_room,1,0 ) < 0 ) {

        return 1;
    }
    sem_init_array[0]=1;
    if ( sem_init ( &init_sem->waiting_room,1,0 ) < 0 ) {
        return 1;
    }
    sem_init_array[1]=1;
    if ( sem_init ( &init_sem->access_seat,1,1 ) <0 ) {
        return 1;
    }
    sem_init_array[2]=1;
    if ( sem_init ( &init_sem->barber_ready,1,0 ) <0 ) {
        return 1;
    }
    sem_init_array[3]=1;
    if ( sem_init ( &init_sem->barber_finished,1,0 ) <0 ) {
        return 1;
    }
    sem_init_array[4]=1;

    if ( sem_init ( &init_sem->customer_ready,1,0 ) <0 ) {
        return 1;
    }
    sem_init_array[5]=1;

    if ( sem_init ( &init_sem->ordinary_num,1,1 ) <0 ) {
        return 1;
    }
    sem_init_array[6]=1;

    if ( sem_init ( &init_sem->barber_end,0,1 ) <0 ) {

        return 1;
    }
    sem_init_array[7]=1;


    return 0;
}
/* Deinitializing semaphores */
int semaphore_deinit ( S_share_data *deinit_sem ) {

    if ( sem_destroy ( &deinit_sem->barber_room ) <0 ) {

        return 1;
    }

    if ( sem_destroy ( &deinit_sem->waiting_room ) <0 ) {

        return 1;
    }
    if ( sem_destroy ( &deinit_sem->access_seat ) <0 ) {

        return 1;
    }

    if ( sem_destroy ( &deinit_sem->barber_ready ) <0 ) {

        return 1;
    }

    if ( sem_destroy ( &deinit_sem->barber_finished ) <0 ) {

        return 1;
    }


    if ( sem_destroy ( &deinit_sem->customer_ready ) <0 ) {

        return 1;
    }

    if ( sem_destroy ( &deinit_sem->ordinary_num ) <0 ) {

        return 1;
    }
    if ( sem_destroy ( &deinit_sem->barber_end ) <0 ) {

        return 1;
    }

    return 0;
}
void remove_garbage ( S_share_data *remove_mem, int rem_segment_id,int *sem_init_array,int main_PID,pid_t *customer_PIDs, int rem_segment_proc_id) {

    unsigned int a;

    for (a=0;a<remove_mem->number_of_customers;a++) {

        if (customer_PIDs[a]>0) {
            kill(customer_PIDs[a],SIGKILL);
        }

    }

    kill(remove_mem->barber_PID,SIGKILL);

    if ( sem_init_array[0]==1 )
        sem_destroy ( &remove_mem->barber_room );
    if ( sem_init_array[1]==1 )
        sem_destroy ( &remove_mem->waiting_room );
    if ( sem_init_array[2]==1 )
        sem_destroy ( &remove_mem->access_seat );
    if ( sem_init_array[3]==1 )
        sem_destroy ( &remove_mem->barber_ready );
    if ( sem_init_array[4]==1 )
        sem_destroy ( &remove_mem->barber_finished );
    if ( sem_init_array[5]==1 )
        sem_destroy ( &remove_mem->customer_ready );
    if ( sem_init_array[6]==1 )
        sem_destroy ( &remove_mem->ordinary_num );
    if ( sem_init_array[7]==1 )
        sem_destroy ( &remove_mem->barber_end );

    if((strcmp(remove_mem->name_of_file,"-"))!=0)
        fclose(remove_mem->file);
    
		shmdt ( remove_mem );
    shmctl ( rem_segment_id, IPC_RMID, NULL );

    
		shmdt  (customer_PIDs);

    shmctl (rem_segment_proc_id,IPC_RMID,NULL);
    kill(main_PID,SIGKILL);
    return;

}

/* Initializing variables into shared segment */
void init_var_shared_mem ( S_arguments *share_arg ,S_share_data *variables ) {

    variables->T_barber_serve=share_arg->T_barber;
    variables->T_customer_create=share_arg->T_generate;
    variables->free_seats=share_arg->free_seats;
    variables->all_free_seats=share_arg->free_seats;
    variables->number_of_customers=share_arg->customer;
    variables->name_of_file=share_arg->name_of_file;
    variables->last_customer=share_arg->customer;
    variables->kill_customer=0;
}

int open_file ( S_share_data *s_file ) {

    if ( ( s_file->file=fopen ( s_file->name_of_file,"w" ) ) ==NULL ) {

        return 1;
    }

    return 0;
}

int close_file ( S_share_data *close_file ) {

    if ( ( fclose ( close_file->file ) ) ==EOF ) {

        return 1;
    }

    return 0;
}

void signal_control_ppid ( int type_sig) {
    pid_t parent=getppid();
    kill ( parent,type_sig );
    exit ( 1 );
}
/* Initializing array [shared mem (processID)] */
void init_custom_proc_PID(S_share_data *init_mem,pid_t *proc_PID) {

    unsigned int a;
    for (a=0;a<init_mem->number_of_customers;a++) {
        proc_PID[a]=-1;

    }

}

int main ( int argc, char **argv ) {

    ( void ) signal (SIGINT,signal_control_ppid);
    ( void ) signal (SIGHUP,signal_control_ppid);
    ( void ) signal (SIGTERM,signal_control_ppid);
    S_arguments f_arg;
    S_share_data *mem_share;

    int segment_id;
    int segment_proc_ID=-1;
    int customer_PID;
    int main_PID=getpid();
    pid_t *custom_PID_proc=NULL;
    int sem_array[10]={0};



    if ( argument ( argc,argv,&f_arg ) !=0 ) {
        fprintf ( stderr,"Bad input argument.\n" );
        return 1;
    }

			if(f_arg.customer==0)
			return 0;



    /* SEMAPHORES AND OTHER STUFFS FOR SYNCHRONIZATIONS */

    /* Size of shared segment size [IPC] */
    const int size = sizeof ( S_share_data );

    segment_id=shmget ( IPC_PRIVATE, size, S_IRUSR | S_IWUSR );
    if ( segment_id<0 ) {
        fprintf ( stderr,"Error, mem allocate.\n" );

        return 1;
    }
    /* Attache shared segment */
    mem_share= ( S_share_data * ) shmat ( segment_id,NULL,0 );

    /* Initialize semaphores */
    if ( semaphore_init ( mem_share,sem_array ) ==1 ) {
        fprintf ( stderr,"Error, init semaphors.\n" );
        remove_garbage ( mem_share,segment_id,sem_array,main_PID, custom_PID_proc,segment_proc_ID);
        return 1;
    }

    /* Initialize variables */
    init_var_shared_mem ( &f_arg,mem_share );


    /* SHARED MEM FOR SAVING CUSTOMER PIDs */
    /* Allocate shared memory */
    segment_proc_ID=shmget(IPC_PRIVATE,sizeof((*custom_PID_proc))*f_arg.customer,S_IRUSR|S_IWUSR);
    if (segment_proc_ID < 0) {
        fprintf ( stderr,"Error, mem allocate .\n" );
        return 1;
    }
    /* Attache shared memory */
    custom_PID_proc= (pid_t *) shmat(segment_proc_ID,NULL,0);


    init_custom_proc_PID(mem_share,custom_PID_proc);


    if ( ( strcmp ( argv[5],"-" ) !=0 ) ) {
        if ( open_file ( mem_share ) ==1 )
            fprintf ( stderr,"Error, open file" );
    }

    mem_share->barber_PID = create_barber_process ( mem_share );
    if ( mem_share->barber_PID<0 || mem_share->barber_PID==1) {
        fprintf ( stderr,"Error,creating barber.\n" );
        remove_garbage ( mem_share,segment_id,sem_array,main_PID,custom_PID_proc,segment_proc_ID);
        return 1;
    }

    customer_PID = create_customer_process ( mem_share,custom_PID_proc );
    if ( customer_PID<0 || customer_PID==1 ) {
        fprintf ( stderr,"Error,creating customer.\n" );
        remove_garbage ( mem_share,segment_id,sem_array ,main_PID, custom_PID_proc,segment_proc_ID);
        return 1;
    }

    waitpid ( mem_share->barber_PID,NULL,0 );

    if ( ( strcmp ( argv[5],"-" ) !=0 ) ) {
        if ( ( close_file ( mem_share ) ) ==1 )
            fprintf ( stderr,"Error, close file.\n" );
    }
    if ( semaphore_deinit ( mem_share ) ==1 ) {
        fprintf ( stderr,"Error,deinit semaphors.\n" );
        return 1;
    }
    /* Detach memory [processID] */
    if ( shmdt (custom_PID_proc)<0) {
        fprintf ( stderr,"Error,detach mem.\n" );
        return 1;
    }
    /* Remove shared memory segment [processID] */

    if ( shmctl ( segment_proc_ID, IPC_RMID, NULL ) <0 ) {
        fprintf ( stderr,"Error,rem mem.\n" );
        return 1;
    }
    /* Detach memory [semaphors +etc] */
    if ( shmdt ( mem_share ) <0 ) {
        fprintf ( stderr,"Error,detach mem.\n" );
        return 1;

    }

    /* Remove shared memory segment [semaphors +etc] */
    if ( shmctl ( segment_id, IPC_RMID, NULL ) <0 ) {
        fprintf ( stderr,"Error,rem mem.\n" );
        return 1;
    }

    return 0;
}
