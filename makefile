.SUFFIXES:.c .o

CC=gcc

SRCS=main.c\
	pub.c\
	work.c\
	myhttp.c

OBJS=$(SRCS:.c=.o)
EXEC=main

start: $(OBJS)
	$(CC) -lpthread -o  $(EXEC) $(OBJS)
	@echo "--------------ok----------------"

.c.o:
	$(CC) -Wall -g -o $@ -c $<

clean:
	rm -rf *.o core.* $(EXEC)
