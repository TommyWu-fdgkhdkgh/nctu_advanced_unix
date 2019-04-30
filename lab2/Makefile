all: a.out fsmon.so

a.out: main.c
	@gcc main.c

fsmon.so: fsmon.c
	@gcc fsmon.c -o fsmon.so -fPIC -shared -ldl -D_GNU_SOURCE

clean:
	@rm -f a.out fsmon.so 

start:
	@LD_PRELOAD=./fsmon.so ./a.out
