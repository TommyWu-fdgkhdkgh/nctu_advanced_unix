想看所有的question，可以grep -r "Q:" ./

## Features

/* system calls */
long sys_read(int fd, char *buf, size_t count);
long sys_write(int fd, const void *buf, size_t count);
long sys_open(const char *filename, int flags, ... /*mode*/);
long sys_close(unsigned int fd);
long sys_pause();
long sys_nanosleep(struct timespec *rqtp, struct timespec *rmtp);
long sys_exit(int error_code) __attribute__ ((noreturn));
//HW3 EXTEND
long sys_alarm(unsigned int);
long sys_rt_sigprocmask(int how, const sigset_t  *set,sigset_t  *oset,size_t);
long sys_rt_sigpending(sigset_t  *set,size_t);
long sys_rt_sigaction(int,const struct sigaction  *,struct sigaction  *,size_t);
long sys_rt_sigreturn(unsigned long __unused);


/* wrappers */
ssize_t read(int fd, char *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
int     open(const char *filename, int flags, ... /*mode*/);
int     close(unsigned int fd);
int     pause();
int     nanosleep(struct timespec *rqtp, struct timespec *rmtp);
void    exit(int error_code);
//HW3 EXTEND
unsigned int alarm(unsigned int);
int sigemptyset(sigset_t *set);
int sigaddset(sigset_t *_set, int _sig);
int sigfillset(sigset_t *set);
int sigdelset(sigset_t *set, int signum);
int sigismember(const sigset_t *_set, int _sig);
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
int sigpending(sigset_t *_set);
sighandler_t signal(int signum, sighandler_t handler);
int sigaction(int signum, const struct sigaction *act,struct sigaction *oldact);
int setjmp(jmp_buf env);
void longjmp(jmp_buf env, int val);

void bzero(void *s, size_t size);
size_t strlen(const char *s);
void perror(const char *prefix);
unsigned int sleep(unsigned int s);




