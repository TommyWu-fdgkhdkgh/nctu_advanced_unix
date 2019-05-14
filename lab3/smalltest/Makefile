CC = gcc
CXX = g++
ASM32 = yasm -f elf32
ASM64 = yasm -f elf64 -DPIC

CFLAGS = -g -Wall -masm=intel -fno-stack-protector

PROGS = hello

all: $(PROGS)

# default is x86_64

%: %.asm
	$(ASM64) $< -o $@.o
	ld -m elf_x86_64 -o $@ $@.o



clean:
	rm -f a.out *.o $(PROGS)
