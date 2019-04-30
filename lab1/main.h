#ifndef __main_h__
#define __main_h__
typedef struct PID{
  pid_t pid;
  char name[3000];
  int iindex;
  int inodes[500];
}pid;

typedef struct INODE{
  int inode_n;
  int proto; //0==tcp4, 1==tcp6, 2==udp4, 3==udp6
  union local_IPS{
    struct in_addr ipv4;
    struct in6_addr ipv6;
  }local_ip;
  int local_port;

  union remote_IPS{
    struct in_addr ipv4;
    struct in6_addr ipv6;
  }remote_ip;
  int remote_port;
}inode;
#endif
