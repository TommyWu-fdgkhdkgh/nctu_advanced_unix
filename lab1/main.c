#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>    
#include <sys/types.h>
#include <netinet/in.h>
#include <regex.h> 
#include <fcntl.h> 
#include <arpa/inet.h>
#include "main.h"

extern int get_option(int argc, char* argv[]);
extern void build_pids(pid pids[], inode inodes[], int *pindex);
extern void build_inodes(pid pids[], inode inodes[], int *nindex);
extern void display_message(pid pids[], inode inodes[], int *nindex, int *pindex, char *filter_string, int option);

int main(int argc, char* argv[]){

  //get parameters
  //-u, --udp, -t, --tcp, filter_string
  char *filter_string=NULL;
  int option;
  option=get_option(argc, argv);
  if(argc==3){
    filter_string=argv[2];
  }else if(argc==2 && argv[1][0]!='-'){
    filter_string=argv[1];
  }


  pid pids[500];

  inode inodes[5000];

  int pindex=0;
  //build_pids
  build_pids(pids, inodes , &pindex);

  //get inodes
  int nindex=0;

  build_inodes(pids, inodes, &nindex);
  //display all the message
  display_message(pids, inodes, &nindex, &pindex, filter_string, option);  
  return 0;
}
