#include <stdio.h>

//signal
struct sigaction{
        /*union{
                __sighandler_t sa_handler;
                //void (*sa_sigaction)(int, siginfo_t *, void *);
        }*/
        __sighandler_t sa_handler;

        //additional set of signals to be blocked
        sigset_t sa_mask;

        //special flags
        int sa_flags;

        //restore handler
        void (*sa_restorer)(void);
};


int main(){

	printf("%ld\n", sizeof(struct sigaction));
}
