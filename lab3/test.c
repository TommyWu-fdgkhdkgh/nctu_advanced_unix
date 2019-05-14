#include "libmini.h"

typedef void (*proc_t)();
static jmp_buf jb;

#define FUNBODY(m, from) { write(1, m, strlen(m)); longjmp(jb, from); }

void a() FUNBODY("This is function a().\n", 1);
void b() FUNBODY("This is function b().\n", 2);
void c() FUNBODY("This is function c().\n", 3);
void d() FUNBODY("This is function d().\n", 4);
void e() FUNBODY("This is function e().\n", 5);
void f() FUNBODY("This is function f().\n", 6);
void g() FUNBODY("This is function g().\n", 7);
void h() FUNBODY("This is function h().\n", 8);
void i() FUNBODY("This is function i().\n", 9);
void j() FUNBODY("This is function j().\n", 10);

proc_t funs[] = { a, b, c, d, e, f, g, h, i, j };



static char err_o[] = "Error opening file.\n";
static char err_w[] = "Error writing to stdout.\n";


void handler(int s) { 
	//puts("get into handler!\n");
	/* do nothing */ 
}


int main(int argc, char *argv[]){

	//puts("hi!\n");

        volatile int i = 0;

        if(setjmp(jb) != 0) {
                i++;
        }
        if(i < 10) funs[i]();


	/*signal(SIGALRM, handler);
	alarm(1);
	pause();*/

	
	//third test case
	/*sigset_t s;
	sigemptyset(&s);
	sigaddset(&s, SIGALRM);
	sigprocmask(SIG_BLOCK, &s, NULL);
	signal(SIGALRM, SIG_IGN);
	signal(SIGINT, handler);
	alarm(1);
	pause();
	if(sigpending(&s) < 0) perror("sigpending");
	if(sigismember(&s, SIGALRM)) {
		char m[] = "sigalrm is pending.\n";
		write(1, m, sizeof(m));
	} else {
		char m[] = "sigalrm is not pending.\n";
		write(1, m, sizeof(m));
	}*/
	
	/*
	//second test case
	sigset_t s;
	sigemptyset(&s);
	sigaddset(&s, SIGALRM);
	sigprocmask(SIG_BLOCK, &s, NULL);
	alarm(3);
	sleep(5);
	if(sigpending(&s) < 0) perror("sigpending");
	if(sigismember(&s, SIGALRM)) {
		char m[] = "sigalrm is pending.\n";
		write(1, m, sizeof(m));
	} else {
		char m[] = "sigalrm is not pending.\n";
		write(1, m, sizeof(m));
	}
	*/

	//first test case
	/*	
		alarm(3);
		pause();
	*/

	//test cat1s
	/*int fd = 0, rlen;
	char buf[4096];
	if(argc > 1) {*/
	//	if((fd = sys_open(argv[1], 0 /*O_RDONLY*/, 0)) < 0) {
	/*		sys_write(2, err_o, sizeof(err_o));
			return -1;
		}
	}
	while((rlen = sys_read(fd, buf, sizeof(buf))) > 0) {
		if(sys_write(1, buf, rlen) < 0) {
			sys_write(2, err_w, sizeof(err_w));
			return -2;
		}
	}
	sys_close(fd);
	*/	

	return 0;
}
