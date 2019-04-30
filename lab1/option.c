#include <stdio.h>
#include <getopt.h>

char *l_opt_arg;
char *const short_options="ut";

struct option long_options[]={
  {"udp",0,NULL,'u'},
  {"tcp",0,NULL,'t'},
  {0,0,0,0}
};
int get_option(int argc, char *argv[]){
  int c;
  int option=0;
  while((c=getopt_long(argc, argv, short_options, long_options, NULL)) !=-1){
    switch(c){
      case 't':
        option=1;
        break;
      case 'u':
        option=2;
        break;
      default:
        break;
    }
  }

  return option;
}
