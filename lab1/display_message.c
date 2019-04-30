#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <regex.h>
#include <arpa/inet.h>
#include "main.h"


void print_message(pid pids[], inode inodes[], int *nindex, int *pindex, int inodein, int pidin){

  char slocal_ip[INET_ADDRSTRLEN];
  char sremote_ip[INET_ADDRSTRLEN];
  char temp_localaddress[256];
  char temp_foreignaddress[256];
  char temp_pid_name[3100];
  char temp_proto[256];

  memset(slocal_ip,0,sizeof(char)*INET_ADDRSTRLEN);
  memset(sremote_ip,0,sizeof(char)*INET_ADDRSTRLEN);
  memset(temp_localaddress, 0, 256);
  memset(temp_foreignaddress, 0, 256);
  memset(temp_pid_name, 0, 3100);
  memset(temp_proto, 0, 256);


  switch(inodes[inodein].proto){
    case 0:
      inet_ntop(AF_INET,&(inodes[inodein].local_ip.ipv4),slocal_ip,INET_ADDRSTRLEN);
      inet_ntop(AF_INET,&(inodes[inodein].remote_ip.ipv4),sremote_ip,INET_ADDRSTRLEN);
      sprintf(temp_proto, "%s", "tcp");
      break;
    case 1:
      inet_ntop(AF_INET6,&(inodes[inodein].local_ip.ipv6),slocal_ip,INET6_ADDRSTRLEN);
      inet_ntop(AF_INET6,&(inodes[inodein].remote_ip.ipv6),sremote_ip,INET6_ADDRSTRLEN);
      sprintf(temp_proto, "%s", "tcp6");
      break;
    case 2:
      inet_ntop(AF_INET,&(inodes[inodein].local_ip.ipv4),slocal_ip,INET_ADDRSTRLEN);
      inet_ntop(AF_INET,&(inodes[inodein].remote_ip.ipv4),sremote_ip,INET_ADDRSTRLEN);
      sprintf(temp_proto, "%s", "udp");
      break;
    case 3:
      inet_ntop(AF_INET6,&(inodes[inodein].local_ip.ipv6),slocal_ip,INET6_ADDRSTRLEN);
      inet_ntop(AF_INET6,&(inodes[inodein].remote_ip.ipv6),sremote_ip,INET6_ADDRSTRLEN);
      sprintf(temp_proto, "%s", "udp6");
      break;
  }


  if(inodes[inodein].local_port==0 && inodes[inodein].remote_port==0){
  
    sprintf(temp_localaddress, "%s:*", slocal_ip);
    sprintf(temp_foreignaddress, "%s:*", sremote_ip);
    sprintf(temp_pid_name, "%d/%s", pids[pidin].pid, pids[pidin].name);
  
  }else if(inodes[inodein].local_port==0){
  
    sprintf(temp_localaddress, "%s:*", slocal_ip);
    sprintf(temp_foreignaddress, "%s:%d", sremote_ip, inodes[inodein].remote_port);
    sprintf(temp_pid_name, "%d/%s", pids[pidin].pid, pids[pidin].name);
  
  }else if(inodes[inodein].remote_port==0){
  
    sprintf(temp_localaddress, "%s:%d", slocal_ip, inodes[inodein].local_port);
    sprintf(temp_foreignaddress, "%s:*", sremote_ip);
    sprintf(temp_pid_name, "%d/%s", pids[pidin].pid, pids[pidin].name);
  
  }else{
  
    sprintf(temp_localaddress, "%s:%d", slocal_ip, inodes[inodein].local_port);
    sprintf(temp_foreignaddress, "%s:%d", sremote_ip, inodes[inodein].remote_port);
    sprintf(temp_pid_name, "%d/%s", pids[pidin].pid, pids[pidin].name);
  
  }

  printf("%-12s%-25s%-25s%-20s\n", temp_proto, temp_localaddress, temp_foreignaddress, temp_pid_name);

}


void display_message(pid pids[], inode inodes[], int *nindex, int*pindex, char *filter_string, int option){

  regex_t regex;
  int reti;
  int first=1;

  if(filter_string!=NULL){
    reti=regcomp(&regex,filter_string,0);
  }

  if(option==0 || option==1){
    for(int p=0;p<=1;p++){
      for(int i=0;i<*pindex;i++){
        if(filter_string!=NULL){
          reti=regexec(&regex,pids[i].name,0,NULL,0);
          regfree(&regex);
          if(reti){
            continue;
          }
        }
        for(int j=0;j<pids[i].iindex;j++){
          for(int k=0;k<*nindex;k++){
            if(inodes[k].inode_n==pids[i].inodes[j] && inodes[k].proto==p){
              if(first==1){
                printf("List of TCP connections:\n");
		printf("%-12s%-25s%-25s%-20s\n", "Proto", "Local Address", "Foreign Address", "PID/Program name and arguments");
                //printf("Proto\tLocal Address\t\tForeign Address\t\tPID/Program name and arguments\n");
                first=0;
              }

	      print_message(pids, inodes, nindex, pindex, k, i);

            }
          }
        }
      }
    }
  }

  if(first==0){
    printf("\n");
  }

  first=1;
  if(option==0 || option==2){
    for(int p=2;p<=3;p++){
      for(int i=0;i<*pindex;i++){
        if(filter_string!=NULL){
          reti=regexec(&regex,pids[i].name,0,NULL,0);
          regfree(&regex);
          if(reti){
            continue;
          }
        }
        for(int j=0;j<pids[i].iindex;j++){
          for(int k=0;k<*nindex;k++){
            if(inodes[k].inode_n==pids[i].inodes[j] && inodes[k].proto==p){
              if(first==1){
                printf("List of UDP connections:\n");
		printf("%-12s%-25s%-25s%-20s\n", "Proto", "Local Address", "Foreign Address", "PID/Program name and arguments");
                //printf("Proto\tLocal Address\t\tForeign Address\t\tPID/Program name and arguments\n");
                first=0;
              }

              print_message(pids, inodes, nindex, pindex, k, i);
 
            }
          }
        }
      }
    }
  }


}
