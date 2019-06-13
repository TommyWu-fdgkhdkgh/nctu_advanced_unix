#ifndef __LIBMINI_H__

#define __LIBMINI_H__


#define NULL 0
#define _SIGSET_NWORDS (1024/(8*sizeof(unsigned long int)))

#define        SIG_ERR         ((__sighandler_t) -1)        /* Error return.  */
#define        SIG_DFL         ((__sighandler_t)  0)        /* Default action.  */
#define        SIG_IGN         ((__sighandler_t)  1)        /* Ignore signal.  */

//type of signal handler
typedef void (* __sighandler_t)(int);
//信號的集合
//typedef char sigset_t;
/*typedef struct {
	unsigned long int __val[_SIGSET_NWORDS];
}sigset_t;*/
typedef unsigned long int sigset_t;

typedef long long size_t;
typedef unsigned int mode_t;
typedef unsigned int uid_t;
typedef unsigned int gid_t;


//找出sig要擺在哪個bit
//sizeof(unsigned long int)的bytes
//每個bytes有8個bit
#define __sigmask(sig) \
	(((unsigned long int) 1) << (((sig) - 1)))

//找出sig要擺在哪個element(也就是算出index)
#define __sigword(sig) \
	 (((sig) - 1) / (8 * sizeof (unsigned long int)))

//signal
struct sigaction{
	/*union{
		__sighandler_t sa_handler;
		//void (*sa_sigaction)(int, siginfo_t *, void *);
	}*/
	__sighandler_t sa_handler;


	//special flags
	int sa_flags;

	//restore handler
	void (*sa_restorer)(void);

	//additional set of signals to be blocked
	sigset_t sa_mask;


};

//jmp_buf
//Q:為則麼要有[1]?
typedef struct jmp_buf_s {
	long long reg[8];
	//saved the signal mask?
	//int mask_was_saved;
	sigset_t mask;
} jmp_buf[1];


//time
struct timespec {
	long tv_sec;	//seconds
	long tv_nsec;	//nanoseconds
};

struct timeval{
	long tv_sec;	//seconds
	long tv_usec;	//microseconds
};

struct timezone{
	long tz_minuteswest;	//minetes west of Greenwich
	long tz_dsttime;	//type of DST correction
};



// lib
long puts(const char *st);
long alarm(unsigned int seconds);
long pause();
long open(const char *filename, int flags, int mode);
long read(unsigned int fd, char *buf, size_t count);
long write(unsigned int fd, const char *buf, size_t count);
long close(unsigned int fd);
long exit(int error_code);
long nanosleep(struct timespec *rqtp, struct timespec *rmtp);
long rt_sigprocmask(int how, sigset_t *nset, sigset_t *oset, size_t sigsetsize);
long rt_sigpending(sigset_t *set, size_t sigsetsize);
long sleep(unsigned int seconds);
//初始化sigset_t結構
int sigemptyset(sigset_t *set);
//在一個信號集中增加一個信號
int sigaddset(sigset_t *set, int signo); 
//改變一個信號集的處理方法(block it, or unblock it)
int sigprocmask(int how, const sigset_t *set, sigset_t *oset);
//returns the set of signals that are pending for delivery to the calling thread
int sigpending(sigset_t *set);
//test whether signum is a member of set
int sigismember(const sigset_t *set, int signo);
//I think it is puts and exit
void perror(const char *st);
long sigaction(int sig, const struct sigaction *act, struct sigaction *oact);
int strlen(char *st);
__sighandler_t signal(int sig, __sighandler_t handler);
int setjmp(jmp_buf env);
void longjmp(jmp_buf env, int val);

// syscall (asm code)
long sys_open(const char *filename, int flags, int mode);
long sys_read(unsigned int fd, char *buf, size_t count);
long sys_write(unsigned int fd, const char *buf, size_t count);
long sys_close(unsigned int fd);
long sys_alarm(unsigned int seconds);
long sys_pause();
long sys_exit(int error_code);
long sys_rt_sigprocmask(int how, sigset_t *nset, sigset_t *oset, size_t sigsetsize);
long sys_rt_sigpending(sigset_t *set, size_t sigsetsize);
//第一個參數是想要停多久，第二個參數是當我在sleep的時候遇到interrupt，會把剩下的時間寫在rem
long sys_nanosleep(struct timespec *rqtp, struct timespec *rmtp);
long sys_rt_sigaction(int sig, const struct sigaction *act, struct sigaction *oact, size_t sigsetsize);
long sys_rt_sigreturn(unsigned long __unused);
int setjmp_asm(jmp_buf env);
void longjmp_asm(jmp_buf env, int val);

//signals
#define SIGHUP		 1
#define SIGINT		 2
#define SIGQUIT		 3
#define SIGILL		 4
#define SIGTRAP		 5
#define SIGABRT		 6
#define SIGIOT		 6
#define SIGBUS		 7
#define SIGFPE		 8
#define SIGKILL		 9
#define SIGUSR1		10
#define SIGSEGV		11
#define SIGUSR2		12
#define SIGPIPE		13
#define SIGALRM		14
#define SIGTERM		15
#define SIGSTKFLT	16
#define SIGCHLD		17
#define SIGCONT		18
#define SIGSTOP		19
#define SIGTSTP		20
#define SIGTTIN		21
#define SIGTTOU		22
#define SIGURG		23
#define SIGXCPU		24
#define SIGXFSZ		25
#define SIGVTALRM	26
#define SIGPROF		27
#define SIGWINCH	28
#define SIGIO		29

#define __SIGRTMIN        32
#define __SIGRTMAX        64

#define _NSIG                (__SIGRTMAX + 1)

#define SIG_BLOCK          0	/* for blocking signals */
#define SIG_UNBLOCK        1	/* for unblocking signals */
#define SIG_SETMASK        2	/* for setting the signal mask */

/* Bits in `sa_flags'.  */
#define SA_NOCLDSTOP  1 /* Don't send SIGCHLD when children stop.  */
#define SA_NOCLDWAIT  2 /* Don't create zombie on child death.  */
#define SA_SIGINFO    4 /* Invoke signal-catching function with three arguments instead of one.  */
#define SA_ONSTACK   0x08000000 /* Use signal stack by using `sa_restorer'. */
# define SA_RESTART   0x10000000 /* Restart syscall on signal return.  */
# define SA_NODEFER   0x40000000 /* Don't automatically block the signal when
                                    its handler is being executed.  */
# define SA_RESETHAND 0x80000000 /* Reset to SIG_DFL on entry to handler.  */
# define SA_INTERRUPT 0x20000000 /* Historical no-op.  */

/* NIOS2 uses the generic Linux UAPI but defines SA_RESTORER.  */
#define SA_RESTORER 0x04000000

//setjmp & longjmp
/* We only need to save callee-saved registers plus stackpointer and
   program counter.  */
#define JB_RBX        0
#define JB_RBP        1
#define JB_R12        2
#define JB_R13        3
#define JB_R14        4
#define JB_R15        5
#define JB_RSP        6
#define JB_PC        7
#define JB_SIZE (8*8)

#endif
