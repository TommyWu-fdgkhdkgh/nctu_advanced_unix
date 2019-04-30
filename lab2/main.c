#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
  char buf[100];

  read(0, buf, 3);

  write(1,"hi\n",3);

  return 0;
}
