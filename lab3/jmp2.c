#include "libmini.h"

typedef void (*proc_t)();
static jmp_buf jb;

#define	FUNBODY(m, from) { write(1, m, strlen(m)); longjmp(jb, from); }


void handler(){
	
}

proc_t funs[] = { a, b, c, d, e, f, g, h, i, j };

int main() {



	while(1){


	}


	return 0;
}

