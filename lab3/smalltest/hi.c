#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>

void handler(int s){/* do nothing */}


typedef void (*proc_t)();
static jmp_buf jb;

#define	FUNBODY(m, from) { write(1, m, strlen(m)); longjmp(jb, from); }

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

int hi(int g){
	puts("hi!\n");
	return g;
}


int main(){
	//int g=5;

	//hi(g);

	volatile int i = 0;
	if(setjmp(jb) != 0){
		i++;
	}
	if(i<10) funs[i]();
	


	/*printf("sizeof(unsigned long int):%ld\n", sizeof(unsigned long int));
	printf("sizeof(sigset_t):%ld\n", sizeof(sigset_t));
	printf("1024/(8 * sizeof(unsigned long int)):%ld\n", 1024/(8*sizeof(unsigned long int)));
	//printf("_NSIG_WORDS:%d\n", _NSIG_WORDS);	
	//alarm(5);
	printf("SIGALRM:%d\n", SIGALRM);
	printf("SIG_BLOCK:%d\n", SIG_BLOCK);*/
	/*printf("sizeof(sigaction):%ld\n", sizeof(struct sigaction));

	sigset_t s;
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



	return 0;
}
