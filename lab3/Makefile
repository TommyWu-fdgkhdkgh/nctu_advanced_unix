CC = gcc
CXX = g++
ASM32 = yasm -f elf32
ASM64 = yasm -f elf64 


CFLAGS = -g -Wall -masm=intel -fno-stack-protector


all: libmini.so test alarm1 alarm2 alarm3 jmp1 


libmini.so: libmini64.o libmini.o
	ld -shared -o $@ $^	

libmini64.o: libmini64.asm
	$(ASM64) -DYASM -D__x86_64__ -DPIC $< -o $@

libmini.o: libmini.c
	$(CC) -c -g -Wall -fno-stack-protector -fPIC -nostdlib $<

test: test.o start.o libmini.so
	ld -m elf_x86_64 --dynamic-linker /lib64/ld-linux-x86-64.so.2 -o $@ test.o start.o -L. -L.. -lmini	

alarm1: alarm1.o start.o libmini.so
	ld -m elf_x86_64 --dynamic-linker /lib64/ld-linux-x86-64.so.2 -o $@ alarm1.o start.o -L. -L.. -lmini
alarm2: alarm2.o start.o libmini.so
	ld -m elf_x86_64 --dynamic-linker /lib64/ld-linux-x86-64.so.2 -o $@ alarm2.o start.o -L. -L.. -lmini
alarm3: alarm3.o start.o libmini.so
	ld -m elf_x86_64 --dynamic-linker /lib64/ld-linux-x86-64.so.2 -o $@ alarm3.o start.o -L. -L.. -lmini
jmp1: jmp1.o start.o libmini.so
	ld -m elf_x86_64 --dynamic-linker /lib64/ld-linux-x86-64.so.2 -o $@ jmp1.o start.o -L. -L.. -lmini

alarm1.o: alarm1.c
	$(CC) -c -g -Wall -fno-stack-protector -nostdlib -I. -I.. -DUSEMINI $<

alarm2.o: alarm2.c
	$(CC) -c -g -Wall -fno-stack-protector -nostdlib -I. -I.. -DUSEMINI $<

alarm3.o: alarm3.c
	$(CC) -c -g -Wall -fno-stack-protector -nostdlib -I. -I.. -DUSEMINI $<

jmp1.o: jmp1.c
	$(CC) -c -g -Wall -fno-stack-protector -nostdlib -I. -I.. -DUSEMINI $<
 
test.o: test.c
	$(CC) -c -g -Wall -fno-stack-protector -nostdlib -I. -I.. -DUSEMINI $<

start.o: start.asm
	$(ASM64) -DYASM -D__x86_64__ -DPIC $< -o $@

test_test:
	LD_LIBRARY_PATH=. ./test

clean:
	rm -f *.o test alarm1 alarm2 alarm3 jmp1 *.so
