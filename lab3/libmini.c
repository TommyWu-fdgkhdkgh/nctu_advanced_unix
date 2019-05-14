#include "libmini.h"

long toy(){
	puts("toy!\n");	
	return 1;
}
//<<<write
long write(unsigned int fd, const char *buf, size_t count){
	return sys_write(fd, buf, count);
}
//<<<read
long read(unsigned int fd, char *buf, size_t count){
	return sys_read(fd, buf, count);
}
//<<<open
long open(const char *filename, int flags, int mode){
	return sys_open(filename, flags, mode);
}
//<<<close
long close(unsigned int fd){
	return sys_close(fd);
}
//<<<exit
long exit(int error_code){
	return sys_exit(error_code);
}
//<<<puts
long puts(const char *st){

	int len = strlen(st);

	long result = write(0, st, len);

	return result;
}
//<<<nanosleep
long nanosleep(struct timespec *rqtp, struct timespec *rmtp){
	return sys_nanosleep(rqtp, rmtp);
}
//<<<sleep
long sleep(unsigned int seconds){
	//simple sleep
	//不考慮睡到一半被interrupt
	//也不考慮睡的秒數大於一次能睡的最大秒數
	//(上面兩個在原本的glibc都有考慮到)

	struct timespec ts = {0, 0};
	ts.tv_sec = seconds;

	nanosleep(&ts, NULL);	

	return 0;
}
//<<<alarm
long alarm(unsigned int seconds){
	return sys_alarm(seconds);
}
//<<<pause
long pause(){
	return sys_pause();
}
//<<<sigemptyset
int sigemptyset(sigset_t *set){
	if(set == NULL){
		return -1;
	}
	
	*set = 0;
	
	return 0;
}
//<<<sigaddset
int sigaddset(sigset_t *set, int signo){
	if(set == NULL || signo<=0){
		puts("error!\n");
		return -1;
	}

	unsigned long int __mask = __sigmask(signo);

	(*set) = (*set) | __mask;

	return 0;
}
//<<<rt_sigprocmask
long rt_sigprocmask(int how, sigset_t *nset, sigset_t *oset, size_t sigsetsize){
	return sys_rt_sigprocmask(how, nset, oset, sigsetsize);
}
//<<<rt_sigpending
long rt_sigpending(sigset_t *set, size_t sigsetsize){
	return sys_rt_sigpending(set, sigsetsize);
}
//<<<sigpending
int sigpending(sigset_t *set){
	return rt_sigpending(set, _NSIG/8);
}
//<<<sigprocmask
int sigprocmask(int how, const sigset_t *nset, sigset_t *oset){
	//error detect
	switch(how){
		case SIG_BLOCK:
		case SIG_UNBLOCK:
		case SIG_SETMASK:
			break;
		default:
			return -1;
	}

	//Q:不懂sigsetsize是什麼
	//A:sigset_t這個type的大小
	rt_sigprocmask(how, nset, oset, _NSIG/8);

	return 0;

}
//<<<sigismember
int sigismember(const sigset_t *set, int signo){
	if(set==NULL || signo<=0 || signo>=_NSIG){
		return -1;
	}

	unsigned long int __mask = __sigmask(signo);

	int result = ((*set) & __mask)? 1 : 0;

	return result;
}
//<<<sigaction
long sigaction(int sig, const struct sigaction *act, struct sigaction *oact){
	return sys_rt_sigaction(sig, act, oact, _NSIG/8);
}
//signal
__sighandler_t signal(int sig, __sighandler_t handler){

	int result;

	struct sigaction act, oact;

	if(handler == SIG_ERR || sig<1){
		return SIG_ERR;
	}

	act.sa_handler = handler;

	sigemptyset(&(act.sa_mask));

	sigaddset(&(act.sa_mask), sig);

	//set restore
	act.sa_flags = 0;
	act.sa_flags = act.sa_flags | SA_RESTORER;
	act.sa_restorer = &sys_rt_sigreturn;
	
	result = sigaction(sig, &act, &oact);

	if(result<0){
		return SIG_ERR;
	}

	return result;
}
//<<<perror
void perror(const char *st){
	puts(st);
	exit(-1);
}
//<<<strlen
int strlen(char *st){
	int i=0;
	while(1){
		if(st[i]==0){
			break;
		}else {
			i++;
		}
	}
	return i;
}



