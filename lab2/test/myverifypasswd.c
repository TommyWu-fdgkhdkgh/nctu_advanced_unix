#include <stdio.h>
#include <string.h>
#include "mystrcmp.h"

void main(int argc,char **argv) {
    char passwd[] = "password";
    if (argc < 2) {
        printf("usage: %s <password>\n",argv[0]);
        return;
    }
    if (!mystrcmp(passwd,argv[1])) {
        printf("Correct Password!\n");
        return;
    }
    printf("Invalid Password!\n");
}
