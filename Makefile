CC = gcc
CFLAGS = -Werror -Wpedantic -Wextra -Wall -std=c99 -o

all:	increment

increment:	increment.c
	$(CC)	$(CFLAGS)	increment	increment.c	-lpthread

clean:
	rm	-f	increment
	
