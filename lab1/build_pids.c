#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <dirent.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include "main.h"

//1 is number
//0 is not number
int isNumber(char *a){
  int len=strlen(a);
  for(int i=0;i<len;i++){
    if(!(a[i]>='0' && a[i]<='9')){
      return 0;
    }
  }
  return 1;
}

void build_pids(pid pids[], inode inodes[], int *pindex){

  //char proc_pid[512];
  char *proc_pid=(char *)malloc(sizeof(char)*512);
  //char path_link[512];
  char *path_link=(char *)malloc(sizeof(char)*512);
  char openbuffer[10000];
  DIR *start=opendir("/proc"); 
  struct dirent *b=readdir(start);
  if(b==NULL){
     free(proc_pid);
     free(path_link);
     return;
  }

  *pindex=0;//index of pid

  memset(pids, 0, sizeof(pid)*500);
  memset(inodes, 0, sizeof(inode)*5000);
  memset(openbuffer, 0, sizeof(char)*10000);
  memset(proc_pid, 0, 512);
  memset(path_link, 0, 512);

  for(;b!=NULL;b=readdir(start)){
    if(b->d_type==DT_DIR){
      //是資料夾，假如檔名是數字的話，就繼續進去看
      if(isNumber(b->d_name)){
        //拿pid 
        pids[*pindex].pid=atoi(b->d_name);

        //開fd directory，遍尋所有filedescriptor，搭配readlink挖到inode number
        sprintf(proc_pid,"/proc/%s/fd",b->d_name);

        //need sudo
        DIR *fds = opendir(proc_pid);
        if(fds==NULL) continue;
        for(struct dirent *j=readdir(fds);j!=NULL;j=readdir(fds)){
          if(j==NULL) break;
          sprintf(path_link,"%s/%s",proc_pid,j->d_name);

          int len=readlink(path_link,openbuffer,10000);
          //printf("link: %s\n",openbuffer);
          //variables for traverse the whole string
          //int len=strlen(openbuffer);
          int state=0;
          int cindex=0;
          openbuffer[len]=0;


          //printf("openbuffer:%s\n",openbuffer);

          //這裡可以用strncmp


          //variables for regex
          regex_t regex;
          int reti;
	  char reg[256];
	  memset(reg,0,256);
	  sprintf(reg,"%s","socket:");
          //compile the regex
          reti = regcomp(&regex, reg, 0);
          if(reti){
            printf("cannot compile regex!\n");
          }
          //execute the regex
          reti = regexec(&regex, openbuffer, 0, NULL, 0);
          if(!reti){
            //Match
            //printf("\n\n\nproc_pid:%s j->d_name:%s openbuffer:%s\n\n\n", proc_pid, j->d_name, openbuffer);

            //get inode number from whole string 
            for(int g=0;g<len;g++){
              if(openbuffer[g]=='['){
                state=1;
              }
              if(state==1 && openbuffer[g]>='0' && openbuffer[g]<='9'){
                path_link[cindex++]=openbuffer[g];
              }
              if(state==1 && openbuffer[g]==']'){
                break;
              }
            }
	    //printf("path_link:%s atoi:%d\n", path_link, atoi(path_link));
	    if(pids[*pindex].iindex>100){

	      //printf("iindex:%d\n", pids[*pindex].iindex);
	     
	    }

            path_link[cindex]=0;
            pids[*pindex].inodes[pids[*pindex].iindex]=atoi(path_link);
            pids[*pindex].iindex++;
          }
	  
	  regfree(&regex);

          if(strncmp("[0000]:", openbuffer, 7)==0){
            printf("hi\n");
            pids[*pindex].inodes[pids[*pindex].iindex]=atoi(openbuffer+7);
            pids[*pindex].iindex++;
          }

        }

        closedir(fds);

        //printf("before pids[%d].iindex:%d\n",pindex,pids[pindex].iindex);
        //開cmdline，拿到參數
        //其實整個檔案都有讀進去，只是%s碰到null byte就不印東西了
        memset(openbuffer,0,sizeof(openbuffer));

        sprintf(proc_pid,"/proc/%s/cmdline",b->d_name);
        //printf("read %s\n",proc_pid);
        int fd=open(proc_pid,O_RDONLY);
        int readlen=read(fd,openbuffer,sizeof(openbuffer));
        //replace null byte with space & 丟到pids的陣列裡
        for(int i=0;i<readlen;i++){
          if(openbuffer[i]==0 && openbuffer[i+1]!=0){
            openbuffer[i]=' ';
          }
          pids[*pindex].name[i]=openbuffer[i];
        }

        if(readlen==-1){
          printf("failed!\n");
        }
        //printf("cmdline:%s ",pids[pindex].name);   

        (*pindex)++;
      }
    }
  }

  closedir(start);
  free(proc_pid);
  free(path_link);
}
