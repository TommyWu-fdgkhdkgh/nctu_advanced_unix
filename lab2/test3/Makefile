all: test lib.so


test: main.c
	@gcc main.c -o test

lib.so: lib.c
	@gcc lib.c -o lib.so -fPIC -shared -ldl -D_GNU_SOURCE

clean: 
	@rm -f test lib.so

start:
	@LD_PRELOAD=./lib.so ./test
