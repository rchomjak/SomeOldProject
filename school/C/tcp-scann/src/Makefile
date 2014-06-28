APP = tcpsearch
C_SRCS =file_read_parser.c network.c print.c mgetopt.c ports_range_parser.c tcpsearch.c host_t.c ports_operations.c result_t.c 

C_OBJS = ${C_SRCS:.c=.o}

program_OBJS = $(C_OBJS)


CFLAGS +=-D_POSIX_C_SOURCE=200112L -Os  -std=c99 -Wall -pedantic -pedantic-errors 


.PHONY: all clean distclean 

all: $(APP)

$(APP): $(program_OBJS)
	$(CC) $(CFLAGS) $(program_OBJS) -o $(APP)


clean:
	rm -f *.o *.core $(APP)	

distclean: clean

