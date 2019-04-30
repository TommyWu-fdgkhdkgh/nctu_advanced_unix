CC	= gcc
CFLAG   = -Wall -g


#./a.out: build_pids.c display_message.c build_inodes.c main.c
#	@$(CC) $(CFLAG) build_pids.c display_message.c build_inodes.c main.c

all: build_pids.c option.c main.c build_inodes.c display_message.c
	@$(CC) $(CFLAG) build_pids.c option.c main.c build_inodes.c display_message.c

debug: build_pids.c option.c main.c build_inodes.c display_message.c
	@$(CC) $(CFLAG) build_pids.c option.c main.c build_inodes.c display_message.c

clean:
	@rm -f ./a.out


