// myhack.c
#include <stdio.h>
#include <string.h>

int mystrcmp(const char *s1, const char *s2)
{
    printf("hack function invoked. s1=<%s> s2=<%s>\n", s1, s2);
    // always return 0, which means s1 equals to s2--总是相等
    return 0;
}
