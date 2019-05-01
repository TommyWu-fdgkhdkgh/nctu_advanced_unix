CC = gcc
CFLAG = -Wall -g

all: a.out fsmon.so

a.out: main.c
	@$(CC) $(CFLAG) main.c

fsmon.so: fsmon.c
	@$(CC) $(CFLAG) fsmon.c -o fsmon.so -fPIC -shared -ldl -D_GNU_SOURCE

clean:
	@rm -f a.out fsmon.so 

start:
	@LD_PRELOAD=./fsmon.so ./a.out
