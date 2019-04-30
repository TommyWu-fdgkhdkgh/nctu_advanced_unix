#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "main.h"


void build_inodes(pid pids[], inode inodes[], int *nindex){

  int fd;
  char slocal_ip[INET_ADDRSTRLEN];
  char sremote_ip[INET_ADDRSTRLEN];
  char openbuffer[10000];

  for(int i=0;i<4;i++){
    memset(openbuffer,0,sizeof(openbuffer));
    switch(i){
      case 0:
        fd=open("/proc/net/tcp",O_RDONLY);
        break;
      case 1:
        fd=open("/proc/net/tcp6",O_RDONLY);
        break;
      case 2:
        fd=open("/proc/net/udp",O_RDONLY);
        break;
      case 3:
        fd=open("/proc/net/udp6",O_RDONLY);
        break;
    }

    int readlen=read(fd,openbuffer,sizeof(openbuffer));
    //printf("open:%s\n",openbuffer);   
    //parse the content of the file to tokens 
    char tokens[18][512];

    memset(tokens,0,sizeof(char)*18*512);
    memset(slocal_ip,0,sizeof(char)*INET_ADDRSTRLEN);
    memset(sremote_ip,0,sizeof(char)*INET_ADDRSTRLEN);

    int state=0;
    int tokennumber=0;
    int cindex=0;
    //printf("readlen:%d\n",readlen);
    int after_first_line=0;

    for(int j=0;j<readlen;j++){
      //printf("%c ",openbuffer[j]);
      if(openbuffer[j]==0xa){
        if(after_first_line==0) after_first_line=1;
      }
      if(after_first_line==0) continue;

      if(openbuffer[j]!=10 && state==0 && openbuffer[j]==32 && openbuffer[j+1]!=32){
        state=1;
      }
      if(openbuffer[j]!=10 && state==1 && openbuffer[j]!=32){
        tokens[tokennumber][cindex++]=openbuffer[j];
      }

      if(openbuffer[j]!=10 && state==1 && openbuffer[j]!=32  && openbuffer[j+1]==32){
        state=0;
        tokens[tokennumber][cindex]=0;//add null byte
        cindex=0;
        tokennumber++;
      }
      if(tokennumber==17 || (openbuffer[j]==10 && tokennumber>0)){
        //a round is end!    
        tokennumber=0;
        cindex=0;

        //fill in inode number
        inodes[*nindex].inode_n=atoi(tokens[9]);

        //fill in proto
        inodes[*nindex].proto=i;

        //parse local port & remote port
        if(i==0 || i==2){
          inodes[*nindex].local_port=(int)strtol(tokens[1]+9,NULL,16);
          inodes[*nindex].remote_port=(int)strtol(tokens[2]+9,NULL,16);
        }else if(i==1 || i==3){
          inodes[*nindex].local_port=(int)strtol(tokens[1]+33,NULL,16);
          inodes[*nindex].remote_port=(int)strtol(tokens[2]+33,NULL,16);
        }

        //fill in local_addr  &  remote_addr
        if(i==0 || i==2){
          tokens[1][8]=0;
          inodes[*nindex].local_ip.ipv4.s_addr=(uint32_t)strtol(tokens[1],NULL,16);

          tokens[2][8]=0;
          inodes[*nindex].remote_ip.ipv4.s_addr=(uint32_t)strtol(tokens[2],NULL,16);
        }else if(i==1 || i==3){
          tokens[1][32]=0;
          sscanf(tokens[1],"%08X%08X%08X%08X",
            &inodes[*nindex].local_ip.ipv6.s6_addr32[0],
            &inodes[*nindex].local_ip.ipv6.s6_addr32[1],
            &inodes[*nindex].local_ip.ipv6.s6_addr32[2],
            &inodes[*nindex].local_ip.ipv6.s6_addr32[3]);

          tokens[2][32]=0;
          sscanf(tokens[2],"%08X%08X%08X%08X",
            &inodes[*nindex].remote_ip.ipv6.s6_addr32[0],
            &inodes[*nindex].remote_ip.ipv6.s6_addr32[1],
            &inodes[*nindex].remote_ip.ipv6.s6_addr32[2],
            &inodes[*nindex].remote_ip.ipv6.s6_addr32[3]);
        }
        (*nindex)++;
      }
    }
  }






}
